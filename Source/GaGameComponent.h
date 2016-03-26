#pragma once

#include "System/Scene/ScnComponent.h"

#include "GaTypes.h"

class GaGameComponent:
	public ScnComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaGameComponent, ScnComponent );

	GaGameComponent();
	virtual ~GaGameComponent();

	void onAttach( ScnEntityWeakRef Parent ) override;
	void onDetach( ScnEntityWeakRef Parent ) override;

	void update( BcF32 Tick );

	void registerUnit( class GaUnitComponent* Unit );
	void deregisterUnit( class GaUnitComponent* Unit );

private:
	BcS32 TickHz_ = 60;
	GaReal TickRate_ = 0.0f;
	GaReal TickAccumulator_ = 0.0f;

	ScnEntity* TestEntity_ = nullptr;

	std::vector< class GaUnitComponent* > Units_;
	std::vector< class GaUnitComponent* > PendingRegisterUnits_;
	std::vector< class GaUnitComponent* > PendingDeregisterUnits_;



};
