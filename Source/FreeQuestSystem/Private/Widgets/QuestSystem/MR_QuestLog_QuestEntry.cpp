// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/QuestSystem/MR_QuestLog_QuestEntry.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/TextBlock.h"
#include "QuestSystem/MR_QuestBase.h"
#include "QuestSystem/Types/QuestTypes.h"

void UMR_QuestLog_QuestEntry::NativeConstruct()
{
	Super::NativeConstruct();
	
	const FQuestDetails* QuestDetails = QuestData.GetRow<FQuestDetails>(QuestID.ToString());
	
	Button_CheckBox->OnClicked.AddDynamic(this, &UMR_QuestLog_QuestEntry::HandleButtonClicked);
	Text_QuestName->SetText(QuestDetails->Name);
	IsQuestCompleted();
}

void UMR_QuestLog_QuestEntry::NativeDestruct()
{
	Super::NativeDestruct();
	Button_CheckBox->OnClicked.RemoveDynamic(this, &UMR_QuestLog_QuestEntry::HandleButtonClicked);
}

void UMR_QuestLog_QuestEntry::HandleButtonClicked()
{
	QuestSelected.Broadcast(QuestID, QuestActor);
}

void UMR_QuestLog_QuestEntry::IsQuestCompleted() const
{
	if (!IsValid(QuestActor) || IsValid(CheckBox_IsCompleted))
	if (QuestActor->GetIsCompleted())
	{
		CheckBox_IsCompleted->SetCheckedState(ECheckBoxState::Checked);
	}
	else
	{
		CheckBox_IsCompleted->SetCheckedState(ECheckBoxState::Unchecked);
	}
}

void UMR_QuestLog_QuestEntry::SetQuestID(const FName& InQuestID)
{
	QuestID = InQuestID;
}

void UMR_QuestLog_QuestEntry::SetQuestData(const FDataTableRowHandle& InQuestData)
{
	QuestData = InQuestData;
}

void UMR_QuestLog_QuestEntry::SetQuestActor(AMR_QuestBase* InQuestActor)
{
	QuestActor = InQuestActor;
}
