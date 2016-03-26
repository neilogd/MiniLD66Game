#include "GaGameComponent.h"

#include "GaCameraComponent.h"
#include "GaUnitComponent.h"

#include "System/Debug/DsImGui.h"

#include "System/Scene/ScnComponentProcessor.h"
#include "System/Scene/ScnCore.h"
#include "System/Scene/ScnEntity.h"

#include "System/Scene/Rendering/ScnDebugRenderComponent.h"
#include "System/Scene/Rendering/ScnViewComponent.h"

#include "System/Os/OsCore.h"


REFLECTION_DEFINE_DERIVED( GaGameComponent );

void GaGameComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "TickHz_", &GaGameComponent::TickHz_, bcRFF_IMPORTER ),
		new ReField( "TestEntity_", &GaGameComponent::TestEntity_, bcRFF_IMPORTER | bcRFF_SHALLOW_COPY ),
		
		new ReField( "TickRate_", &GaGameComponent::TickRate_, bcRFF_TRANSIENT ),
		new ReField( "TickAccumulator_", &GaGameComponent::TickAccumulator_, bcRFF_TRANSIENT ),
	};

	using namespace std::placeholders;
	ReRegisterClass< GaGameComponent, Super >( Fields )
		.addAttribute( new ScnComponentProcessor( 
			{
				ScnComponentProcessFuncEntry::Update< GaGameComponent >()
			} ) );
}


GaGameComponent::GaGameComponent()
{
}


GaGameComponent::~GaGameComponent()
{
}


void GaGameComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );

	Camera_ = Parent->getComponentAnyParentByType< ScnEntity >( "CameraEntity" )->getComponentByType< GaCameraComponent >();
	BcAssert( Camera_ );

	TickRate_ = GaReal( 1.0f ) / GaReal( TickHz_ );
	TickAccumulator_ = TickRate_;
	
	// Input events.
	OsCore::pImpl()->subscribe( osEVT_INPUT_MOUSEDOWN, this, 
		[ this ]( EvtID ID, const EvtBaseEvent& InEvent )
		{
			auto Event = InEvent.get< OsEventInputMouse >();

			if( Event.ButtonCode_ == 0 )
			{
				switch( InputState_ )
				{
				case InputState::IDLE:
					InputState_ = InputState::DOWN;
					BeginDragMouseEvent_ = Event;
					break;
				case InputState::DOWN:
					InputState_ = InputState::DOWN;
					BeginDragMouseEvent_ = Event;
					break;
				case InputState::DRAGGING:
					InputState_ = InputState::DRAGGING;
					onCancelDrag( Event ); // Shouldn't have been dragging, cancel it.
					BeginDragMouseEvent_ = Event;
					onBeginDrag( Event );
					break;
				}
			}

			LastMouseEvent_ = Event;
			return evtRET_PASS;
		} );

	OsCore::pImpl()->subscribe( osEVT_INPUT_MOUSEUP, this,
		[ this ]( EvtID ID, const EvtBaseEvent& InEvent )
		{
			auto Event = InEvent.get< OsEventInputMouse >();

			if( Event.ButtonCode_ == 0 )
			{
				switch( InputState_ )
				{
				case InputState::IDLE:
					InputState_ = InputState::DOWN;
					break;
				case InputState::DOWN:
					InputState_ = InputState::IDLE;
					onClick( Event );
					break;
				case InputState::DRAGGING:
					InputState_ = InputState::IDLE;
					onEndDrag( Event );
					break;
				}
			}
			else
			{
				onClick( Event );
			}
			LastMouseEvent_ = Event;
			return evtRET_PASS;
		} );

	OsCore::pImpl()->subscribe( osEVT_INPUT_MOUSEMOVE, this,
		[ this ]( EvtID ID, const EvtBaseEvent& InEvent )
		{
			auto Event = InEvent.get< OsEventInputMouse >();

			if( Event.ButtonCode_ == 0 )
			{
				switch( InputState_ )
				{
				case InputState::IDLE:
					InputState_ = InputState::IDLE;
					break;
				case InputState::DOWN:
					{
						InputState_ = InputState::DOWN;

						// If mouse moves enough, switch to dragging.
						MaVec2d A( BeginDragMouseEvent_.MouseX_, BeginDragMouseEvent_.MouseY_ );
						MaVec2d B( Event.MouseX_, Event.MouseY_ );

						// Wide range for drag for non-mouse people.
						if( ( A - B ).magnitude() > 12.0f )
						{
							InputState_ = InputState::DRAGGING;
							onBeginDrag( Event );
						}
					}				
					break;
				case InputState::DRAGGING:
					InputState_ = InputState::DRAGGING;
					onUpdateDrag( Event );
					break;
				}
			}

			LastMouseEvent_ = Event;
			return evtRET_PASS;
		} );

}


