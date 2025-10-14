// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Soul_WeaponCollision.h"

#include "UE_Soul/Components/Soul_CombatComponent.h"
#include "UE_Soul/Components/Soul_WeaponCollisionComponent.h"
#include "UE_Soul/Equip/Soul_Weapon.h"

UAnimNotify_Soul_WeaponCollision::UAnimNotify_Soul_WeaponCollision()
{
}

void UAnimNotify_Soul_WeaponCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (const AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (const USoul_CombatComponent* CombatComponent = OwnerActor->GetComponentByClass<USoul_CombatComponent>())
		{
			const ASoul_Weapon* Weapon = CombatComponent->GetMainWeapon();
			if (IsValid(Weapon))
			{
				Weapon->GetCollision()->TurnOnCollision();
			}
		}
	}
}

void UAnimNotify_Soul_WeaponCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (const AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (const USoul_CombatComponent* CombatComponent = OwnerActor->GetComponentByClass<USoul_CombatComponent>())
		{
			const ASoul_Weapon* Weapon = CombatComponent->GetMainWeapon();
			if (IsValid(Weapon))
			{
				Weapon->GetCollision()->TurnOffCollision();
			}
		}
	}
}
