#pragma once

#include "CoreMinimal.h"

#define COLLISION_OBJECT_INTERACTION ECC_GameTraceChannel1

UENUM(BlueprintType)
enum class ESoul_AttributeType : uint8
{
	Stamina,
	Health,
};
