#include "GaGameComponent.h"

#include "GaUnitComponent.h"

#include "System/Debug/DsImGui.h"

#include "System/Scene/ScnComponentProcessor.h"
#include "System/Scene/ScnCore.h"
#include "System/Scene/ScnEntity.h"

REFLECTION_DEFINE_DERIVED( GaGameComponent );

void GaGameComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "TickHz_", &GaGameComponent::TickHz_, bcRFF_IMPORTER ),
		new ReField( "TestEntity_", &GaGameComponent::TestEntity_, bcRFF_IMPORTER ),
		
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

	TickRate_ = GaReal( 1.0f ) / GaReal( TickHz_ );
	TickAccumulator_ = TickRate_;
}


void GaGameComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );
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

	// Rendering.
	const BcF32 Alpha = InterpolatedRender ? TickAccumulator_ / TickRate_ : 0;
	for( auto* Unit : Units_ )
	{
		GaUnitState State = Unit->getInterpolatedState( Alpha );

		MaVec3d Position( State.Position_.x(), State.Position_.y(), State.Position_.z() );

		Unit->getParentEntity()->setLocalPosition( Position );
	}

}

void GaGameComponent::registerUnit( class GaUnitComponent* Unit )
{
	PendingRegisterUnits_.push_back( Unit );
}

void GaGameComponent::deregisterUnit( class GaUnitComponent* Unit )
{
	PendingDeregisterUnits_.push_back( Unit );
}
