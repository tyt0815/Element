#include "Magic/Explosion.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

#include "Element/DebugMacro.h"

void AExplosion::BeginBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SCREEN_LOG_NONE_KEY(TEXT("siba"));
	if (OtherActor->ActorHasTag(TEXT("Magic"))) return;
	Super::BeginBoxOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	HitBoxComponent->Deactivate();
	ProjectileMovementComponent->Deactivate();
	FHitResult HitResult;
	BoxTrace(HitResult);
	DamageActor(HitResult, GetOwnerATK() * DamageCoefficient, EFourElement::EPE_Ventus);
	AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
	FTimerHandle ExplosionTimer;
	GetWorldTimerManager().SetTimer(ExplosionTimer, this, &AExplosion::Explosion, ExplosionDelay);
}

void AExplosion::Explosion()
{
	TArray<AActor*> ExplosionIgnore;
	while(true)
	{
		FHitResult HitResult;
		UKismetSystemLibrary::SphereTraceSingleForObjects(
			this,
			GetActorLocation(),
			GetActorLocation(),
			ExplosionRadius,
			HitTraceObjectTypes,
			false,
			ExplosionIgnore,
			EDrawDebugTrace::None,
			HitResult,
			true
		);
		if (HitResult.GetActor() == nullptr) break;
		ExplosionIgnore.Add(HitResult.GetActor());
		PushLiftableActor(HitResult.GetActor(), GetActorForwardVector() * PushForce * 5);
		DamageActor(HitResult, GetOwnerATK() * ExplosionDamageCoefficient, EFourElement::EPE_Ignis);
	}
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionNiagara, GetActorLocation(), GetActorRotation());
	Destroy();
}
