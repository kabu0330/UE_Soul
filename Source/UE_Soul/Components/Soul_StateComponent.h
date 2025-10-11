// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Soul_StateComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE_SOUL_API USoul_StateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USoul_StateComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
						   FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE void SetState(const FGameplayTag NewState) {CurrentState = NewState;}
	FORCEINLINE FGameplayTag GetCurrentState() const {return CurrentState;}
	void ClearState();

	bool IsCurrentStateEqualToAny(const FGameplayTagContainer& TagsToCheck) const;

	/** 이동 입력이 현재 활성화 상태인지? */
	UFUNCTION()
	FORCEINLINE bool MovementInputEnabled() const { return bMovementInputEnabled;}

	void ToggleMovementInput(bool bEnabled, float Duration = 0.f);
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void MovementInputEnableAction();
	
	/** 이동 키 입력 상태관리*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bMovementInputEnabled = true;

	/** 캐릭터의 현재 상태 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	FGameplayTag CurrentState;
	
};
