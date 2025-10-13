// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Soul_AttackFinished.h"

#include "UE_Soul/Character/Soul_Character.h"

UAnimNotify_Soul_AttackFinished::UAnimNotify_Soul_AttackFinished()
{
}

void UAnimNotify_Soul_AttackFinished::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ASoul_Character* Character = Cast<ASoul_Character>(MeshComp->GetOwner()))
	{
		Character->AttackFinished(ComboResetDelay);
	}
}
