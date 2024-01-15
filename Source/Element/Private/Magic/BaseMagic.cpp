#include "Magic/BaseMagic.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Element/DebugMacro.h"
#include "Interfaces/HitInterface.h"

ABaseMagic::ABaseMagic()
{
	PrimaryActorTick.bCanEverTick = true;

	// Components
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetGenerateOverlapEvents(false);
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	NiagaraComponent->SetGenerateOverlapEvents(false);
	HitBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	HitBoxComponent->SetupAttachment(GetRootComponent());
	HitBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ArrowComponent->SetupAttachment(GetRootComponent());
	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceStart"));
	BoxTraceStart->SetupAttachment(GetRootComponent());
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceEnd"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void ABaseMagic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseMagic::BeginPlay()
{
	Super::BeginPlay();
	HitBoxComponent->IgnoreActorWhenMoving(GetOwner(), true);
	HitBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseMagic::OnBoxOverlap);
	Tags.Add(TEXT("Magic"));
}

void ABaseMagic::Activate_Implementation(FVector Location, FRotator Rotator, float Range)
{
	ActorsToIgnore.Empty();
	ActorsToIgnore.Add(GetOwner());
}

void ABaseMagic::Deactivate_Implementation()
{
	Destroy();
}

void ABaseMagic::BoxTrace(TArray<FHitResult>& HitResults)
{
	FVector Start = BoxTraceStart->GetComponentLocation();
	FVector End = BoxTraceEnd->GetComponentLocation();
	UKismetSystemLibrary::BoxTraceMulti(
		this,
		Start,
		End,
		BoxTraceHalfSize,
		BoxTraceOrientation,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResults,
		true
	);
	int n = HitResults.Num();
	for (int i = 0; i < n; ++i)
	{
		ActorsToIgnore.Add(HitResults[i].GetActor());
	}
}

void ABaseMagic::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Magic"))) return;

	TArray<FHitResult> HitResults;
	
	BoxTrace(HitResults);

	int n = HitResults.Num();
	SCREEN_LOG(2, FString::FromInt(n));
	for (int i = 0; i < n; ++i)
	{
		IHitInterface* HitInterface = Cast<IHitInterface>(HitResults[i].GetActor());
		if (HitInterface)
		{
			SCREEN_LOG(1, TEXT("Sibal"));
			HitInterface->Execute_GetHit(HitResults[i].GetActor(), HitResults[i].ImpactPoint, GetOwner());
		}
		else
		{
			SCREEN_LOG(1, HitResults[i].GetActor()->GetName());
		}
	}
}
