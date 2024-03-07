#include "Magic/Portal.h"
#include "Components/BoxComponent.h"

#include "Element/DebugMacro.h"

APortal::APortal()
{
	HitBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HitBoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	HitBoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
}

void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OutPortal == nullptr) return;
	while (true)
	{
		FHitResult HitResult;
		BoxTrace(HitResult);
		if (!HitResult.GetActor()) break;
		TeleportActor(HitResult.GetActor());
	}
	
}

void APortal::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(PortalLifeTime);
}

void APortal::SetOutPortal(APortal* Portal)
{
	OutPortal = Portal;
	ActorsToIgnore.Add(Portal);
}

void APortal::TeleportActor(AActor* Actor)
{
	OutPortal->AddActorsToIgnore(Actor);
	Actor->SetActorLocation(OutPortal->GetActorLocation(), false, nullptr, ETeleportType::TeleportPhysics);
	ActorsToIgnore.Remove(Actor);
}

void APortal::EndBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::EndBoxOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	ActorsToIgnore.Remove(OtherActor);
}

void APortal::InitActorsToIgnore()
{
	ActorsToIgnore.Empty();
}

void APortal::InitHitTraceObjectTypes()
{
	Super::InitHitTraceObjectTypes();
	HitTraceObjectTypes.Empty();
	HitTraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
}
