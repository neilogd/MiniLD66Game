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
		
		new ReField( "Health_", &GaUnitComponent::Health_, bcRFF_TRANSIENT ),
		new ReField( "Position_", &GaUnitComponent::Position_, bcRFF_TRANSIENT ),
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

	Health_ = MaxHealth_;
}


void GaUnitComponent::onDetach( ScnEntityWeakRef Parent )
{
	GameComponent_->deregisterUnit( this );

	Super::onDetach( Parent );
}

void GaUnitComponent::update( GaReal Tick )
{
}
