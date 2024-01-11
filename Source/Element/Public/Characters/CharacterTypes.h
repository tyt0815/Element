#pragma once

UENUM(BlueprintType)
enum class EPlayerActionState
{
	EPAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EPAS_Casting UMETA(DisplayName = "Casting")
};

enum class EPlayerCastedMagic
{
	EPCM_None UMETA(DisplayName = "None"),
	EPCM_MagicBullet UMETA(DisplayName = "MagicBullet")
};