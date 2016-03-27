#pragma once

#include "System/Scene/ScnComponent.h"

#include "System/Os/OsEvents.h"

#include "GaTypes.h"
#include "GaUnitComponent.h"

class GaGameComponent:
	public ScnComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaGameComponent, ScnComponent );

	GaGameComponent();
	virtual ~GaGameComponent();

	void onAttach( ScnEntityWeakRef Parent ) override;
	void onDetach( ScnEntityWeakRef Parent ) override;

	void onBeginDrag( OsEventInputMouse Event );
	void onUpdateDrag( OsEventInputMouse Event );
	void onEndDrag( OsEventInputMouse Event );
	void onCancelDrag( OsEventInputMouse Event );
	void onClick( OsEventInputMouse Event );

	void update( BcF32 Tick );

	void actionMenu();

	void drawMinimap();

	class GaUnitComponent* getUnit( BcU32 UnitID );

	void registerUnit( class GaUnitComponent* Unit );
	void deregisterUnit( class GaUnitComponent* Unit );

	void spawnUnit( class ScnEntity* BaseEntity, BcU32 TeamID, GaVec3d Position );
	void destroyUnit( BcU32 UnitID );

	void rebuildCommands();
	

private:
	BcS32 TickHz_ = 60;
	GaReal TickRate_ = 0.0f;
	GaReal TickAccumulator_ = 0.0f;
	BcU32 CurrentUnitID_ = 0;
	BcU32 TeamID_ = 0;

	std::vector< ScnEntity* > TestEntities_;

	std::vector< class GaUnitComponent* > Units_;
	std::vector< class GaUnitComponent* > PendingRegisterUnits_;
	std::vector< class GaUnitComponent* > PendingDeregisterUnits_;

	std::vector< class GaUnitComponent* > DestroyedUnits_;

	class GaCameraComponent* Camera_;
	class ScnMaterialComponent* Material_;
	class ScnCanvasComponent* Canvas_;

	std::vector< BcU32 > SelectedUnitIDs_;

	std::set< GaUnitCommand > Commands_;
	GaUnitCommand SelectedCommand_;

	// Mouse input.
	enum class InputState
	{
		IDLE,
		DOWN,
		DRAGGING,
	};
	std::array< InputState, 8 > InputState_;
	std::array< OsEventInputMouse, 8 > BeginDragMouseEvent_;

	OsEventInputMouse LastMouseEvent_;

	BcBool SelectionBoxEnable_;
	MaVec2d SelectionBoxA_;
	MaVec2d SelectionBoxB_;

};
