// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/QuestSystem/MR_HUDWidget.h"

#include "Widgets/QuestSystem/MR_InfoMessage.h"

void UMR_HUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UMR_HUDWidget::ShowInteractionMessage(const FName& Message) const
{
	if (IsValid(InfoMessage) && !Message.IsNone())
	{
		InfoMessage->SetMessage(Message);
	}
}

void UMR_HUDWidget::HideInteractionMessage() const
{
	InfoMessage->MessageHide();
}
