#pragma once

UENUM(BlueprintType)
enum class EPlayerActionState : uint8
{
	EPAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EPAS_Casting UMETA(DisplayName = "Casting")
};
UENUM(BlueprintType)
enum class EPlayerCameraState : uint8
{
	EPCS_ZoomOut UMETA(DisplayName = "ZoomOut"),
	EPCS_ZoomIn UMETA(DisplayName = "ZoomIn")
};

UENUM(BlueprintType)
enum class ECastedMagic : uint8
{
	ECM_None UMETA(DisplayName = "None"),
	ECM_MagicBullet UMETA(DisplayName = "MagicBullet"),
	ECM_II UMETA(DisplayName = "Ignis + Ignis"),
	ECM_AA UMETA(DisplayName = "Aqua + Aqua"),
	ECM_VV UMETA(DisplayName = "Ventus + Ventus"),
	ECM_TT UMETA(DisplayName = "Terra + Terra"),
	ECM_IV UMETA(DisplayName = "Ignis + Ventus"),
	ECM_VA UMETA(DisplayName = "Ventus + Aqua"),
	ECM_AT UMETA(DisplayName = "Aqua + Terra"),
	ECM_TI UMETA(DisplayName = "Terra + Ignis")
};

UENUM(BlueprintType)
enum class EFourElement : uint8
{
	EPE_None = 0 UMETA(DisplayName = "None"),
	EPE_Ignis = 0x1 UMETA(DisplayName = "Ignis"),
	EPE_Aqua = 0x2 UMETA(DisplayName = "Aqua"),
	EPE_Ventus = 0x4 UMETA(DisplayName = "Ventus"),
	EPE_Terra = 0x8 UMETA(DisplayName = "Terra")
};