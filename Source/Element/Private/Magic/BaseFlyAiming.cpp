#include "Magic/BaseFlyAiming.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Element/DebugMacro.h"

ABaseFlyAiming::ABaseFlyAiming()
{
	PrimaryActorTick.bCanEverTick = true;

	SplineMesh = CreateDefaultSubobject<USplineMeshComponent>(TEXT("SplineMesh"));
	SplineMesh->SetupAttachment(GetRootComponent());
	SetOnlyOwnerSee(SplineMesh);
	SplineMesh->SetMobility(EComponentMobility::Movable);
	FloorMarkingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMarking"));
	FloorMarkingMesh->SetupAttachment(GetRootComponent());
	SetOnlyOwnerSee(FloorMarkingMesh);
}

void ABaseFlyAiming::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsHidden())
	{
		FHitResult HitResult;
		TArray<AActor*> ActorsToIgnore;
		UKismetSystemLibrary::LineTraceSingle
		(
			this,
			GetActorLocation(),
			GetActorLocation() + FVector(0.0f, 0.0f, -100000.0f),
			ETraceTypeQuery::TraceTypeQuery1,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			HitResult,
			true
		);
		if (HitResult.GetActor())
		{
			SplineMesh->SetEndPosition((HitResult.ImpactPoint - GetActorLocation()).Length() * FVector::XAxisVector);
			FloorMarkingMesh->SetRelativeLocation(HitResult.ImpactPoint - GetActorLocation() + FVector(0.0f, 0.0f, 0.01f));
		}
	}
}

void ABaseFlyAiming::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseFlyAiming::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);
	SplineMesh->SetHiddenInGame(bNewHidden);
}
