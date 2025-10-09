// Fill out your copyright notice in the Description page of Project Settings.


#include "Soul_PlayerOverlay.h"

#include "Soul_StatBarWidget.h"
#include "UE_Soul/Components/Soul_AttributeComponent.h"
#include "UE_Soul/Data/Soul_Define.h"

USoul_PlayerOverlay::USoul_PlayerOverlay(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void USoul_PlayerOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	if (APawn* OwingPawn = GetOwningPlayerPawn())
	{
		if (USoul_AttributeComponent* AttributeComponent = OwingPawn->FindComponentByClass<USoul_AttributeComponent>())
		{
			AttributeComponent->OnAttributeChanged.AddDynamic(this, &USoul_PlayerOverlay::OnAttributeChanged);
			AttributeComponent->BroadcastAttributeChanged(ESoul_AttributeType::Stamina);
		}
	}
}

void USoul_PlayerOverlay::OnAttributeChanged(ESoul_AttributeType AttributeType, float InValue)
{
	switch (AttributeType)
	{
	case ESoul_AttributeType::Stamina:
		StaminaBarWidget->SetRatio(InValue);
		break;
	case ESoul_AttributeType::Health:
		break;
	}
}
