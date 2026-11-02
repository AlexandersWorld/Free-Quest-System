// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/QuestSystem/MR_InfoMessage.h"
#include "Components/TextBlock.h"

void UMR_InfoMessage::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	Text_Message->SetText(FText::GetEmpty());
	MessageHide();
}

void UMR_InfoMessage::SetMessage(const FName& Message)
{
	if (Message.IsNone())
	{
		MessageHide();
		bIsMessageActive = false;
		return;
	}
	Text_Message->SetText(FText::FromName(Message));
	
	if (!bIsMessageActive)
	{
		MessageShow();
	}
	
	bIsMessageActive = true;
	
	GetWorld()->GetTimerManager().SetTimer(MessageTimer, [this]()
	{
		MessageHide();
		bIsMessageActive = false;
	}, MessageLifeTime, false);
}

void UMR_InfoMessage::MessageShow()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UMR_InfoMessage::MessageHide()
{
	SetVisibility(ESlateVisibility::Collapsed);
}