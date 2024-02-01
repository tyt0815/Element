#include "Magic/BaseAiming.h"

#include "Components/ArrowComponent.h"

ABaseAiming::ABaseAiming()
{
	PrimaryActorTick.bCanEverTick = false;

	SetActorEnableCollision(false);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	SetRootComponent(Arrow);
	AimingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Aiming"));
	AimingMesh->SetupAttachment(GetRootComponent());
	AimingMesh->bVisibleInRayTracing = false;
	AimingMesh->bVisibleInRealTimeSkyCaptures = false;
	AimingMesh->bVisibleInReflectionCaptures = false;
	AimingMesh->bReceivesDecals = false;
	AimingMesh->bOnlyOwnerSee = true;
	AimingMesh->bUseAsOccluder = false;
	AimingMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AimingMesh->SetGenerateOverlapEvents(false);
	AimingMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
}

void ABaseAiming::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseAiming::SetAimingMeshVisibility(bool b)
{
	AimingMesh->SetVisibility(b);
}

void ABaseAiming::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

