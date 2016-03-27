#pragma once

#include "System/Scene/ScnComponent.h"
#include "System/Scene/Rendering/ScnShaderFileData.h"

#include "GaTypes.h"

struct GaUnitState
{
	GaReal Health_ = 0.0f;
	GaVec3d Position_;
	GaVec3d Velocity_;
	GaVec3d Acceleration_;
};


enum class GaUnitCommandType
{
	// Invalid command.
	INVALID,
	/// Move to target unit/location.
	MOVE,
	/// Stop all activity..
	STOP,
	/// Attack target unit/location.
	ATTACK,
	/// Change unit behaviour state.
	BEHAVIOUR,
};


class GaUnitCommand
{
public:
	REFLECTION_DECLARE_BASE( GaUnitCommand );

	GaUnitCommand():
		Name_(),
		Key_(),
		Type_( GaUnitCommandType::INVALID ),
		UnitID_( BcErrorCode )
	{}

	GaUnitCommand( GaUnitCommandType Type, BcU32 UnitID ):
		Name_(),
		Key_(),
		Type_( Type ),
		UnitID_( UnitID ),
		Location_(),
		Behaviour_()
	{}

	GaUnitCommand( GaUnitCommandType Type, GaVec3d Location ):
		Name_(),
		Key_(),
		Type_( Type ),
		UnitID_( BcErrorCode ),
		Location_( Location ),
		Behaviour_()
	{}

	/// Name of command. TODO: Static string?
	std::string Name_;
	/// Key for command. TODO: Static string?
	std::string Key_;
	/// Type of command.
	GaUnitCommandType Type_;
	/// Unit ID for MOVE, ATTACK.
	BcU32 UnitID_;
	/// Location for MOVE, ATTACK.
	GaVec3d Location_;
	/// Behaviour for command BEHAVIOUR. TODO: Static string?
	std::string Behaviour_;

	bool operator < ( const GaUnitCommand& Other ) const
	{
		return std::make_tuple( Key_, Type_, Name_ ) < std::make_tuple( Other.Key_, Other.Type_, Other.Name_ );
	}

	bool operator == ( const GaUnitCommand& Other ) const
	{
		return std::make_tuple( Key_, Type_, Name_ ) == std::make_tuple( Other.Key_, Other.Type_, Other.Name_ );
	}

	bool operator != ( const GaUnitCommand& Other ) const
	{
		return std::make_tuple( Key_, Type_, Name_ ) != std::make_tuple( Other.Key_, Other.Type_, Other.Name_ );
	}

};


class GaUnitBehaviour
{
public:
	REFLECTION_DECLARE_BASE( GaUnitBehaviour );

	GaUnitBehaviour(){}
	/// Name.
	std::string Name_;
	
	/// Max velocity.
	BcS32 MaxVelocity_ = 1;

	/// Rate of fire in projectiles/sec.
	BcS32 RateOfFire_ = 1;
	
	/// Attack projectile.
	ScnEntity* AttackProjectile_ = nullptr;

	/// Valid commands.
	std::vector< GaUnitCommand > Commands_;
};



class GaUnitComponent:
	public ScnComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaUnitComponent, ScnComponent );

	GaUnitComponent();
	virtual ~GaUnitComponent();

	void onAttach( ScnEntityWeakRef Parent ) override;
	void onDetach( ScnEntityWeakRef Parent ) override;

	/**
	 * Called to setup all state for game logic.
	 */
	void setupUnit( BcU32 ID, BcU32 TeamID, GaVec3d Position );

	/**
	 * Destroy unit. Will persist in the world still but play no part.
	 */
	void destroyUnit();
	
	void updateState();
	void update( GaReal Tick );

	const GaUnitBehaviour* getBehaviour() const { return CurrBehaviour_; }

	GaUnitState getState() const { return PrevState_; }
	GaUnitState getInterpolatedState( GaReal Alpha ) const;

	BcU32 getID() const { return ID_; }
	BcU32 getTeamID() const { return TeamID_; }

	void command( const GaUnitCommand& InCommand );

private:
	class GaGameComponent* GameComponent_ = nullptr;

	BcS32 MaxHealth_ = 100;

	BcBool DeleteBehaviour_ = BcFalse;
	std::vector< GaUnitBehaviour* > Behaviours_;
	const GaUnitBehaviour* CurrBehaviour_ = nullptr;

	// General.
	BcU32 TeamID_ = 0;

	ScnShaderMaterialUniformBlockData Material_;

	// Unit state.
	GaUnitState CurrState_;
	GaUnitState PrevState_;
		
	// Movement commands.
	GaVec3d MovePosition_;

	// Attack commands.


	BcU32 ID_ = BcErrorCode;

	
};
