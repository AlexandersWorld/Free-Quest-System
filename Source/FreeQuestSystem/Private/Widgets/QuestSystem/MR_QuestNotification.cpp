// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/QuestSystem/MR_QuestNotification.h"
#include "Components/TextBlock.h"

void UMR_QuestNotification::NativeConstruct()
{
	Super::NativeConstruct();
	
	Text_ObjectiveDescription->SetText(ObjectiveDescription);
}

void UMR_QuestNotification::NativeDestruct()
{
	Super::NativeDestruct();
}

void UMR_QuestNotification::SetObjectiveDescription(const FText& InObjectiveDescription)
{
	ObjectiveDescription = InObjectiveDescription;

	if (Text_ObjectiveDescription)
	{
		Text_ObjectiveDescription->SetText(ObjectiveDescription);
	}
}

