// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotify_Soul_WeaponCollision.generated.h"

/**
 * 
 */
UCLASS()
class UE_SOUL_API UAnimNotify_Soul_WeaponCollision : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UAnimNotify_Soul_WeaponCollision();
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation,
		float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation,
		const FAnimNotifyEventReference& EventReference) override;
};
