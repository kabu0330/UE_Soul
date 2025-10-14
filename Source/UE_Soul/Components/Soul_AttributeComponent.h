// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UE_Soul/Data/Soul_Define.h"
#include "Soul_AttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegateOnAttributeChanged, ESoul_AttributeType, Type, float, Data);
DECLARE_MULTICAST_DELEGATE(FOnDeath);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE_SOUL_API USoul_AttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USoul_AttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	// 스태미나
	/** 스태미나 데이터 Getter */
	FORCEINLINE float GetBaseStamina() const {return BaseStamina; }
	FORCEINLINE float GetMaxStamina() const {return MaxStamina; }
	FORCEINLINE float GetStaminaRatio() const {return BaseStamina / MaxStamina; }

	bool CheckHasEnoughStamina(float StaminaCost) const;
	
	/** 스태미나 소비 */
	void DecreaseStamina(float StaminaCost);

	/** 스태미나 회복 */
	void ToggleStaminaRegeneration(bool bEnabled, float StartDelay = 2.f);
	void BroadcastAttributeChanged(ESoul_AttributeType Type) const;
	FDelegateOnAttributeChanged OnAttributeChanged;
	
	// 체력
	FORCEINLINE float GetBaseHealth() const {return BaseHealth;}
	FORCEINLINE float GetMaxHealth() const {return MaxHealth;}

	void TakeDamageAmount(float DamageAmount);
	
	FOnDeath OnDeath;
	
protected:
	virtual void BeginPlay() override;

	// 스태미나
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
	float BaseStamina = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
	float MaxStamina = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
	float StaminaRegenRate = 0.2f;
	
	FTimerHandle StaminaRegenTimer;

	// 체력
	UPROPERTY(EditAnywhere, Category = "Health")
	float BaseHealth = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100.0f;


private:
	void RegenStaminaHandler();

};
