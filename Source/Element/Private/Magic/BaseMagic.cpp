#include "Magic/BaseMagic.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

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

	InitActorsToIgnore();
	InitBoxTraceObjectTypes();

	SpawnedLocation = GetActorLocation();
	HitBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseMagic::BeginBoxOverlap);
	HitBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ABaseMagic::EndBoxOverlap);
	HitBoxComponent->IgnoreActorWhenMoving(GetOwner(), true);
	Tags.Add(TEXT("Magic"));

	BoxTraceStart->SetRelativeLocation(HitBoxComponent->GetRelativeLocation() + FVector(-HitBoxComponent->GetScaledBoxExtent().X, 0.0f, 0.0f));
	BoxTraceEnd->SetRelativeLocation(HitBoxComponent->GetRelativeLocation() + FVector(HitBoxComponent->GetScaledBoxExtent().X, 0.0f, 0.0f));
	BoxTraceHalfSize = FVector(0.0f, HitBoxComponent->GetScaledBoxExtent().Y, HitBoxComponent->GetScaledBoxExtent().Z);
	BoxTraceOrientation = GetActorRotation();

}

void ABaseMagic::InitActorsToIgnore()
{
	ActorsToIgnore.Empty();
	if (GetOwner() != nullptr)
	{
		ActorsToIgnore.Add(GetOwner());
	}
}

void ABaseMagic::AddActorsToIgnore(AActor* Actor)
{
	if (Actor)
	{
		ActorsToIgnore.Add(Actor);
	}
}

void ABaseMagic::RemoveActorsToIgnore(AActor* Actor)
{
	if (Actor)
	{
		ActorsToIgnore.Remove(Actor);
	}
}

void ABaseMagic::BoxTrace(FHitResult& HitResult)
{
	FVector Start = BoxTraceStart->GetComponentLocation();
	FVector End = BoxTraceEnd->GetComponentLocation();
	UKismetSystemLibrary::BoxTraceSingleForObjects(
		this,
		Start,
		End,
		BoxTraceHalfSize,
		BoxTraceOrientation,
		BoxTraceObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
	);
	if (HitResult.GetActor())
	{
		ActorsToIgnore.Add(HitResult.GetActor());
	}
}

void ABaseMagic::BeginBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Magic"))) return;
}

void ABaseMagic::EndBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ABaseMagic::DamageActor(FHitResult& HitResult)
{
	if (HitResult.GetActor())
	{
		IHitInterface* HitInterface = Cast<IHitInterface>(HitResult.GetActor());
		if (HitInterface)
		{
			HitInterface->Execute_GetHit(HitResult.GetActor(), HitResult.ImpactPoint, GetOwner());
		}
		UGameplayStatics::ApplyDamage(
			HitResult.GetActor(),
			Damage,
			GetInstigatorController(),
			this,
			UDamageType::StaticClass()
		);
	}
}

void ABaseMagic::EndMagicAfter(float Time)
{
	GetWorldTimerManager().ClearTimer(DestroyTimer);
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &ABaseMagic::EndMagic, Time);
}

void ABaseMagic::InitBoxTraceObjectTypes()
{
	BoxTraceObjectTypes.Empty();
	BoxTraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
}

void ABaseMagic::EndMagic()
{
	Destroy();
}
