// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UE_Soul/Interfaces/Soul_Targeting.h"
#include "Soul_Enemy.generated.h"


class USphereComponent;
class USoul_StateComponent;
class USoul_AttributeComponent;
class UWidgetComponent;

UCLASS()
class UE_SOUL_API ASoul_Enemy : public ACharacter, public ISoul_Targeting
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USoul_AttributeComponent> AttributeComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USoul_StateComponent> StateComponent;

	/** 타게팅 충돌 체크용 */
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> TargetingSphereComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UWidgetComponent> LockOnWidgetComponent;

	// Effect Section
	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<USoundCue> ImpactSound;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<UParticleSystem> ImpactParticle;

	// Montage Section
	UPROPERTY(EditAnywhere, Category = "Montage|HitReact")
	TObjectPtr<UAnimMontage> HitReactAnimFront;
	
	UPROPERTY(EditAnywhere, Category = "Montage|HitReact")
	TObjectPtr<UAnimMontage> HitReactAnimBack;
	
	UPROPERTY(EditAnywhere, Category = "Montage|HitReact")
	TObjectPtr<UAnimMontage> HitReactAnimLeft;
	
	UPROPERTY(EditAnywhere, Category = "Montage|HitReact")
	TObjectPtr<UAnimMontage> HitReactAnimRight;
	
public:
	ASoul_Enemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void OnDeath();

	// ISoul_Targeting
	/** 타게팅 처리 함수 */
	virtual void OnTargeted(bool bTarget) override;
	/** 타게팅이 가능한 상태인지 체크하는 함수 */
	virtual bool CanBeTargeted() override;

protected:
	virtual void BeginPlay() override;

	void ImpactEffect(const FVector& Location);
	void HitReaction(const AActor* Attacker);
	UAnimMontage* GetHitReactAnimation(const AActor* Attacker) const;


};
