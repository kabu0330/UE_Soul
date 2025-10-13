// Fill out your copyright notice in the Description page of Project Settings.


#include "Soul_Weapon.h"

#include "UE_Soul/Components/Soul_CombatComponent.h"
#include "UE_Soul/Data/Soul_MontageActionData.h"
#include "UE_Soul/Data/Soul_GameplayTags.h"


ASoul_Weapon::ASoul_Weapon()
{
	PrimaryActorTick.bCanEverTick = true;

	StaminaCostMap.Add(Soul_GameplayTag::Character_Attack_Light, 7.f);
	StaminaCostMap.Add(Soul_GameplayTag::Character_Attack_Running, 12.f);
	StaminaCostMap.Add(Soul_GameplayTag::Character_Attack_Special, 15.f);
	StaminaCostMap.Add(Soul_GameplayTag::Character_Attack_Heavy, 20.f);
}

float ASoul_Weapon::GetStaminaCost(const FGameplayTag& InTag) const
{
	if (StaminaCostMap.Contains(InTag))
	{
		return StaminaCostMap[InTag];
	}
	return 0.f;
}

void ASoul_Weapon::BeginPlay()
{
	Super::BeginPlay();
}

UAnimMontage* ASoul_Weapon::GetMontageForTag(const FGameplayTag& Tag, const int32 Index) const
{
	return MontageActionData->GetMontageForTag(Tag, Index);
}

void ASoul_Weapon::EquipItem()
{
	Super::EquipItem();

	CombatComponent = GetOwner()->GetComponentByClass<USoul_CombatComponent>();
	if (CombatComponent)
	{
		CombatComponent->SetWeapon(this);

		// 전투 상태 여부에 따라 소켓을 달리 적용
		const FName AttachSocket = CombatComponent->IsCombatEnabled() ? EquipSocketName : UnequipSocketName; 
		AttachToOwner(AttachSocket);
	}
}


