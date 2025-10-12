// Fill out your copyright notice in the Description page of Project Settings.


#include "Soul_MontageActionData.h"

UAnimMontage* USoul_MontageActionData::GetMontageForTag(const FGameplayTag& GroupTag, const int32 Index)
{
	if (MontageGroups.Contains(GroupTag))
	{
		const FSoul_MontageGroup& MontageGroup = MontageGroups[GroupTag];
		if (MontageGroup.Animations.Num() > 0 && MontageGroup.Animations.Num() > Index)
		{
			return MontageGroup.Animations[Index];
		}
	}
	return nullptr;
}
