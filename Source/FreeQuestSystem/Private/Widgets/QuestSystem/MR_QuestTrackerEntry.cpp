#include "Widgets/QuestSystem/MR_QuestTrackerEntry.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "QuestSystem/MR_QuestBase.h"
#include "QuestSystem/Types/QuestTypes.h"
#include "Widgets/QuestSystem/MR_QuestLogEntry_Objective.h"

void UMR_QuestTrackerEntry::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMR_QuestTrackerEntry::NativeDestruct()
{
	ObjectiveWidgets.Empty();
	Super::NativeDestruct();
}

void UMR_QuestTrackerEntry::InitializeQuestEntry(AMR_QuestBase* InQuestActor)
{
	if (!InQuestActor || !VerticalBox_Objectives || !QuestObjectiveWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestTrackerEntry: Missing Actor, VerticalBox, or WidgetClass!"));
		return;
	}

	SetQuestActor(InQuestActor);

	VerticalBox_Objectives->ClearChildren();
	ObjectiveWidgets.Empty();

	if (const FQuestDetails* QuestDetails = InQuestActor->GetQuestDetails())
	{
		Text_QuestName->SetText(QuestDetails->Name);

		for (const FStageDetails& Stage : QuestDetails->StageDetails)
		{
			for (const FObjectiveDetails& QuestObjectiveDetails : Stage.Objectives)
			{
				if (UMR_QuestLogEntry_Objective* QuestObjectiveWidget = CreateWidget<UMR_QuestLogEntry_Objective>(this, QuestObjectiveWidgetClass))
				{
					QuestObjectiveWidget->SetObjectiveData(QuestObjectiveDetails);
					QuestObjectiveWidget->SetQuestActor(InQuestActor);
					VerticalBox_Objectives->AddChildToVerticalBox(QuestObjectiveWidget);
					ObjectiveWidgets.Add(QuestObjectiveWidget);
				}
			}
		}
	}

	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UMR_QuestTrackerEntry::UpdateQuestEntry(const AMR_QuestBase* InQuestActor)
{
	if (!InQuestActor) return;

	if (const FQuestDetails* QuestDetails = InQuestActor->GetQuestDetails())
	{
		int32 WidgetIndex = 0;

		for (const FStageDetails& Stage : QuestDetails->StageDetails)
		{
			for (const FObjectiveDetails& QuestObjectiveDetails : Stage.Objectives)
			{
				if (ObjectiveWidgets.IsValidIndex(WidgetIndex) && IsValid(ObjectiveWidgets[WidgetIndex]))
				{
					ObjectiveWidgets[WidgetIndex]->SetObjectiveData(QuestObjectiveDetails);
				}
				WidgetIndex++;
			}
		}
	}
}

void UMR_QuestTrackerEntry::SetQuestActor(AMR_QuestBase* InQuestActor)
{
	QuestActor = InQuestActor;
}