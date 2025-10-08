// Fill out your copyright notice in the Description page of Project Settings.


#include "Soul_AnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

USoul_AnimInstance::USoul_AnimInstance()
{
}

void USoul_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = CastChecked<ACharacter>(GetOwningActor());
	MovementComponent = Character->GetCharacterMovement();
}

void USoul_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!IsValid(Character)) return;
	if (!IsValid(MovementComponent)) return;

	Velocity = MovementComponent->Velocity;
	GroundSpeed = Velocity.Size2D();

	bShouldMove = GroundSpeed > 3.f && MovementComponent->GetCurrentAcceleration() != FVector::ZeroVector;
	
	bIsFalling = MovementComponent->IsFalling();
}
