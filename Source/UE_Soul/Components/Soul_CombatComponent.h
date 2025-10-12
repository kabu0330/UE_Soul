// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Soul_CombatComponent.generated.h"


class ASoul_Weapon;

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

protected:
	virtual void BeginPlay() override;

	/** 전투가 가능한 상태(무기를 꺼낸)인지? */
	UPROPERTY(EditAnywhere)
	bool bCombatEnabled = false;

	UPROPERTY()
	TObjectPtr<ASoul_Weapon> MainWeapon;


};
