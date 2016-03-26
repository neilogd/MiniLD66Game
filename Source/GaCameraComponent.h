/**************************************************************************
*
* File:		GaCameraComponent.h
* Author:	Neil Richardson 
* Ver/Date:	15/12/12	
* Description:
*		Camera component.
*		
*
*
* 
**************************************************************************/

#ifndef __GACAMERACOMPONENT_H__
#define __GACAMERACOMPONENT_H__

#include "Psybrus.h"

#include "System/Os/OsEvents.h"
#include "System/Scene/ScnEntity.h"

//////////////////////////////////////////////////////////////////////////
// GaExampleComponentRef
typedef ReObjectRef< class GaCameraComponent > GaCameraComponentRef;

//////////////////////////////////////////////////////////////////////////
// GaCameraComponent
class GaCameraComponent:
	public ScnComponent
{
public:
	REFLECTION_DECLARE_DERIVED( GaCameraComponent, ScnComponent );

	GaCameraComponent();
	virtual ~GaCameraComponent();

	void preUpdate( BcF32 Tick );

	void onAttach( ScnEntityWeakRef Parent ) override;
	void onDetach( ScnEntityWeakRef Parent ) override;

	eEvtReturn onMouseDown( EvtID ID, const EvtBaseEvent& Event );
	eEvtReturn onMouseUp( EvtID ID, const EvtBaseEvent& Event );
	eEvtReturn onMouseMove( EvtID ID, const EvtBaseEvent& Event );
	eEvtReturn onMouseWheel( EvtID ID, const EvtBaseEvent& Event );

	eEvtReturn onKeyDown( EvtID ID, const EvtBaseEvent& Event );
	eEvtReturn onKeyUp( EvtID ID, const EvtBaseEvent& Event );

	MaMat4d getCameraRotationMatrix() const;
	
private:
	MaVec3d CameraTarget_;
	MaVec3d CameraRotation_;
	MaVec3d CameraWalk_;
	BcF32 CameraDistance_;
	BcF32 CameraZoom_;
	BcBool MoveFast_;
	MaVec3d CameraRotationDelta_;

	enum CameraState
	{
		STATE_IDLE = 0,
		STATE_ROTATE,
		STATE_PAN
	};

	CameraState CameraState_;
	CameraState NextCameraState_;
	OsEventInputMouse LastMouseEvent_;
	OsEventInputKeyboard LastKeyboardEvent_;

	OsEventInputMouse InitialMouseEvent_;
	MaVec3d BaseCameraRotation_;

	int SelectedRenderer_ = 0;
	std::vector< ScnEntityRef > Renderers_;
	ScnEntityRef SpawnedRenderer_;
};

#endif

