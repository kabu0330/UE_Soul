// Fill out your copyright notice in the Description page of Project Settings.


#include "Soul_Weapon.h"

#include "Kismet/GameplayStatics.h"
#include "UE_Soul/Components/Soul_CombatComponent.h"
#include "UE_Soul/Components/Soul_WeaponCollisionComponent.h"
#include "UE_Soul/Data/Soul_MontageActionData.h"
#include "UE_Soul/Data/Soul_GameplayTags.h"


ASoul_Weapon::ASoul_Weapon()
{
	PrimaryActorTick.bCanEverTick = true;

	StaminaCostMap.Add(Soul_GameplayTag::Character_Attack_Light, 7.f);
	StaminaCostMap.Add(Soul_GameplayTag::Character_Attack_Running, 12.f);
	StaminaCostMap.Add(Soul_GameplayTag::Character_Attack_Special, 15.f);
	StaminaCostMap.Add(Soul_GameplayTag::Character_Attack_Heavy, 20.f);

	WeaponCollision = CreateDefaultSubobject<USoul_WeaponCollisionComponent>("WeaponCollision");
	WeaponCollision->OnHitActor.AddUObject(this, &ThisClass::OnHitActor); // 델리게이트

	// 데미지 배수 적용
	DamageMultiplierMap.Add(Soul_GameplayTag::Character_Attack_Heavy, 1.8f);
	DamageMultiplierMap.Add(Soul_GameplayTag::Character_Attack_Running, 1.8f);
	DamageMultiplierMap.Add(Soul_GameplayTag::Character_Attack_Special, 2.1f);
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

		WeaponCollision->SetWeaponMesh(Mesh);
		// 무기를 소유한 대상자는 충돌에서 무시
		WeaponCollision->AddIgnoredActor(GetOwner());
	}
}

float ASoul_Weapon::GetAttackDamage() const
{
	if (const AActor* OwnerActor = GetOwner())
	{
		const FGameplayTag LastAttackType = CombatComponent->GetLastAttackType();

		if (DamageMultiplierMap.Contains(LastAttackType))
		{
			const float Multiplier = DamageMultiplierMap[LastAttackType];
			return BaseDamage * Multiplier;
		}
	}
	return BaseDamage;
}

void ASoul_Weapon::OnHitActor(const FHitResult& Hit)
{
	AActor* TargetActor = Hit.GetActor();
	const FVector DamageDirection = GetOwner()->GetActorForwardVector();

	const float AttackDamage = GetAttackDamage();

	// 액터의 한 지점(Point)에 피해를 주는 함수로, 피격 위치가 중요한 공격에 사용
	UGameplayStatics::ApplyPointDamage(
		TargetActor,
		AttackDamage,
		DamageDirection, // 피해가 적용된 방향, 피격 당한 액터가 넉백되거나 피격 효과를 출력할 때 유용
		Hit,
		GetOwner()->GetInstigatorController(), // 피해를 일으킨 컨트롤러, 어그로 관리나 점수 처리 등 판단
		this, // 피해를 일으킨 액터, 주로 무기나 발사체
		nullptr // 피해 유형을 정의하는 클래스, 화염 냉기 관통 등 속성 데미지
		);
}


