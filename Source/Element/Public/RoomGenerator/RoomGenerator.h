#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomGenerator.generated.h"

UCLASS()
class ELEMENT_API ARoomGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	ARoomGenerator();

public:
	FIntVector GetRoomSize();
	FORCEINLINE float GetCellSize() { return CellSize; }
	FORCEINLINE void AddClearSpace(AActor* ClearSpace) { ClearSpaces.AddUnique(ClearSpace); }
	FORCEINLINE void RemoveClearSpace(AActor* ClearSpace) { ClearSpaces.Remove(ClearSpace); }
	FIntVector ToRoomCoordinate(FVector Location);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GenerateRoom();
	virtual void GenerateRoom_Implementation();

	void RegenerateRoom();

protected:
	FORCEINLINE void AddInstance(UInstancedStaticMeshComponent* InstancedStaticMesh, float X, float Y, float Z, FRotator Rotation = FRotator::ZeroRotator);
	FORCEINLINE bool IsClearSpace(int x, int y, int z);

	UPROPERTY(BlueprintReadWrite, Category = Components)
	UInstancedStaticMeshComponent* InstancedWallMeshComponent;

	UPROPERTY(BlueprintReadWrite, Category = Components)
	UInstancedStaticMeshComponent* InstancedFloorMeshComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attributes)
	UStaticMesh* WallMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attributes)
	UStaticMesh* FloorMesh;

	/*InstancedStaticMesh의 사이즈*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attributes)
	float CellSize = 200.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attributes)
	bool Floor = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Attributes)
	bool Ceil = false;

	UPROPERTY(EditInstanceOnly, Category = Attributes)
	TArray<AActor*> ClearSpaces;
};
