#include "GameElements/Door.h"
#include "Components/SphereComponent.h"

#include "Helper/MathHelper.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootMesh"));
	SetRootComponent(RootMesh);
	DoorInnerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorInner"));
	DoorInnerMesh->SetupAttachment(GetRootComponent());
}

void ADoor::Open_Implementation()
{
	Opened = true;
}

void ADoor::Close_Implementation()
{
	Opened = false;
}

float ADoor::GetOpenDirection(AActor* OpeningActor) const
{
	float OpenDirection = 1.0f;
	if (OpeningActor)
	{
		FVector StartVector = GetActorLocation();
		FVector A = GetActorForwardVector(), B = OpeningActor->GetActorLocation() - StartVector;
		double Degree = FMath::RadiansToDegrees(MathHelper::GetAngleBetweenTwoVectors(A, B));
		if (Degree < 90.0f)
		{
			OpenDirection *= -1.0f;
		}
	}

	return OpenDirection;
}
