#include "Magic/Portal.h"
#include "Components/BoxComponent.h"

#include "Element/DebugMacro.h"

APortal::APortal()
{
	HitBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitBoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
}

void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (OutPortal == nullptr) return;
	FHitResult HitResult;
	BoxTrace(HitResult);
	if (HitResult.GetActor())
	{
		OutPortal->AddActorsToIgnore(HitResult.GetActor());
		HitResult.GetActor()->SetActorLocation(OutPortal->GetActorLocation(), false, nullptr, ETeleportType::TeleportPhysics);
		ActorsToIgnore.Remove(HitResult.GetActor());
	}
}

void APortal::BeginPlay()
{
	Super::BeginPlay();
	
	HitBoxComponent->SetBoxExtent(FVector(10.0f, 50.0f, 100.0f));
}

void APortal::SetOutPortal(APortal* Portal)
{
	OutPortal = Portal;
	ActorsToIgnore.Add(Portal);
}

void APortal::SetPortalLifeTime(float Value)
{
	PortalLifeTime = Value;
	EndMagicAfter(PortalLifeTime);
}

void APortal::BeginBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*Super::BeginBoxOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (OutPortal == nullptr) return;
	FHitResult HitResult;
	BoxTrace(HitResult);
	if (HitResult.GetActor())
	{
		OutPortal->AddActorsToIgnore(HitResult.GetActor());
		HitResult.GetActor()->SetActorLocation(OutPortal->GetActorLocation(), false, nullptr, ETeleportType::TeleportPhysics);
		ActorsToIgnore.Remove(OtherActor);
	}*/
}

void APortal::EndBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::EndBoxOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	ActorsToIgnore.Remove(OtherActor);
	SCREEN_LOG(1, OtherActor->GetName());
}

void APortal::InitActorsToIgnore()
{
	ActorsToIgnore.Empty();
}

void APortal::InitBoxTraceObjectTypes()
{
	Super::InitBoxTraceObjectTypes();
	BoxTraceObjectTypes.Empty();
	BoxTraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
}
