// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Soul_Character.generated.h"

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

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	/** 질주 */
	void Sprinting();
	void StopSprint();
	bool IsMoving() const;
	
	/** 질주 속도 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MovementData)
	float SprintingSpeed = 1000.f;

	/** 일반 속도 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MovementData)
	float NormalSpeed = 700.0f;

	/** 구르기 */
	void Rolling();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimMontage)
	UAnimMontage* RollingMontage;

	// Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoul_StateComponent> StateComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoul_AttributeComponent> AttributeComponent;

	// UI
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<USoul_PlayerOverlay> PlayerOverlayClass;

	UPROPERTY()
	TObjectPtr<USoul_PlayerOverlay> PlayerOverlay;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditDefaultsOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditDefaultsOnly, Category = "Input|MappingContext", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Action", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Action", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Action", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SprintRollingAction;
	
};
