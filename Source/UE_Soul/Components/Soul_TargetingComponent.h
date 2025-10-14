// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UE_Soul/Data/Soul_Define.h"
#include "Soul_TargetingComponent.generated.h"


class UCameraComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE_SOUL_API USoul_TargetingComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	/** 타게팅 할 후보군을 감지할 영역의 크기 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TargetingRadius = 500.f;

	/** 타게팅 시 카메라 회전 속도 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FaceLockOnRotationSpeed = 20.f;
	
	UPROPERTY(Editanywhere)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForDuration;

	UPROPERTY()
	TObjectPtr<ACharacter> Character;

	UPROPERTY()
	TObjectPtr<UCameraComponent> Camera;

	/** 현재 락온이 된 대상 */
	UPROPERTY()
	TObjectPtr<AActor> LockedTargetActor;

	/** 락온 상태 관리 */
	UPROPERTY()
	bool bIsLockOn = false;
	
public:
	USoul_TargetingComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
						   FActorComponentTickFunction* ThisTickFunction) override;

	/** 락온 온오프 */
	void ToggleLockOn();

	/** 락온 대상 전환 */
	void SwitchingLockedOnActor(ESwitchingDirection InDirection);

	FORCEINLINE bool IsLockOn() const {return bIsLockOn;}

	/** 락온 시킬 후보군 탐색 */
	void FindTargets(OUT TArray<AActor*>& OutTargetingActors) const;

	/** 최종 락온 대상 탐색 */
	AActor* FindClosestTarget(TArray<AActor*>& InTargets, ESwitchingDirection InDirection = ESwitchingDirection::None);

	/** 캐릭터가 카메라 회전에 동기화되도록 설정 */
	void OrientCamera() const;

	/** 캐릭터가 이동 방향으로 회전하도록 설정 */
	void OrientMovement() const;

	/** 락온 상태에서 카메라 회전 제어 */
	void FaceLockOnActor() const;

	/** 락온 */
	void LockOnTarget();

	/** 락온 중지 */
	void StopLockOn();

protected:
	virtual void BeginPlay() override;



};
