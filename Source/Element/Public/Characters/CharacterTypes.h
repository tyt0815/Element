#pragma once

UENUM(BlueprintType)
enum class EPlayerActionState : uint8
{
	EPAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EPAS_Casting UMETA(DisplayName = "Casting")
};

UENUM(BlueprintType)
enum class EPlayerCastedMagic : uint8
{
	EPCM_None UMETA(DisplayName = "None"),
	EPCM_MagicBullet UMETA(DisplayName = "MagicBullet")
};

UENUM(BlueprintType)
enum class EPlayerElement : uint8

{
	EPE_None = 0 UMETA(DisplayName = "None"),
	EPE_Ignis = 0x1 UMETA(DisplayName = "Ignis"),
	EPE_Aqua = 0x2 UMETA(DisplayName = "Aqua"),
	EPE_Ventus = 0x4 UMETA(DisplayName = "Ventus"),
	EPE_Terra = 0x8 UMETA(DisplayName = "Terra")
};