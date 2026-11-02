// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/QuestSystem/MR_QuestLog.h"
#include "Widgets/QuestSystem/MR_QuestLog_QuestEntry.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/VerticalBox.h"
#include "QuestSystem/MR_QuestBase.h"
#include "QuestSystem//Types/QuestTypes.h"
#include "Widgets/QuestSystem/MR_QuestLogEntry_Objective.h"

void UMR_QuestLog::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		const FInputModeGameAndUI InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = true;
	}
	Button_Close->OnClicked.AddDynamic(this, &UMR_QuestLog::CloseQuestLog);
}

void UMR_QuestLog::NativeDestruct()
{
	Super::NativeDestruct();
	
	Button_Close->OnClicked.RemoveDynamic(this, &UMR_QuestLog::CloseQuestLog);
	
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
	}
}

void UMR_QuestLog::RefreshQuestLog() const
{
	Scroll_MainQuest->ClearChildren();
	Scroll_SideQuest->ClearChildren();
	Switcher->SetActiveWidgetIndex(SwitchInvisible);
}

void UMR_QuestLog::PopulateQuestList(const TArray<AMR_QuestBase*>& InQuestActors)
{
	RefreshQuestLog();

	for (AMR_QuestBase* CurrentQuestActor : InQuestActors)
	{
		if (!IsValid(CurrentQuestActor)) continue;

		FDataTableRowHandle QuestHandle = CurrentQuestActor->GetQuestData();
		if (QuestHandle.IsNull() || !QuestHandle.DataTable) continue;

		FQuestDetails* QuestDetails = QuestHandle.GetRow<FQuestDetails>(CurrentQuestActor->GetQuestID().ToString());
		if (!QuestDetails) continue;

		UMR_QuestLog_QuestEntry* QuestLog_Entry = CreateWidget<UMR_QuestLog_QuestEntry>(this, QuestEntryWidgetClass);
		if (IsValid(QuestLog_Entry))
		{
			QuestLog_Entry->SetQuestData(QuestHandle);
			QuestLog_Entry->SetQuestID(CurrentQuestActor->GetQuestID());
			QuestLog_Entry->SetQuestActor(CurrentQuestActor);
			QuestLog_Entry->QuestSelected.AddUObject(this, &UMR_QuestLog::OnQuestSelected);
            
			if (QuestDetails->bMain)
			{
				Scroll_MainQuest->AddChild(QuestLog_Entry);
			}
			else
			{
				Scroll_SideQuest->AddChild(QuestLog_Entry);
			}
		}
	}
}

void UMR_QuestLog::UpdateQuestLog(AMR_QuestBase* InQuestActor)
{
	if (!IsValid(InQuestActor)) return;

	if (CurrentDisplayedQuest != InQuestActor) return;

	DisplayQuest(InQuestActor->GetQuestID(), InQuestActor);
}

void UMR_QuestLog::OnQuestSelected(const FName& InQuestID, AMR_QuestBase* QuestActor)
{
	DisplayQuest(InQuestID, QuestActor);
}

void UMR_QuestLog::DisplayQuest(const FName& InQuestID, AMR_QuestBase* QuestActor)
{
	FDataTableRowHandle QuestHandle = QuestActor->GetQuestData();
	if (QuestHandle.IsNull() || !QuestHandle.DataTable) return;
	
	const FQuestDetails* QuestDetails = QuestHandle.GetRow<FQuestDetails>(InQuestID.ToString());
	if (!QuestDetails || !IsValid(QuestActor)) return;

	Switcher->SetActiveWidgetIndex(SwitchVisible);
	QuestLog_VerticalBox_Objectives->ClearChildren();
	Text_QuestLogTitle->SetText(QuestDetails->Name);
	Text_QuestDescription->SetText(QuestDetails->LogDescription);
	CurrentDisplayedQuest = QuestActor;

	if (QuestDetails->StageDetails.IsValidIndex(0))
	{
		const FStageDetails& Stage = QuestDetails->StageDetails[0];

		Text_StageDescription->SetText(Stage.Description);

		for (const FObjectiveDetails& QuestObjectiveDetails : Stage.Objectives)
		{
			UMR_QuestLogEntry_Objective* QuestObjectiveWidget =
				CreateWidget<UMR_QuestLogEntry_Objective>(GetWorld(), QuestObjectiveWidgetClass);

			QuestObjectiveWidget->SetObjectiveData(QuestObjectiveDetails);
			QuestObjectiveWidget->SetQuestActor(QuestActor);
			QuestLog_VerticalBox_Objectives->AddChildToVerticalBox(QuestObjectiveWidget);
		}
	}
}

void UMR_QuestLog::SetQuestData(const FDataTableRowHandle& InQuestData)
{
	QuestData = InQuestData;
}

void UMR_QuestLog::CloseQuestLog()
{
	RemoveFromParent();
	bIsOpened = false;
}

void UMR_QuestLog::OpenQuestLog()
{
	AddToViewport();
	bIsOpened = true;
}