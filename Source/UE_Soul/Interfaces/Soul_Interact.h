// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Soul_Interact.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USoul_Interact : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UE_SOUL_API ISoul_Interact
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Interact(AActor* InteractActor) = 0;
};
