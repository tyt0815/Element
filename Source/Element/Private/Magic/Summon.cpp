#include "Magic/Summon.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Characters/BasePlayer.h"
#include "Magic/HomingBulletProjectile.h"

ASummon::ASummon()
{
	HitBoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
	HitBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HitBoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	HitBoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap);
	HitBoxComponent->SetSimulatePhysics(true);
	HitBoxComponent->SetMassOverrideInKg(NAME_None, 100.0f, true);
	HitBoxComponent->SetEnableGravity(false);
	DetectionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DetectionSphereComponent->SetupAttachment(GetRootComponent());

	StaticMeshComponent->SetMaterial(0, OffMaterial);
}

void ASummon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SetActorRotation(FMath::LerpStable(GetActorRotation(), GetOwnerRotation(), FollowingLerpAlpha));
	StaticMeshComponent->AddLocalRotation(TickRotator * DeltaTime * RotationSpeed);
	SetActorLocation(FMath::LerpStable(GetActorLocation(), GetWorldLocationRelatedWithOwner(), FollowingLerpAlpha));

	if (Target == nullptr)
	{
		DetectTarget();
	}
	else if(FVector::Distance(GetActorLocation(), Target->GetActorLocation()) > DetectionSphereComponent->GetScaledSphereRadius())
	{
		StaticMeshComponent->SetMaterial(0, OffMaterial);
		Target = nullptr;
	}
}

void ASummon::DetectTarget()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> SphereTraceObjectTypes;
	SphereTraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TArray<AActor*> Ignore;
	while (true)
	{
		FHitResult Hitresult;
		UKismetSystemLibrary::SphereTraceSingleForObjects
		(
			this,
			GetActorLocation(),
			GetActorLocation(),
			DetectionSphereComponent->GetScaledSphereRadius(),
			SphereTraceObjectTypes,
			false,
			Ignore,
			EDrawDebugTrace::None,
			Hitresult,
			true
		);
		if (Hitresult.GetActor() == nullptr) return;
		if (GetOwner() != Hitresult.GetActor())
		{
			Target = Hitresult.GetActor();
			StaticMeshComponent->SetMaterial(0, OnMaterial);
			return;
		}
		Ignore.Add(Hitresult.GetActor());
	}
}

void ASummon::BeginPlay()
{
	Super::BeginPlay();

	FRotator InitRotator;
	InitRotator.Yaw = FMath::RandRange(0, 360);
	InitRotator.Pitch = FMath::RandRange(0, 360);
	InitRotator.Roll = FMath::RandRange(0, 360);
	StaticMeshComponent->SetRelativeRotation(InitRotator);

	FVector RotationAxis;
	RotationAxis.X = FMath::FRand();
	RotationAxis.Y = FMath::FRand();
	RotationAxis.Z = FMath::FRand();
	RotationSpeed = FMath::RandRange(MinRotateSpeed, MaxRotateSpeed);
	FQuat RotationQuat = FQuat(RotationAxis, UKismetMathLibrary::GetPI() / 4);
	TickRotator = RotationQuat.Rotator();

	GetWorldTimerManager().SetTimer(AttackTimer, this, &ASummon::Attack, AttackDelay, true);
	SetLifeSpan(LifeTime);
}

FVector ASummon::GetWorldLocationRelatedWithOwner()
{
	if (GetOwner() == nullptr) return GetActorLocation();
	FVector Location = GetOwner()->GetActorLocation();
	Location += GetOwner()->GetActorForwardVector() * LocationRelatedWithOwner.X;
	Location += GetOwner()->GetActorRightVector() * LocationRelatedWithOwner.Y;
	Location += GetOwner()->GetActorUpVector() * LocationRelatedWithOwner.Z;
	if (FVector::Distance(GetActorLocation(), Location) <= AcceptanceRadius) return GetActorLocation();
	
	FVector ToThis = UKismetMathLibrary::GetDirectionUnitVector(Location, GetActorLocation());
	return Location + ToThis * AcceptanceRadius;
}

void ASummon::Attack()
{
	UWorld* World = GetWorld();
	if (World && Target)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		AHomingBulletProjectile* Bullet = World->SpawnActor<AHomingBulletProjectile>(BulletClass, GetActorLocation(), FRotator(90, 0, 0), SpawnParams);
		Bullet->SetHomingTarget(Target);
	}
}

