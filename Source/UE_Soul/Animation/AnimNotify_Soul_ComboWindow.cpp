// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Soul_ComboWindow.h"

#include "UE_Soul/Character/Soul_Character.h"

UAnimNotify_Soul_ComboWindow::UAnimNotify_Soul_ComboWindow()
{
}

void UAnimNotify_Soul_ComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (ASoul_Character* Character = Cast<ASoul_Character>(MeshComp->GetOwner()))
	{
		Character->EnableComboWindow();
	}
}

void UAnimNotify_Soul_ComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (ASoul_Character* Character = Cast<ASoul_Character>(MeshComp->GetOwner()))
	{
		Character->DisableComboWindow();
	}
}
