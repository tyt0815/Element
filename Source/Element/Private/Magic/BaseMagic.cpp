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
#include "GameElements/BaseLiftableActor.h"

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
}

void ABaseMagic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseMagic::BeginPlay()
{
	Super::BeginPlay();

	InitActorsToIgnore();
	InitHitTraceObjectTypes();

	SpawnedLocation = GetActorLocation();
	HitBoxComponent->IgnoreActorWhenMoving(GetOwner(), true);
	Tags.Add(TEXT("Magic"));

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

FVector ABaseMagic::GetHitBoxTraceBackLocation()
{
	return HitBoxComponent->GetComponentLocation() - GetActorForwardVector() * HitBoxComponent->GetScaledBoxExtent().X;
}

FVector ABaseMagic::GetHitBoxTraceFrontLocation()
{
	return HitBoxComponent->GetComponentLocation() + GetActorForwardVector() * HitBoxComponent->GetScaledBoxExtent().X;
}

FVector ABaseMagic::GetHitBoxTraceLeftLocation()
{
	return HitBoxComponent->GetComponentLocation() - GetActorRightVector() * HitBoxComponent->GetScaledBoxExtent().Y;
}

FVector ABaseMagic::GetHitBoxTraceRightLocation()
{
	return HitBoxComponent->GetComponentLocation() + GetActorRightVector() * HitBoxComponent->GetScaledBoxExtent().Y;
}

FVector ABaseMagic::GetHitBoxTraceDownLocation()
{
	return HitBoxComponent->GetComponentLocation() - GetActorUpVector() * HitBoxComponent->GetScaledBoxExtent().Z;
}

FVector ABaseMagic::GetHitBoxTraceUpLocation()
{
	return HitBoxComponent->GetComponentLocation() + GetActorUpVector() * HitBoxComponent->GetScaledBoxExtent().Z;
}

FVector ABaseMagic::GetHitBoxTraceBackToFrontHalfSize()
{
	return FVector(0.0f, HitBoxComponent->GetScaledBoxExtent().Y, HitBoxComponent->GetScaledBoxExtent().Z);
}

FVector ABaseMagic::GetHitBoxTraceLeftToRightHalfSize()
{
	return FVector(HitBoxComponent->GetScaledBoxExtent().X, 0.0f, HitBoxComponent->GetScaledBoxExtent().Z);
}

FVector ABaseMagic::GetHitBoxTraceDownToUpHalfSize()
{
	return FVector(HitBoxComponent->GetScaledBoxExtent().X, HitBoxComponent->GetScaledBoxExtent().Y, 0.0f);
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
	UKismetSystemLibrary::BoxTraceSingleForObjects(
		this,
		GetHitBoxTraceBackLocation(),
		GetHitBoxTraceFrontLocation(),
		GetHitBoxTraceBackToFrontHalfSize(),
		GetActorRotation(),
		HitTraceObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
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
	UKismetSystemLibrary::BoxTraceSingleForObjects(
		this,
		GetHitBoxTraceBackLocation(),
		GetHitBoxTraceFrontLocation(),
		GetHitBoxTraceBackToFrontHalfSize(),
		GetActorRotation(),
		HitTraceObjectTypes,
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

void ABaseMagic::BoxTraceMulti(TArray<FHitResult>& HitResults)
{
	BoxTraceMulti(HitResults, ActorsToIgnore);
}

void ABaseMagic::BoxTraceMulti(TArray<FHitResult>& HitResults, TArray<AActor*>& Ignore)
{
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		this,
		GetHitBoxTraceBackLocation(),
		GetHitBoxTraceFrontLocation(),
		GetHitBoxTraceBackToFrontHalfSize(),
		GetActorRotation(),
		HitTraceObjectTypes,
		false,
		Ignore,
		EDrawDebugTrace::None,
		HitResults,
		true
	);
	for (auto& HitResult : HitResults)
	{
		Ignore.AddUnique(HitResult.GetActor());
	}
}

void ABaseMagic::EndBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ABaseMagic::DamageActor(FHitResult& HitResult, float Damage, EFourElement Element)
{
	if (HitResult.GetActor())
	{
		if (HitResult.GetActor()->Implements<UHitInterface>())
		{
			IHitInterface::Execute_GetHit(HitResult.GetActor(), HitResult.ImpactPoint, GetOwner(), Element);
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

void ABaseMagic::DamageActorMulti(TArray<FHitResult>& HitResults, float Damage, EFourElement Element)
{
	TArray<AActor*> HitedActors;
	for (auto& HitResult : HitResults)
	{
		if (HitedActors.Find(HitResult.GetActor()) == INDEX_NONE)
		{
			HitedActors.AddUnique(HitResult.GetActor());
			DamageActor(HitResult, Damage, Element);
		}
	}
}

void ABaseMagic::SetMultiStageHit(float Damage, float Delay, EFourElement Element)
{
	SCREEN_LOG(1, FString::FromInt(Element));
	FTimerDelegate MultiStageHitDelegate;
	MultiStageHitDelegate.BindUFunction(this, FName("MultiStageHit"), Damage, Element);
	GetWorldTimerManager().SetTimer(MultiStageHitTimer, MultiStageHitDelegate, Delay, true, 0);
}

void ABaseMagic::PushLiftableActor(AActor* Actor, FVector Force)
{
	ABaseLiftableActor* LiftableActor = Cast<ABaseLiftableActor>(Actor);
	if (LiftableActor && !LiftableActor->IsLifted())
	{
		LiftableActor->GetMesh()->AddForce(Force * 10000000.0f);
	}
}

void ABaseMagic::MultiStageHit_v2(float Damage, EFourElement Element, float Delay)
{
	TArray<FHitResult> HitResults;
	BoxTraceMulti(HitResults);
	DamageActorMulti(HitResults, Damage, Element);

	FTimerHandle RemoveTimer;
	FTimerDelegate RemoveDelegate;
	RemoveDelegate.BindUFunction(this, FName("RemoveFromActorsToIgnore"), HitResults);
	GetWorldTimerManager().SetTimer(RemoveTimer, RemoveDelegate, Delay, false);
}

void ABaseMagic::RemoveFromActorsToIgnore(TArray<FHitResult> HitResults)
{
	for (auto& HitResult : HitResults)
	{
		ActorsToIgnore.Remove(HitResult.GetActor());
	}
}

void ABaseMagic::MultiStageHit(float Damage, EFourElement Element)
{
	SCREEN_LOG(2, FString::FromInt(Element));
	InitActorsToIgnore();
	while (true)
	{
		FHitResult HitResult;
		BoxTrace(HitResult);
		if (HitResult.GetActor() == nullptr) break;
		DamageActor(HitResult, Damage, Element);
	}
}

void ABaseMagic::InitHitTraceObjectTypes()
{
	HitTraceObjectTypes.Empty();
	HitTraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	HitTraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3));
}

void ABaseMagic::EndMagic()
{
	Destroy();
}