void GaGameComponent::onDetach( ScnEntityWeakRef Parent )
{
	OsCore::pImpl()->unsubscribeAll( this );

	Super::onDetach( Parent );
}

void GaGameComponent::onBeginDrag( OsEventInputMouse Event )
{
	SelectionBoxEnable_ = BcTrue;
	SelectionBoxA_ = MaVec2d( Event.MouseX_, Event.MouseY_ );
	SelectionBoxB_ = MaVec2d( Event.MouseX_, Event.MouseY_ );
}


void GaGameComponent::onUpdateDrag( OsEventInputMouse Event )
{
	SelectionBoxB_ = MaVec2d( Event.MouseX_, Event.MouseY_ );
}


void GaGameComponent::onEndDrag( OsEventInputMouse Event )
{
	SelectionBoxB_ = MaVec2d( Event.MouseX_, Event.MouseY_ );
	SelectionBoxEnable_ = BcFalse;

	// Do selection.
}


void GaGameComponent::onCancelDrag( OsEventInputMouse Event )
{
	SelectionBoxEnable_ = BcFalse;
}


void GaGameComponent::onClick( OsEventInputMouse Event )
{
	if( Event.ButtonCode_ == 0 )
	{
		SelectedUnitIDs_.clear();

		// Check for unit.
		for( auto* Unit : Units_ )
		{
			MaVec3d NearPos, FarPos;
			Camera_->getWorldPosition( MaVec2d( Event.MouseX_, Event.MouseY_ ), NearPos, FarPos );
			auto SpatialComponent = Unit->getComponentByType< ScnSpatialComponent >();
			if( SpatialComponent != nullptr )
			{
				auto AABB = SpatialComponent->getAABB();

				MaVec3d Intersection;
				if( AABB.lineIntersect( NearPos, FarPos, &Intersection, nullptr ) )
				{	
					SelectedUnitIDs_.push_back( Unit->getID() );
				}
			}
		}
	}

	if( Event.ButtonCode_ == 1 )
	{
		// If we have selection, move to position on grid.
		MaVec3d NearPos, FarPos;
		Camera_->getWorldPosition( MaVec2d( Event.MouseX_, Event.MouseY_ ), NearPos, FarPos );

		MaPlane GroundPlane;
		GroundPlane.fromPointNormal( MaVec3d( 0.0f, 0.0f, 0.0f ), MaVec3d( 0.0f, 1.0f, 0.0f ) );
		auto Dir = ( FarPos - NearPos ).normal();
		BcF32 Distance = 0.0f;
		MaVec3d Intersection;
		if( GroundPlane.lineIntersection( NearPos, FarPos, Distance, Intersection ) )
		{
			// Send all units to this position.
			// Draw selection (debug).
			for( auto UnitID : SelectedUnitIDs_ )
			{
				auto* Unit = getUnit( UnitID );
				if( Unit )
				{
					Unit->commandMove( GaVec3d( Intersection.x(), Intersection.y(), Intersection.z() ) );
				}
			}
		}

	}
}


