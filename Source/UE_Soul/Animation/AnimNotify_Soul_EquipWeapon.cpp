// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Soul_EquipWeapon.h"

#include "UE_Soul/Components/Soul_CombatComponent.h"
#include "UE_Soul/Data/Soul_GameplayTags.h"
#include "UE_Soul/Equip/Soul_Weapon.h"

UAnimNotify_Soul_EquipWeapon::UAnimNotify_Soul_EquipWeapon()
{
}

void UAnimNotify_Soul_EquipWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (const AActor* Owner = MeshComp->GetOwner()) // 여기서 null 체크 안하면 에디터에서 크래시가 발생한다.
	{
		if (USoul_CombatComponent* CombatComponent = Owner->GetComponentByClass<USoul_CombatComponent>())
		{
			if (ASoul_Weapon* MainWeapon = CombatComponent->GetMainWeapon())
			{
				bool bCombatEnabled = CombatComponent->IsCombatEnabled();
				FName WeaponSocketName;

				// 애님 노티파이에서 설정한 태그에 따라 무기 부착 소켓 위치 변경
				if (MontageActionTag == Soul_GameplayTag::Character_Action_Equip)
				{
					bCombatEnabled = true;
					WeaponSocketName = MainWeapon->GetEquipSocketName();
				}
				else if (MontageActionTag == Soul_GameplayTag::Character_Action_Unequip)
				{
					bCombatEnabled = false;
					WeaponSocketName = MainWeapon->GetUnequipSocketName();
				}

				CombatComponent->SetCombatEnabled(bCombatEnabled);
				MainWeapon->AttachToOwner(WeaponSocketName);
			}
		}
	}
}
