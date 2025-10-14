#pragma once

#include "CoreMinimal.h"

#define COLLISION_OBJECT_INTERACTION ECC_GameTraceChannel1
#define COLLISION_OBJECT_TARGETING ECC_GameTraceChannel2

UENUM(BlueprintType)
enum class ESoul_AttributeType : uint8
{
	Stamina,
	Health,
};

UENUM(BlueprintType)
enum class EHitDirection : uint8
{
	Front,
	Left,
	Right,
	Back
};

UENUM(BlueprintType)
enum class ESwitchingDirection : uint8
{
	None,
	Left,
	Right
};