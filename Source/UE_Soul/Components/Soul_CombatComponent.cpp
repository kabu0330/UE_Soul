// Fill out your copyright notice in the Description page of Project Settings.


#include "Soul_CombatComponent.h"

#include "UE_Soul/Character/Soul_Character.h"
#include "UE_Soul/Equip/Soul_Weapon.h"
#include "UE_Soul/Items/Soul_PickupItem.h"


USoul_CombatComponent::USoul_CombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void USoul_CombatComponent::BeginPlay()
{
	Super::BeginPlay();

}

void USoul_CombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void USoul_CombatComponent::SetWeapon(ASoul_Weapon* NewWeapon)
{
	// 기존에 가지고 있는 무기가 있으면 기존 무기는 새로 스폰해서 땅에 떨구고 새 무기를 장착한다.
	if (IsValid(MainWeapon))
	{
		if (ASoul_Character* Character = Cast<ASoul_Character>(GetOwner()))
		{
			ASoul_PickupItem* PickupItem = GetWorld()->SpawnActorDeferred<ASoul_PickupItem>(
				ASoul_PickupItem::StaticClass(), Character->GetActorTransform(),
				nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
			PickupItem->SetEquipmentClass(MainWeapon->GetClass());
			PickupItem->FinishSpawning(Character->GetActorTransform());

			MainWeapon->Destroy();
		}
	}
	MainWeapon = NewWeapon;
}

