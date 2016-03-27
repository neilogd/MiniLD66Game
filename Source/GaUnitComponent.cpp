#include "GaUnitComponent.h"

#include "GaGameComponent.h"

#include "System/Scene/ScnComponentProcessor.h"
#include "System/Scene/ScnEntity.h"

#include "System/Scene/Rendering/ScnModel.h"

REFLECTION_DEFINE_BASE( GaUnitCommand );

void GaUnitCommand::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "Name_", &GaUnitCommand::Name_, bcRFF_IMPORTER ),
		new ReField( "Key_", &GaUnitCommand::Key_, bcRFF_IMPORTER ),
		new ReField( "Type_", &GaUnitCommand::Type_, bcRFF_IMPORTER ),
		new ReField( "UnitID_", &GaUnitCommand::UnitID_, bcRFF_IMPORTER ),
		new ReField( "Location_", &GaUnitCommand::Location_, bcRFF_IMPORTER ),
		new ReField( "Behaviour_", &GaUnitCommand::Behaviour_, bcRFF_IMPORTER ),
	};

	using namespace std::placeholders;
	ReRegisterClass< GaUnitCommand >( Fields );


	ReEnumConstant* GaUnitCommandTypeEnumConstants[] = 
	{
		new ReEnumConstant( "MOVE", GaUnitCommandType::MOVE ),
		new ReEnumConstant( "STOP", GaUnitCommandType::STOP ),
		new ReEnumConstant( "ATTACK", GaUnitCommandType::ATTACK ),
		new ReEnumConstant( "BEHAVIOUR", GaUnitCommandType::BEHAVIOUR )
	};
	ReRegisterEnum< GaUnitCommandType >( GaUnitCommandTypeEnumConstants );
}


REFLECTION_DEFINE_BASE( GaUnitBehaviour );

void GaUnitBehaviour::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "Name_", &GaUnitBehaviour::Name_, bcRFF_IMPORTER ),
		new ReField( "MaxVelocity_", &GaUnitBehaviour::MaxVelocity_, bcRFF_IMPORTER ),
		new ReField( "RateOfFire_", &GaUnitBehaviour::RateOfFire_, bcRFF_IMPORTER ),
		new ReField( "AttackProjectile_", &GaUnitBehaviour::AttackProjectile_, bcRFF_IMPORTER | bcRFF_SHALLOW_COPY ),
		new ReField( "Commands_", &GaUnitBehaviour::Commands_, bcRFF_IMPORTER ),
	};

	using namespace std::placeholders;
	ReRegisterClass< GaUnitBehaviour >( Fields );
}

REFLECTION_DEFINE_DERIVED( GaUnitComponent );


void GaUnitComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "MaxHealth_", &GaUnitComponent::MaxHealth_, bcRFF_IMPORTER ),
		new ReField( "Behaviours_", &GaUnitComponent::Behaviours_, bcRFF_IMPORTER | bcRFF_SHALLOW_COPY ),
	};

	using namespace std::placeholders;
	ReRegisterClass< GaUnitComponent, Super >( Fields );
}


GaUnitComponent::GaUnitComponent()
{
}


GaUnitComponent::~GaUnitComponent()
{
	// We are the basis or need to delete them.
	if( getBasis() == nullptr || DeleteBehaviour_ )
	{
		for( auto Behaviour : Behaviours_ )
		{
			delete Behaviour;
		}
	}
	Behaviours_.clear();
}


void GaUnitComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );

	GameComponent_ = Parent->getComponentAnyParentByType< GaGameComponent >();
	BcAssert( GameComponent_ );

	CurrState_.Health_ = MaxHealth_;

	PrevState_ = CurrState_;
}


void GaUnitComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );
}


void GaUnitComponent::setupUnit( BcU32 ID, BcU32 TeamID, GaVec3d Position )
{
	ID_ = ID;
	TeamID_ = ID;
	CurrState_.Position_ = Position;
	CurrState_.Health_ = MaxHealth_;

	PrevState_ = CurrState_;

	// Set default state.
	BcAssert( Behaviours_.size() > 0 );
	CurrBehaviour_ = Behaviours_[ 0 ];
	
	// Team colours hack.
	std::array< RsColour, 2 > TeamColour =  
	{
		RsColour::RED,
		RsColour::BLUE,
	};

	auto Model = getComponentByType< ScnModelComponent >();
	BcAssert( Model );
	Material_.MaterialBaseColour_ = TeamColour[ TeamID ];
	
	Model->setUniforms( Material_ );

}


void GaUnitComponent::destroyUnit()
{
	updateState();
}


void GaUnitComponent::updateState()
{
	PrevState_ = CurrState_;
}


void GaUnitComponent::update( GaReal Tick )
{
	BcAssert( CurrBehaviour_ );

	// Do behaviour related stuff.
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

				CurrState_.Velocity_ = ( MovePosition_ - CurrState_.Position_ ).normal() * CurrBehaviour_->MaxVelocity_;
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


void GaUnitComponent::command( const GaUnitCommand& InCommand )
{
	// Check it can be ran.
	for( const auto& Command : CurrBehaviour_->Commands_ )
	{
		if( InCommand.Type_ == Command.Type_ )
		{
			switch( InCommand.Type_ )
			{
			case GaUnitCommandType::MOVE:
				if( InCommand.UnitID_ == BcErrorCode )
				{
					MovePosition_ = InCommand.Location_;
				}
				else
				{
					BcBreakpoint;
				}
				break;
			case GaUnitCommandType::STOP:
				MovePosition_ = CurrState_.Position_;
				break;
			case GaUnitCommandType::ATTACK:
				PSY_LOG( "Unimplemented" );
				break;
			case GaUnitCommandType::BEHAVIOUR:
				for( const auto& Behaviour : Behaviours_ )
				{
					if( InCommand.Behaviour_ == Behaviour->Name_ )
					{
						CurrBehaviour_ = Behaviour;

						return;
					}
				}
				break;			
			}
			break;
		}
	}
}
