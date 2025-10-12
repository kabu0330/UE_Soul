// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Soul_EquipWeapon.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Equip Weapon"))
class UE_SOUL_API UAnimNotify_Soul_EquipWeapon : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_Soul_EquipWeapon();
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag MontageActionTag;
};
