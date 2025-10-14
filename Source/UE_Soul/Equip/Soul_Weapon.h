// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Soul_Equipment.h"
#include "Soul_Weapon.generated.h"

class USoul_WeaponCollisionComponent;
class USoul_MontageActionData;
class USoul_CombatComponent;

UCLASS()
class UE_SOUL_API ASoul_Weapon : public ASoul_Equipment
{
	GENERATED_BODY()

public:
	ASoul_Weapon();

	UAnimMontage* GetMontageForTag(const FGameplayTag& Tag, const int32 Index = 0) const;
	FORCEINLINE FName GetEquipSocketName() const {return EquipSocketName;}
	FORCEINLINE FName GetUnequipSocketName() const {return UnequipSocketName;}

	float GetStaminaCost(const FGameplayTag& InTag) const;

protected:
	virtual void BeginPlay() override;
	virtual void EquipItem() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Equipment|Socekt")
	FName EquipSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Equipment|Socekt")
	FName UnequipSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment|Animation")
	TObjectPtr<USoul_MontageActionData> MontageActionData;

	/** 캐릭터의 무기 장착과 관련된 기능을 처리하는 컴포넌트 */
	UPROPERTY()
	TObjectPtr<USoul_CombatComponent> CombatComponent;

	/** 각 무기를 장착 후 동작에 따른 스태미나 코스트 */
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, float> StaminaCostMap;

	/** 충돌 감지 컴포넌트 */
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoul_WeaponCollisionComponent> WeaponCollision;

	/** 기본 데미지 */
	UPROPERTY(EditAnywhere)
	float BaseDamage = 15.f;

	/** 데미지 승수 */
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, float> DamageMultiplierMap;

public: // 콜리전 관련
	float GetAttackDamage() const;
	FORCEINLINE USoul_WeaponCollisionComponent* GetCollision() const { return WeaponCollision;}
	void OnHitActor(const FHitResult& Hit);

};
