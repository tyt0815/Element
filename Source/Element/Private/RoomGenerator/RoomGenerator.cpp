#include "RoomGenerator/RoomGenerator.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Element/DebugMacro.h"

ARoomGenerator::ARoomGenerator()
{
	PrimaryActorTick.bCanEverTick = false;
}

FIntVector ARoomGenerator::GetRoomSize()
{
	FIntVector Size = FIntVector(
		UKismetMathLibrary::FTrunc(GetActorScale3D().X),
		UKismetMathLibrary::FTrunc(GetActorScale3D().Y),
		UKismetMathLibrary::FTrunc(GetActorScale3D().Z)
	);
	return Size;
}

FIntVector ARoomGenerator::ToRoomCoordinate(FVector Location)
{
	Location -= GetActorLocation();
	Location /= CellSize;
	Location.X += 0.5f;
	Location.Y += 0.5f;
	FIntVector RoomCoordinate;
	RoomCoordinate.X = UKismetMathLibrary::FFloor(Location.X);
	RoomCoordinate.Y = UKismetMathLibrary::FFloor(Location.Y);
	RoomCoordinate.Z = UKismetMathLibrary::FFloor(Location.Z);
	return RoomCoordinate;
}

void ARoomGenerator::GenerateRoom_Implementation()
{
	const int X = UKismetMathLibrary::FTrunc(GetActorScale3D().X);
	const int Y = UKismetMathLibrary::FTrunc(GetActorScale3D().Y);
	const int Z = UKismetMathLibrary::FTrunc(GetActorScale3D().Z);

	if (InstancedFloorMeshComponent && InstancedFloorMeshComponent->GetStaticMesh())
	{
		InstancedFloorMeshComponent->SetRelativeLocation(FVector::ZeroVector);
		if (Floor)
		{
			for (float i = 1; i < X - 1; ++i)
			{
				for (float j = 1; j < Y - 1; ++j)
				{
					AddInstance(InstancedFloorMeshComponent, i, j, 0.0f, FRotator::ZeroRotator);
				}
			}
		}
		if (Ceil)
		{
			for (float i = 1; i < X - 1; ++i)
			{
				for (float j = 1; j < Y - 1; ++j)
				{
					AddInstance(InstancedFloorMeshComponent, i, j, Z, FRotator(0, 0, 180));
				}
			}
		}

	}

	// Add Wall
	if (InstancedWallMeshComponent && InstancedWallMeshComponent->GetStaticMesh())
	{
		InstancedWallMeshComponent->SetRelativeLocation(FVector::ZeroVector);
		for (float k = 0; k < Z; ++k)
		{
			for (float i = 0; i < X; ++i)
			{
				AddInstance(InstancedWallMeshComponent, i, 0, k);
				AddInstance(InstancedWallMeshComponent, i, Y - 1, k);
			}
			for (float j = 1; j < Y - 1; ++j)
			{
				AddInstance(InstancedWallMeshComponent, 0, j, k);
				AddInstance(InstancedWallMeshComponent, X - 1, j, k);
			}
		}
	}
}

void ARoomGenerator::RegenerateRoom()
{
	InstancedWallMeshComponent->ClearInstances();
	InstancedFloorMeshComponent->ClearInstances();
	GenerateRoom();
}

void ARoomGenerator::AddInstance(UInstancedStaticMeshComponent* InstancedStaticMesh, float X, float Y, float Z, FRotator Rotation)
{
	if (IsClearSpace(X, Y, Z)) return;
	FVector Location = FVector(X * CellSize, Y * CellSize, Z * CellSize) + GetActorLocation();
	FTransform Transform;
	Transform.SetLocation(Location);
	Transform.SetRotation(Rotation.Quaternion());
	Transform.SetScale3D(FVector::OneVector);
	InstancedStaticMesh->AddInstance(Transform, true);
}

bool ARoomGenerator::IsClearSpace(int X, int Y, int Z)
{
	for (const auto& ClearSpace : ClearSpaces)
	{
		if (ClearSpace)
		{
			FIntVector Location = ToRoomCoordinate(ClearSpace->GetActorLocation());
			if (Location.X == X && Location.Y == Y && Location.Z == Z) 
			{
				return true;
			}
		}
	}
	return false;
}
