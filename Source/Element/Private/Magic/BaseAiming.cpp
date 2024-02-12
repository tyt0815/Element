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
	SetOnlyOwnerSee(AimingMesh);
}

void ABaseAiming::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseAiming::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);
	AimingMesh->SetHiddenInGame(bNewHidden);
}

void ABaseAiming::SetOnlyOwnerSee(UMeshComponent* MeshComponent)
{
	MeshComponent->bVisibleInRayTracing = false;
	MeshComponent->bVisibleInRealTimeSkyCaptures = false;
	MeshComponent->bVisibleInReflectionCaptures = false;
	MeshComponent->bReceivesDecals = false;
	MeshComponent->bOnlyOwnerSee = true;
	MeshComponent->bUseAsOccluder = false;
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetGenerateOverlapEvents(false);
	MeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
}

void ABaseAiming::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

