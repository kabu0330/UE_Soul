// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Soul_AnimInstance.generated.h"

class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class UE_SOUL_API USoul_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	USoul_AnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION()
	void AnimNotify_ResetMovementInput();

	UFUNCTION()
	void AnimNotify_ResetState();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OwingActor)
	TObjectPtr<ACharacter> Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OwingActor)
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MovementData)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MovementData)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MovementData)
	bool bShouldMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MovementData)
	bool bIsFalling;

};
