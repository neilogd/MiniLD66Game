#include "Math/MaAABB.h"
#include "Math/MaCPUVec2d.h"
#include "Math/MaCPUVec3d.h"
#include "Math/MaCPUVec4d.h"
#include "Math/MaMat3d.h"
#include "Math/MaMat4d.h"
#include "Math/MaPlane.h"
#include "Math/MaQuat.h"
#include "Reflection/ReAttributable.h"
#include "Reflection/ReAttribute.h"
#include "Reflection/ReClass.h"
#include "Reflection/ReClassSerialiser.h"
#include "Reflection/ReEnum.h"
#include "Reflection/ReEnumConstant.h"
#include "Reflection/ReField.h"
#include "Reflection/ReITypeSerialiser.h"
#include "Reflection/ReObject.h"
#include "Reflection/RePrimitive.h"
#include "System/Content/CsPackage.h"
#include "System/Content/CsPackageDependencies.h"
#include "System/Content/CsPlatformParams.h"
#include "System/Content/CsRedirector.h"
#include "System/Content/CsResource.h"
#include "System/Content/CsResourceImporter.h"
#include "System/Content/CsTypes.h"
#include "System/Debug/DsImGuiFieldEditor.h"
#include "System/File/FsTypes.h"
#include "System/Renderer/RsCore.h"
#include "System/Renderer/RsCoreImpl.h"
#include "System/Renderer/RsRenderState.h"
#include "System/Renderer/RsSamplerState.h"
#include "System/Renderer/RsTypes.h"
#include "System/Scene/Animation/ScnAnimation.h"
#include "System/Scene/Animation/ScnAnimationComponent.h"
#include "System/Scene/Animation/ScnAnimationPose.h"
#include "System/Scene/Animation/ScnAnimationTreeBlendNode.h"
#include "System/Scene/Animation/ScnAnimationTreeNode.h"
#include "System/Scene/Animation/ScnAnimationTreeTrackNode.h"
#include "System/Scene/Import/ScnAnimationImport.h"
#include "System/Scene/Import/ScnComponentImport.h"
#include "System/Scene/Import/ScnEntityImport.h"
#include "System/Scene/Import/ScnFontImport.h"
#include "System/Scene/Import/ScnMaterialImport.h"
#include "System/Scene/Import/ScnModelImport.h"
#include "System/Scene/Import/ScnShaderImport.h"
#include "System/Scene/Import/ScnSoundImport.h"
#include "System/Scene/Import/ScnTextureImport.h"
#include "System/Scene/Import/ScnTileMapImport.h"
#include "System/Scene/Physics/ScnPhysicsBoxCollisionComponent.h"
#include "System/Scene/Physics/ScnPhysicsCapsuleCollisionComponent.h"
#include "System/Scene/Physics/ScnPhysicsCollisionComponent.h"
#include "System/Scene/Physics/ScnPhysicsConstraintComponent.h"
#include "System/Scene/Physics/ScnPhysicsCylinderCollisionComponent.h"
#include "System/Scene/Physics/ScnPhysicsHingeConstraintComponent.h"
#include "System/Scene/Physics/ScnPhysicsMesh.h"
#include "System/Scene/Physics/ScnPhysicsMeshCollisionComponent.h"
#include "System/Scene/Physics/ScnPhysicsMeshImport.h"
#include "System/Scene/Physics/ScnPhysicsRigidBodyComponent.h"
#include "System/Scene/Physics/ScnPhysicsSphereCollisionComponent.h"
#include "System/Scene/Physics/ScnPhysicsWorldComponent.h"
#include "System/Scene/Rendering/ScnCanvasComponent.h"
#include "System/Scene/Rendering/ScnDebugRenderComponent.h"
#include "System/Scene/Rendering/ScnDeferredLightingComponent.h"
#include "System/Scene/Rendering/ScnFont.h"
#include "System/Scene/Rendering/ScnLightComponent.h"
#include "System/Scene/Rendering/ScnMaterial.h"
#include "System/Scene/Rendering/ScnModel.h"
#include "System/Scene/Rendering/ScnParticleSystemComponent.h"
#include "System/Scene/Rendering/ScnPostProcessComponent.h"
#include "System/Scene/Rendering/ScnRenderableComponent.h"
#include "System/Scene/Rendering/ScnShader.h"
#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnSpriteComponent.h"
#include "System/Scene/Rendering/ScnTexture.h"
#include "System/Scene/Rendering/ScnTextureAtlas.h"
#include "System/Scene/Rendering/ScnTileMap.h"
#include "System/Scene/Rendering/ScnViewComponent.h"
#include "System/Scene/ScnComponent.h"
#include "System/Scene/ScnComponentProcessor.h"
#include "System/Scene/ScnEntity.h"
#include "System/Scene/ScnSpatialComponent.h"
#include "System/Scene/Sound/ScnSound.h"
#include "System/Scene/Sound/ScnSoundEmitter.h"
#include "System/Scene/Sound/ScnSoundListenerComponent.h"
#include "System/Sound/SoLoud/SsCoreImplSoLoud.h"
#include "System/Sound/SsCore.h"
#include "System/SysKernel.h"
#include "System/SysSystem.h"
#include "GaCameraComponent.h"
#include "GaGameComponent.h"
#include "GaUnitComponent.h"
void AutoGenRegisterReflection()
{
	MaAABB::StaticRegisterClass();
	MaCPUVec2d::StaticRegisterClass();
	MaCPUVec3d::StaticRegisterClass();
	MaCPUVec4d::StaticRegisterClass();
	MaMat3d::StaticRegisterClass();
	MaMat4d::StaticRegisterClass();
	MaPlane::StaticRegisterClass();
	MaQuat::StaticRegisterClass();
	ReAttributable::StaticRegisterClass();
	ReAttribute::StaticRegisterClass();
	ReClass::StaticRegisterClass();
	ReClassSerialiser::StaticRegisterClass();
	ReEnum::StaticRegisterClass();
	ReEnumConstant::StaticRegisterClass();
	ReField::StaticRegisterClass();
	ReITypeSerialiser::StaticRegisterClass();
	ReObject::StaticRegisterClass();
	RePrimitive::StaticRegisterClass();
	CsPackage::StaticRegisterClass();
	CsPackageDependencies::StaticRegisterClass();
	CsPlatformParams::StaticRegisterClass();
	CsRedirectorImport::StaticRegisterClass();
	CsRedirector::StaticRegisterClass();
	CsResource::StaticRegisterClass();
	CsResourceImporterAttribute::StaticRegisterClass();
	CsResourceImporter::StaticRegisterClass();
	CsDependency::StaticRegisterClass();
	DsImGuiFieldEditor::StaticRegisterClass();
	FsTimestamp::StaticRegisterClass();
	FsStats::StaticRegisterClass();
	RsCore::StaticRegisterClass();
	RsCoreImpl::StaticRegisterClass();
	RsRenderTargetBlendState::StaticRegisterClass();
	RsBlendState::StaticRegisterClass();
	RsStencilFaceState::StaticRegisterClass();
	RsDepthStencilState::StaticRegisterClass();
	RsRasteriserState::StaticRegisterClass();
	RsRenderStateDesc::StaticRegisterClass();
	RsSamplerStateDesc::StaticRegisterClass();
	RsColour::StaticRegisterClass();
	ScnAnimation::StaticRegisterClass();
	ScnAnimationComponent::StaticRegisterClass();
	ScnAnimationPose::StaticRegisterClass();
	ScnAnimationTreeBlendNode::StaticRegisterClass();
	ScnAnimationTreeNode::StaticRegisterClass();
	ScnAnimationTreeTrackNode::StaticRegisterClass();
	ScnAnimationImport::StaticRegisterClass();
	ScnComponentImport::StaticRegisterClass();
	ScnEntityImport::StaticRegisterClass();
	ScnFontImport::StaticRegisterClass();
	ScnMaterialImport::StaticRegisterClass();
	ScnModelVertexFormat::StaticRegisterClass();
	ScnModelMaterialDesc::StaticRegisterClass();
	ScnModelImport::StaticRegisterClass();
	ScnShaderImport::StaticRegisterClass();
	ScnSoundImport::StaticRegisterClass();
	ScnTextureImport::StaticRegisterClass();
	ScnTileMapImport::StaticRegisterClass();
	ScnPhysicsBoxCollisionComponent::StaticRegisterClass();
	ScnPhysicsCapsuleCollisionComponent::StaticRegisterClass();
	ScnPhysicsCollisionComponent::StaticRegisterClass();
	ScnPhysicsConstraintComponent::StaticRegisterClass();
	ScnPhysicsCylinderCollisionComponent::StaticRegisterClass();
	ScnPhysicsHingeConstraintComponent::StaticRegisterClass();
	ScnPhysicsMesh::StaticRegisterClass();
	ScnPhysicsMeshCollisionComponent::StaticRegisterClass();
	ScnPhysicsMeshImport::StaticRegisterClass();
	ScnPhysicsRigidBodyComponent::StaticRegisterClass();
	ScnPhysicsSphereCollisionComponent::StaticRegisterClass();
	ScnPhysicsWorldComponent::StaticRegisterClass();
	ScnCanvasComponent::StaticRegisterClass();
	ScnDebugRenderComponent::StaticRegisterClass();
	ScnDeferredLightingVertex::StaticRegisterClass();
	ScnDeferredLightingComponent::StaticRegisterClass();
	ScnFont::StaticRegisterClass();
	ScnFontUniformBlockData::StaticRegisterClass();
	ScnFontDrawParams::StaticRegisterClass();
	ScnFontComponent::StaticRegisterClass();
	ScnLightComponent::StaticRegisterClass();
	ScnMaterial::StaticRegisterClass();
	ScnMaterialComponent::StaticRegisterClass();
	ScnModelUniforms::StaticRegisterClass();
	ScnModel::StaticRegisterClass();
	ScnModelComponent::StaticRegisterClass();
	ScnParticleSystemComponent::StaticRegisterClass();
	ScnPostProcessVertex::StaticRegisterClass();
	ScnPostProcessUniforms::StaticRegisterClass();
	ScnPostProcessNode::StaticRegisterClass();
	ScnPostProcessComponent::StaticRegisterClass();
	ScnRenderableComponent::StaticRegisterClass();
	ScnShader::StaticRegisterClass();
	ScnShaderViewUniformBlockData::StaticRegisterClass();
	ScnShaderLightUniformBlockData::StaticRegisterClass();
	ScnShaderMaterialUniformBlockData::StaticRegisterClass();
	ScnShaderObjectUniformBlockData::StaticRegisterClass();
	ScnShaderBoneUniformBlockData::StaticRegisterClass();
	ScnShaderAlphaTestUniformBlockData::StaticRegisterClass();
	ScnShaderPostProcessConfigData::StaticRegisterClass();
	ScnShaderPostProcessCopyBlockData::StaticRegisterClass();
	ScnShaderPostProcessBlurBlockData::StaticRegisterClass();
	ScnSpriteAnimation::StaticRegisterClass();
	ScnSpriteComponent::StaticRegisterClass();
	ScnTexture::StaticRegisterClass();
	ScnTextureAtlas::StaticRegisterClass();
	ScnTileMap::StaticRegisterClass();
	ScnTileMapComponent::StaticRegisterClass();
	ScnViewComponent::StaticRegisterClass();
	ScnComponent::StaticRegisterClass();
	ScnComponentProcessor::StaticRegisterClass();
	ScnEntity::StaticRegisterClass();
	ScnSpatialComponent::StaticRegisterClass();
	ScnSound::StaticRegisterClass();
	ScnSoundEmitterComponent::StaticRegisterClass();
	ScnSoundListenerComponent::StaticRegisterClass();
	SsCoreImplSoLoud::StaticRegisterClass();
	SsCore::StaticRegisterClass();
	SysKernel::StaticRegisterClass();
	SysSystem::StaticRegisterClass();
	GaCameraComponent::StaticRegisterClass();
	GaGameComponent::StaticRegisterClass();
	GaUnitCommand::StaticRegisterClass();
	GaUnitBehaviour::StaticRegisterClass();
	GaUnitComponent::StaticRegisterClass();
}
