// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MovementData, meta = (AllowPrivateAccess = "true"))
	float SprintingSpeed = 1000.f;

	/** 일반 속도 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MovementData, meta = (AllowPrivateAccess = "true"))
	float NormalSpeed = 700.0f;

	/** 구르기 */
	void Rolling();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimMontage, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* RollingMontage;

	/** 무기 장착 */
	void Interact();

	/** 전투 상태 전환 */
	void ToggleCombat();

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

	UPROPERTY(EditDefaultsOnly, Category = "Input|Action", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ToggleCombatAction;
	
};
