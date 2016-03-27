#pragma once

#include "System/Scene/ScnComponent.h"

#include "GaTypes.h"

struct GaUnitState
{
	GaReal Health_ = 0.0f;
	GaVec3d Position_;
	GaVec3d Velocity_;
	GaVec3d Acceleration_;
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

	GaUnitState getState() const { return PrevState_; }
	GaUnitState getInterpolatedState( GaReal Alpha ) const;

	BcU32 getID() const { return ID_; }
	BcU32 getTeamID() const { return TeamID_; }

	
	void commandMove( GaVec3d MovePosition );

private:
	class GaGameComponent* GameComponent_;

	BcS32 MaxHealth_ = 100;
	BcS32 MaxVelocity_ = 1;

	// General.
	BcU32 TeamID_ = 0;

	// Unit state.
	GaUnitState CurrState_;
	GaUnitState PrevState_;
		
	// Movement commands.
	GaVec3d MovePosition_;

	// Attack commands.


	BcU32 ID_ = BcErrorCode;

	
};