void GaGameComponent::update( BcF32 Tick )
{
	static bool InterpolatedRender = true;

#if !PSY_PRODUCTION
	if ( ImGui::Begin( "Engine Debug" ) )
	{
		if( ImGui::TreeNode( "GaGameComponent" ) )
		{
			ImGui::Checkbox( "Enable interpolated render", &InterpolatedRender );

			if( ImGui::Button( "Spawn test entity" ) )
			{
				ScnCore::pImpl()->spawnEntity( ScnEntitySpawnParams( BcName::INVALID, TestEntity_, MaMat4d(), getParentEntity() ) );
			}

			if( Units_.size() > 0 )
			{
				static float TargetPosition[3] = { 0.0f, 0.0f, 0.0f };
				ImGui::InputFloat3( "Target Position", TargetPosition );
				if( ImGui::Button( "Test Target Position" ) )
				{
					Units_.back()->commandMove( GaVec3d( TargetPosition[0], TargetPosition[1], TargetPosition[2] ) );
				}			
			}
			ImGui::TreePop();
		}
	}
	ImGui::End();
#endif

	// Simulation.
	TickAccumulator_ += Tick;
	while( TickAccumulator_ >= TickRate_ )
	{
		TickAccumulator_ -= TickRate_;

		// Add all pending units.
		for( auto* Unit : PendingRegisterUnits_ )
		{
			Units_.push_back( Unit );
		}
		PendingRegisterUnits_.clear();

		// Update unit state.
		for( auto* Unit : Units_ )
		{
			Unit->updateState();
		}

		// Update units.
		for( auto* Unit : Units_ )
		{
			Unit->update( TickRate_ );
		}

		// Remove all pending units.
		for( auto* Unit : PendingDeregisterUnits_ )
		{
			auto FoundIt = std::find( Units_.begin(), Units_.end(), Unit );
			if( FoundIt != Units_.end() )
			{
				Units_.erase( FoundIt );
			}
		}
		PendingDeregisterUnits_.clear();
	}

#if !PSY_PRODUCTION
	auto DebugRender = ScnDebugRenderComponent::pImpl();
	BcAssert( DebugRender );

	DebugRender->drawGrid( MaVec3d( 0.0f, 0.0f, 0.0f ), MaVec3d( 1000.0f, 0.0f, 1000.0f ), 1.0f, 10.0f );
#endif

	// Rendering.
	const BcF32 Alpha = InterpolatedRender ? TickAccumulator_ / TickRate_ : 0;
	for( auto* Unit : Units_ )
	{
		GaUnitState State = Unit->getInterpolatedState( Alpha );

		MaVec3d Position( State.Position_.x(), State.Position_.y(), State.Position_.z() );

		Unit->getParentEntity()->setLocalPosition( Position );
	}

	// Draw selection (debug).
	for( auto UnitID : SelectedUnitIDs_ )
	{
		auto* Unit = getUnit( UnitID );
		if( Unit )
		{
			GaUnitState State = Unit->getInterpolatedState( Alpha );
			MaVec3d Position( State.Position_.x(), State.Position_.y(), State.Position_.z() );

			// Debug rendering.
#if !PSY_PRODUCTION
			MaVec3d Velocity( State.Velocity_.x(), State.Velocity_.y(), State.Velocity_.z() );
			DebugRender->drawLine( Position, Position + Velocity, RsColour::GREEN, 0 );
			DebugRender->drawEllipsoid( Position, MaVec3d( 0.51f, 0.51f, 0.51f ), RsColour::GREEN, 0 );
#endif
		}
	}

	// Mouse debug.
	{
#if !PSY_PRODUCTION
		MaVec2d MousePosition( LastMouseEvent_.MouseX_, LastMouseEvent_.MouseY_ );
		MaVec3d NearPos, FarPos;
		Camera_->getWorldPosition( MousePosition, NearPos, FarPos );

		MaPlane GroundPlane;
		GroundPlane.fromPointNormal( MaVec3d( 0.0f, 0.0f, 0.0f ), MaVec3d( 0.0f, 1.0f, 0.0f ) );
		auto Dir = ( FarPos - NearPos ).normal();
		BcF32 Distance = 0.0f;
		MaVec3d Intersection;
		if( GroundPlane.lineIntersection( NearPos, FarPos, Distance, Intersection ) )
		{
			DebugRender->drawEllipsoid( Intersection, MaVec3d( 1.0f, 1.0f, 1.0f ), RsColour::WHITE, 0 );
		}
#endif
	}

}

class GaUnitComponent* GaGameComponent::getUnit( BcU32 UnitID )
{
	// TODO: Lookup table.
	for( auto* Unit : Units_ )
	{
		if( Unit->getID() == UnitID )
		{
			return Unit;
		}
	}
	return nullptr;
}

void GaGameComponent::registerUnit( class GaUnitComponent* Unit )
{
	Unit->setID( CurrentUnitID_++ );
	PendingRegisterUnits_.push_back( Unit );
}

void GaGameComponent::deregisterUnit( class GaUnitComponent* Unit )
{
	PendingDeregisterUnits_.push_back( Unit );
}
