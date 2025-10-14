// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Soul_CombatComponent.generated.h"


class ASoul_Weapon;

/** 캐릭터의 무기 장착과 관련된 기능을 처리하는 컴포넌트
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE_SOUL_API USoul_CombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USoul_CombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
						   FActorComponentTickFunction* ThisTickFunction) override;

	void SetWeapon(ASoul_Weapon* NewWeapon);

	FORCEINLINE bool IsCombatEnabled() const { return bCombatEnabled; }
	FORCEINLINE void SetCombatEnabled(const bool bEnabled) { bCombatEnabled = bEnabled; }
	FORCEINLINE ASoul_Weapon* GetMainWeapon() const { return MainWeapon; }

	FORCEINLINE FGameplayTag GetLastAttackType() const {return LastAttackType;}
	FORCEINLINE void SetLastAttackType(const FGameplayTag& NewAttackTypeTag) {LastAttackType = NewAttackTypeTag;}

protected:
	virtual void BeginPlay() override;

	/** 전투가 가능한 상태(무기를 꺼낸)인지?
	 *  애님 몽타주의 노티파이에서 상태를 변경시키고 있다.
	 */
	UPROPERTY(EditAnywhere)
	bool bCombatEnabled = false;

	UPROPERTY()
	TObjectPtr<ASoul_Weapon> MainWeapon;

	/** 마지막 공격 타입 : 현재 진행 중인 공격 */
	UPROPERTY(VisibleAnywhere)
	FGameplayTag LastAttackType;


};
