// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Soul_AttackFinished.generated.h"

/**
 * 
 */
UCLASS()
class UE_SOUL_API UAnimNotify_Soul_AttackFinished : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_Soul_AttackFinished();
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected:
	UPROPERTY(EditAnywhere)
	float ComboResetDelay = 1.f;
};
