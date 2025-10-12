// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Soul_Equipment.h"
#include "Soul_Weapon.generated.h"

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

protected:
	virtual void BeginPlay() override;
	virtual void EquipItem() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Equipment|Socekt")
	FName EquipSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Equipment|Socekt")
	FName UnequipSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment|Animation")
	TObjectPtr<USoul_MontageActionData> MontageActionData;

	UPROPERTY()
	TObjectPtr<USoul_CombatComponent> CombatComponent;

private:

};
