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
	
	void updateState();
	void update( GaReal Tick );

	GaUnitState getState() const { return PrevState_; }
	GaUnitState getInterpolatedState( GaReal Alpha ) const;

	void setID( BcU32 ID ) { ID_ = ID; }
	BcU32 getID() const { return ID_; }

	void commandMove( GaVec3d MovePosition );

private:
	class GaGameComponent* GameComponent_;
	BcS32 MaxHealth_ = 100;
	BcS32 MaxVelocity_ = 1;

	BcU32 ID_ = BcErrorCode;

	GaUnitState CurrState_;
	GaUnitState PrevState_;
	
	// Movement.
	GaVec3d MovePosition_;

	
};
