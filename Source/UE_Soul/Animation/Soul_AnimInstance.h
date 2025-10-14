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

	// 앞에 AnimNotify_ 를 명시하면 뒤의 노티파이 이름과 동일한 일반 노티파이 생성하면 해당 시점에 함수 콜백
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MovementData)
	float Direction;

};
