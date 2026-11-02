// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/QuestSystem/MR_LocationNotify.h"
#include "Components/TextBlock.h"

void UMR_LocationNotify::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMR_LocationNotify::NativeDestruct()
{
	Super::NativeDestruct();
}

void UMR_LocationNotify::SetLocationName(const FName& InLocationName) const
{
	if (Text_LocationName)
	{
		Text_LocationName->SetText(FText::FromName(InLocationName));
	}
}
