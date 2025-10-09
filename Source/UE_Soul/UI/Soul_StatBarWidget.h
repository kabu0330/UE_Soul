// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Soul_StatBarWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class UE_SOUL_API USoul_StatBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	USoul_StatBarWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;
	void SetRatio(float Ratio) const;
protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UProgressBar> StatBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatBar")
	FLinearColor FillColorAndOpacity = FLinearColor::Red;
	
};
