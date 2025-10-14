// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Soul_Targeting.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USoul_Targeting : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UE_SOUL_API ISoul_Targeting
{
	GENERATED_BODY()

public:
	virtual void OnTargeted(bool bTarget) = 0;
	virtual bool CanBeTargeted() = 0;
};
