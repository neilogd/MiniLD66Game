#pragma once

#include "System/Scene/ScnComponent.h"

#include "GaTypes.h"

class GaUnitComponent:
	public ScnComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaUnitComponent, ScnComponent );

	GaUnitComponent();
	virtual ~GaUnitComponent();

	void onAttach( ScnEntityWeakRef Parent ) override;
	void onDetach( ScnEntityWeakRef Parent ) override;

	void update( GaReal Tick );


private:
	class GaGameComponent* GameComponent_;
	BcS32 MaxHealth_ = 100;
		
	GaReal Health_ = 0.0f;
	GaVec3d Position_;
};
