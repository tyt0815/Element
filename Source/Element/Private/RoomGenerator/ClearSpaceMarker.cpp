#include "RoomGenerator/ClearSpaceMarker.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

#include "RoomGenerator/RoomGenerator.h"
#include "Element/DebugMacro.h"

AClearSpaceMarker::AClearSpaceMarker()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AClearSpaceMarker::CheckClearSpace()
{
	TArray<AActor*> RoomGenerators;
	UGameplayStatics::GetAllActorsOfClass(this, ARoomGenerator::StaticClass(), RoomGenerators);

	for (auto& RoomActor : RoomGenerators)
	{
		ARoomGenerator* RoomGenerator = Cast<ARoomGenerator>(RoomActor);
		if (!RoomGenerator) continue;
		FIntVector RoomCoordinate = RoomGenerator->ToRoomCoordinate(GetActorLocation());
		FIntVector RoomSize = RoomGenerator->GetRoomSize();
		if (RoomCoordinate.X >= 0 && RoomCoordinate.X < RoomSize.X && RoomCoordinate.Y >= 0 && RoomCoordinate.Y < RoomSize.Y &&
			RoomCoordinate.Z >= 0 && RoomCoordinate.Z < RoomSize.Z)
		{
			RoomGenerator->AddClearSpace(this);
		}
		else
		{
			RoomGenerator->RemoveClearSpace(this);
		}
		RoomGenerator->RegenerateRoom();
	}
}
