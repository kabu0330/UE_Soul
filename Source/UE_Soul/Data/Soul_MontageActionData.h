// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Soul_MontageActionData.generated.h"


USTRUCT(BlueprintType)
struct FSoul_MontageGroup
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UAnimMontage>> Animations;
};

/**
 * 
 */
UCLASS()
class UE_SOUL_API USoul_MontageActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UAnimMontage* GetMontageForTag(const FGameplayTag& GroupTag, const int32 Index);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="Montage Groups")
	TMap<FGameplayTag, FSoul_MontageGroup> MontageGroups;

	
};
