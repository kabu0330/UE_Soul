// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "Soul_Character.generated.h"

class USoul_CombatComponent;
class USoul_StateComponent;
class USoul_PlayerOverlay;
class USoul_AttributeComponent;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class UE_SOUL_API ASoul_Character : public ACharacter
{
	GENERATED_BODY()

public:
	ASoul_Character();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void NotifyControllerChanged() override;

	FORCEINLINE USoul_StateComponent* GetStateComponent() const {return StateComponent;}
	FORCEINLINE bool IsSprinting() const {return bSprinting;}

	// public Combo Section: AnimNotify
	void EnableComboWindow();
	void DisableComboWindow();
	void AttackFinished(const float ComboResetDelay);
	
protected:
	virtual void BeginPlay() override;
	
	// Components
	UPROPERTY(EditDefaultsOnly, Category = "Components|Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditDefaultsOnly, Category = "Components|Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	/** 스탯 관리 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<USoul_AttributeComponent> AttributeComponent;

	/** 상태 관리(이동 가능 여부 등) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<USoul_StateComponent> StateComponent;

	/** 무기 및 전투 관리 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<USoul_CombatComponent> CombatComponent;

	/** UI */ 
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<USoul_PlayerOverlay> PlayerOverlayClass;

	UPROPERTY()
	TObjectPtr<USoul_PlayerOverlay> PlayerOverlay;
	
private:
	// Actions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	/** 상태 체크 */
	bool IsMoving() const;
	bool CanToggleCombat() const;

	/** 질주 */
	void Sprinting();
	void StopSprint();
	
	/** 질주 속도 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementData|Sprint", meta = (AllowPrivateAccess = "true"))
	float SprintingSpeed = 1000.f;

	/** 일반 속도 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementData|Sprint", meta = (AllowPrivateAccess = "true"))
	float NormalSpeed = 700.0f;

	/** 질주 상태 체크 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MovementData|Sprint", meta = (AllowPrivateAccess = "true"))
	bool bSprinting = false;

	/** 구르기 */
	void Rolling();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimMontage, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* RollingMontage;

	/** 무기 장착 */
	void Interact();

	/** 전투 상태 전환 */
	void ToggleCombat();
	void AutoToggleCombat();

	// private Combo Section
	void Attack();
	void SpecialAttack();
	void HeavyAttack();

	/** 현재 상태에서 수행 가능한 일반 공격 */
	FGameplayTag GetAttackPerform() const;

	/** 공격 가능 조건 체크 */
	bool CanPerformAttack(const FGameplayTag& AttackTypeTag) const;

	/** 공격 실행 */
	void DoAttack(const FGameplayTag& AttackTypeTag);

	/** 콤보 실행 */
	void ExecuteComboAttack(const FGameplayTag& AttackTypeTag);

	/** 콤보 초기화 */
	void ResetCombo();
	
	/** 콤보 시퀀스 진행 중 */
	bool bComboSequenceRunning = false;

	/** 콤보 입력 가능 상태 체크 */
	bool bCanComboInput = false;

	/** 콤보 카운팅 */
	int32 ComboCounter = 0;

	/** 콤보 입력 여부 */
	bool bSavedComboInput = false;

	/** 콤보 리셋 타이머 핸들 */
	FTimerHandle ComboResetTimerHandle;

	// Enhanced Input
	UPROPERTY(EditDefaultsOnly, Category = "Input|MappingContext", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Action", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Action", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Action", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SprintRollingAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Action", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractAction;

	/** 전투 활성화, 비활성화 토글 */
	UPROPERTY(EditDefaultsOnly, Category = "Input|Action", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ToggleCombatAction;

	/** 일반 공격, 대시 공격, 특수 공격(차징) */
	UPROPERTY(EditDefaultsOnly, Category = "Input|Action", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AttackAction;

	/** 강공격 (Shift + 좌클릭) */
	UPROPERTY(EditDefaultsOnly, Category = "Input|Action", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> HeavyAttackAction;
	
};
