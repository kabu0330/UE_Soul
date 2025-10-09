// Fill out your copyright notice in the Description page of Project Settings.


#include "Soul_StatBarWidget.h"

#include "Components/ProgressBar.h"

USoul_StatBarWidget::USoul_StatBarWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

void USoul_StatBarWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (StatBar)
	{
		StatBar->SetFillColorAndOpacity(FillColorAndOpacity);
	}
}

void USoul_StatBarWidget::SetRatio(float Ratio) const
{
	if (StatBar)
	{
		StatBar->SetPercent(Ratio);
	}
}
