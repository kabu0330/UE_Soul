// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Soul_PlayerOverlay.generated.h"

class USoul_StatBarWidget;
/**
 * 
 */
UCLASS()
class UE_SOUL_API USoul_PlayerOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	USoul_PlayerOverlay(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<USoul_StatBarWidget> StaminaBarWidget;

	UFUNCTION()
	void OnAttributeChanged(ESoul_AttributeType AttributeType, float InValue);
};
