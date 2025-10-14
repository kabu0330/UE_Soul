// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Soul_WeaponCollisionComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitActor, const FHitResult&);

/** 충돌 처리 관련 컴포넌트
 *  무기는 델리게이트를 통해 충돌 대상을 받고, 충돌 검사 시점은 애님 노티파이에서 활성화한다.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), meta = (DisplayName = "WeaponCollision"))
class UE_SOUL_API USoul_WeaponCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USoul_WeaponCollisionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
						   FActorComponentTickFunction* ThisTickFunction) override;

	FOnHitActor OnHitActor;

protected:
	virtual void BeginPlay() override;

	/** 시작 소켓 */
	UPROPERTY(EditAnywhere)
	FName TraceStartSocketName = "WeaponStart";

	/** 끝 소켓 */
	UPROPERTY(EditAnywhere)
	FName TraceEndSocketName = "WeaponEnd";

	// FHitResult 파라미터
	/** 스피어 크기 */
	UPROPERTY(EditAnywhere)
	float TraceRadius = 20.f;

	/** 충돌 트레이스 대상 오브젝트 타입 */
	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	/** 충돌 처리 제외 액터 */
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<AActor>> IgnoreActors;

	/** 드로우 디버그 타입 */
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForDuration; 

	/** 무기의 메시 컴포넌트 */
	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> WeaponMesh;

	/** 프레임마다 충돌체크하므로 이전 프레임에 충돌 처리한 액터들 예외처리 */
	UPROPERTY()
	TArray<TObjectPtr<AActor>> AlreadyHitActors;

	/** 충돌 감지 트리거 */
	bool bIsCollisionEnabled = false;
	
public:
	/** 충돌 감지 트리거 함수 */
	void TurnOnCollision();
	
	void TurnOffCollision();
	void SetWeaponMesh(UPrimitiveComponent* MeshComponent);
	void AddIgnoredActor(AActor* Actor);
	void RemoveIgnoredActor(AActor* Actor);

protected:
	bool CanHitActor(AActor* Actor) const;
	void CollisionTrace();
};
