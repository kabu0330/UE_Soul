// Fill out your copyright notice in the Description page of Project Settings.


#include "Soul_StateComponent.h"

#include "Kismet/KismetSystemLibrary.h"


USoul_StateComponent::USoul_StateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void USoul_StateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USoul_StateComponent::TickComponent(float DeltaTime, ELevelTick TickType,
										 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USoul_StateComponent::ToggleMovementInput(bool bEnabled, float Duration)
{
	if (bEnabled)
	{
		/** FLatentActionInfo는 특정 동작을 즉시 실행하지 않고 일정 시간 후나
		 * 특정 조건이 충족되었을 때 실행하도록 예약
		 * 실행 예약이 된 상태에서 다시 들어오면 예약된 동작을 취소하고 다시 동작을 예약
		 * 
		 */
		FLatentActionInfo LatentAction;
		LatentAction.CallbackTarget = this; // 콜백 함수를 호출할 객체
		LatentAction.ExecutionFunction = "MovementInputEnableAction"; // 리플렉션으로 처리하므로 반드시 UFUNCTION 필수
		LatentAction.Linkage = 0; // 여러 개의 LatentAction을 사용할 때 사용
		LatentAction.UUID = 0; // RetriggerableDelay 동일한 UUID가 있다면 취소하고 새로 시작

		UKismetSystemLibrary::RetriggerableDelay(GetWorld(), Duration, LatentAction);
	}
	else
	{
		bMovementInputEnabled = false;
	}
}

void USoul_StateComponent::MovementInputEnableAction()
{
	bMovementInputEnabled = true;
	ClearState();
}

void USoul_StateComponent::ClearState()
{
	CurrentState = FGameplayTag::EmptyTag;
}

bool USoul_StateComponent::IsCurrentStateEqualToAny(const FGameplayTagContainer& TagsToCheck) const
{
	return TagsToCheck.HasTagExact(CurrentState);
}



