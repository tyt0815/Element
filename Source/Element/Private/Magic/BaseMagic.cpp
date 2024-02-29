#include "Magic/BaseMagic.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraShakeSourceActor.h"

#include "Interfaces/HitInterface.h"
#include "Characters/BaseCharacter.h"

ABaseMagic::ABaseMagic()
{
	PrimaryActorTick.bCanEverTick = true;

	// Components
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootSceneComponent);
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetGenerateOverlapEvents(false);
	HitBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	HitBoxComponent->SetupAttachment(GetRootComponent());
	HitBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HitBoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
	HitBoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	HitBoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	HitBoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	HitBoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	HitBoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);
	HitBoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
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
	HitBoxComponent->IgnoreActorWhenMoving(GetOwner(), true);
	Tags.Add(TEXT("Magic"));

	BoxTraceStart->SetRelativeLocation(HitBoxComponent->GetRelativeLocation() + FVector(-HitBoxComponent->GetScaledBoxExtent().X, 0.0f, 0.0f));
	BoxTraceEnd->SetRelativeLocation(HitBoxComponent->GetRelativeLocation() + FVector(HitBoxComponent->GetScaledBoxExtent().X, 0.0f, 0.0f));
	BoxTraceHalfSize = FVector(0.0f, HitBoxComponent->GetScaledBoxExtent().Y, HitBoxComponent->GetScaledBoxExtent().Z);

	HitBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseMagic::BeginBoxOverlap);
	HitBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ABaseMagic::EndBoxOverlap);

	Owner = Cast<ABaseCharacter>(GetOwner());
}

void ABaseMagic::InitActorsToIgnore()
{
	ActorsToIgnore.Empty();
	if (GetOwner() != nullptr)
	{
		ActorsToIgnore.Add(GetOwner());
	}
}

float ABaseMagic::GetOwnerATK()
{
	return Owner ? Owner->GetATK() : 0;
}

FVector ABaseMagic::GetOwnerLocation()
{
	return Owner ? Owner->GetActorLocation() : FVector();
}

FRotator ABaseMagic::GetOwnerRotation()
{
	return Owner ? Owner->GetActorRotation() : FRotator();
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
		GetActorRotation(),
		BoxTraceObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true
	);
	if (HitResult.GetActor())
	{
		ActorsToIgnore.Add(HitResult.GetActor());
	}
}

void ABaseMagic::BoxTrace(FHitResult& HitResult, TArray<AActor*>& Ignore)
{
	FVector Start = BoxTraceStart->GetComponentLocation();
	FVector End = BoxTraceEnd->GetComponentLocation();
	UKismetSystemLibrary::BoxTraceSingleForObjects(
		this,
		Start,
		End,
		BoxTraceHalfSize,
		GetActorRotation(),
		BoxTraceObjectTypes,
		false,
		Ignore,
		EDrawDebugTrace::None,
		HitResult,
		true
	);
	if (HitResult.GetActor())
	{
		Ignore.Add(HitResult.GetActor());
	}
}

void ABaseMagic::EndBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ABaseMagic::DamageActor(FHitResult& HitResult, float Damage, EFourElement Element)
{
	if (HitResult.GetActor())
	{
		IHitInterface* HitInterface = Cast<IHitInterface>(HitResult.GetActor());
		if (HitInterface)
		{
			HitInterface->Execute_GetHit(HitResult.GetActor(), HitResult.ImpactPoint, GetOwner(), Element);
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

void ABaseMagic::SetMultiStageHit(float Damage, float Delay, EFourElement Element)
{
	FTimerDelegate MultiStageHitDelegate;
	MultiStageHitDelegate.BindUFunction(this, FName("MultiStageHit"), Damage);
	MultiStageHit(Damage, Element);
	GetWorldTimerManager().SetTimer(MultiStageHitTimer, MultiStageHitDelegate, Delay, true, Delay);
}

void ABaseMagic::MultiStageHit(float Damage, EFourElement Element)
{
	InitActorsToIgnore();
	while (true)
	{
		FHitResult HitResult;
		BoxTrace(HitResult);
		if (HitResult.GetActor() == nullptr) break;
		DamageActor(HitResult, Damage, Element);
	}
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
