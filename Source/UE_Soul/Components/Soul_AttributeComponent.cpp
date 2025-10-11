// Fill out your copyright notice in the Description page of Project Settings.


#include "Soul_AttributeComponent.h"


USoul_AttributeComponent::USoul_AttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void USoul_AttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void USoul_AttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType,
											 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool USoul_AttributeComponent::CheckHasEnoughStamina(float StaminaCost) const
{
	return BaseStamina >= StaminaCost;
}

void USoul_AttributeComponent::DecreaseStamina(float StaminaCost)
{
	BaseStamina = FMath::Clamp(BaseStamina - StaminaCost, 0.f, MaxStamina);
}

void USoul_AttributeComponent::ToggleStaminaRegeneration(bool bEnabled, float StartDelay)
{
	BroadcastAttributeChanged(ESoul_AttributeType::Stamina);
	
	if (bEnabled)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(StaminaRegenTimer) == false)
		{
			GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimer, this, &ThisClass::RegenStaminaHandler, 0.1f, true, StartDelay);
		}
	}
	else 
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimer);
	}
}

void USoul_AttributeComponent::BroadcastAttributeChanged(ESoul_AttributeType Type) const
{
	if (OnAttributeChanged.IsBound())
	{
		float Ratio = 0.f;
		switch (Type)
		{
		case ESoul_AttributeType::Stamina:
			Ratio = GetStaminaRatio();
			break;

		case ESoul_AttributeType::Health:
			break;

		default:
			break;
		}

		OnAttributeChanged.Broadcast(Type, Ratio);
	}
}
void USoul_AttributeComponent::RegenStaminaHandler()
{
	BaseStamina = FMath::Clamp(BaseStamina + StaminaRegenRate, 0.f, MaxStamina);

	// 스태미나 회복 중에는 계속 브로드캐스트해서 UI에 데이터를 업데이트
	BroadcastAttributeChanged(ESoul_AttributeType::Stamina);

	// 스태미나가 가득 차면 타이머 루프 종료
	if (BaseStamina >= MaxStamina)
	{
		ToggleStaminaRegeneration(false);
	}
}



