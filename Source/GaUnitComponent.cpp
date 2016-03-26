#include "GaUnitComponent.h"

#include "GaGameComponent.h"

#include "System/Scene/ScnComponentProcessor.h"
#include "System/Scene/ScnEntity.h"

REFLECTION_DEFINE_DERIVED( GaUnitComponent );

void GaUnitComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "MaxHealth_", &GaUnitComponent::MaxHealth_, bcRFF_IMPORTER ),
		new ReField( "MaxVelocity_", &GaUnitComponent::MaxVelocity_, bcRFF_IMPORTER ),
		
	};

	using namespace std::placeholders;
	ReRegisterClass< GaUnitComponent, Super >( Fields );
}


GaUnitComponent::GaUnitComponent()
{
}


GaUnitComponent::~GaUnitComponent()
{
}


void GaUnitComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );

	GameComponent_ = Parent->getComponentAnyParentByType< GaGameComponent >();
	BcAssert( GameComponent_ );

	GameComponent_->registerUnit( this );

	CurrState_.Health_ = MaxHealth_;


	PrevState_ = CurrState_;
}


void GaUnitComponent::onDetach( ScnEntityWeakRef Parent )
{
	GameComponent_->deregisterUnit( this );

	Super::onDetach( Parent );
}


void GaUnitComponent::updateState()
{
	PrevState_ = CurrState_;
}


void GaUnitComponent::update( GaReal Tick )
{
	// Do behaviour.
	// Always use the prev state.
	{
		
	}

	// Do movement.
	{
		const auto DistanceRemaining = ( MovePosition_ - CurrState_.Position_ ).magnitude();
		if( DistanceRemaining > 0.0f )
		{
			auto MoveVector = CurrState_.Velocity_ * Tick;
			if( MoveVector.magnitude() > DistanceRemaining )
			{
				// Clamp to final position and reset velocity + acceleration.
				CurrState_.Position_ = MovePosition_;
				CurrState_.Velocity_ = GaVec3d( 0.0f, 0.0f, 0.0f );
				CurrState_.Acceleration_ = GaVec3d( 0.0f, 0.0f, 0.0f );
			}
			else
			{
				// Euler integration. Inaccuracy doesn't concern me for this game.
				CurrState_.Position_ = CurrState_.Position_ + MoveVector;
				CurrState_.Velocity_ = CurrState_.Velocity_ + CurrState_.Acceleration_ * Tick;

				CurrState_.Velocity_ = ( MovePosition_ - CurrState_.Position_ ).normal() * MaxVelocity_;
			}
		}
		else
		{
			// Clamp to final position and reset velocity + acceleration.
			CurrState_.Position_ = MovePosition_;
			CurrState_.Velocity_ = GaVec3d( 0.0f, 0.0f, 0.0f );
			CurrState_.Acceleration_ = GaVec3d( 0.0f, 0.0f, 0.0f );
		}
	}
}


GaUnitState GaUnitComponent::getInterpolatedState( GaReal Alpha ) const
{
	GaUnitState State;
	State.Health_ = PrevState_.Health_ + ( CurrState_.Health_ - PrevState_.Health_ ) * Alpha;
	State.Position_.lerp( PrevState_.Position_, CurrState_.Position_, Alpha );
	State.Velocity_.lerp( PrevState_.Velocity_, CurrState_.Velocity_, Alpha );
	State.Acceleration_.lerp( PrevState_.Acceleration_, CurrState_.Acceleration_, Alpha );
	return State;
}


void GaUnitComponent::commandMove( GaVec3d MovePosition )
{
	MovePosition_ = MovePosition;
}
