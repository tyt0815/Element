#include "GameElements/Door.h"
#include "Components/SphereComponent.h"

#include "Helper/MathHelper.h"

ADoor::ADoor()
{
	DoorInnerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorInner"));
	DoorInnerMesh->SetupAttachment(GetRootComponent());
}

float ADoor::GetOpenDirection(AActor* OpeningActor) const
{
	float OpenDirection = 1.0f;
	if (OpeningActor)
	{
		FVector StartVector = InteractRangeComponent->GetComponentLocation();
		FVector A = GetActorForwardVector(), B = OpeningActor->GetActorLocation() - StartVector;
		double Degree = FMath::RadiansToDegrees(MathHelper::GetAngleBetweenTwoVectors(A, B));
		if (Degree < 90.0f)
		{
			OpenDirection *= -1.0f;
		}
	}

	return OpenDirection;
}
