#pragma once

#include "CoreMinimal.h"
#include "Magic/BaseMagic.h"
#include "Portal.generated.h"

class USceneCaptureComponent2D;
class UTextureRenderTarget2D;

UCLASS()
class ELEMENT_API APortal : public ABaseMagic
{
	GENERATED_BODY()
public:
	APortal();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE UTextureRenderTarget2D* GetPortalFrontRenderTarget() { return PortalFrontRenderTarget; }
	FORCEINLINE UTextureRenderTarget2D* GetPortalBackRenderTarget() { return PortalBackRenderTarget; }
	FORCEINLINE APortal* GetOutPortal() { return OutPortal; }
	
	UFUNCTION(BlueprintCallable)
	void ChangePortalColor();
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsActivated() { return Activated; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetOutPortal(APortal* Portal);

	void SetPortalRenderTarget();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ActivatePortal();
	virtual void ActivatePortal_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DeactivatePortal();
	virtual void DeactivatePortal_Implementation();

	UFUNCTION(BlueprintCallable)
	void TeleportActor(AActor* Actor);

protected:
	virtual void BeginBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void EndBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	virtual void InitActorsToIgnore() override;
	virtual void InitHitTraceObjectTypes() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PortalBackMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneCaptureComponent2D* PortalFrontSceneCapture;

	UPROPERTY(BlueprintReadWrite, Category = Attribute)
	UTextureRenderTarget2D* PortalFrontRenderTarget;

	UPROPERTY(BlueprintReadWrite, Category = Attribute)
	UMaterialInstanceDynamic* PortalFrontDynamicMaterialInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneCaptureComponent2D* PortalBackSceneCapture;

	UPROPERTY(BlueprintReadWrite, Category = Attribute)
	UTextureRenderTarget2D* PortalBackRenderTarget;

	UPROPERTY(BlueprintReadWrite, Category = Attribute)
	UMaterialInstanceDynamic* PortalBackDynamicMaterialInstance;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float PortalLifeTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attribute)
	APortal* OutPortal;

	bool Activated = true;
};