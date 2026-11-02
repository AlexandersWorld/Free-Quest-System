// MR_QuestTracker.cpp
#include "Widgets/QuestSystem/MR_QuestTracker.h"

#include "Components/VerticalBox.h"
#include "QuestSystem/MR_QuestBase.h"
#include "Widgets/QuestSystem/MR_QuestTrackerEntry.h"

void UMR_QuestTracker::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMR_QuestTracker::NativeDestruct()
{
	QuestWidgetMap.Empty();
	Super::NativeDestruct();
}

void UMR_QuestTracker::AddOrUpdateQuest(AMR_QuestBase* InQuestActor)
{
	if (!InQuestActor || !VerticalBox_Quests || !QuestEntryWidgetClass) return;

	if (QuestWidgetMap.Contains(InQuestActor))
	{
		if (UMR_QuestTrackerEntry* ExistingEntry = QuestWidgetMap[InQuestActor])
		{
			ExistingEntry->UpdateQuestEntry(InQuestActor);
		}
		return;
	}

	if (UMR_QuestTrackerEntry* NewQuestEntry = CreateWidget<UMR_QuestTrackerEntry>(this, QuestEntryWidgetClass))
	{
		NewQuestEntry->InitializeQuestEntry(InQuestActor);
		VerticalBox_Quests->AddChildToVerticalBox(NewQuestEntry);
		QuestWidgetMap.Add(InQuestActor, NewQuestEntry);
	}

	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UMR_QuestTracker::RemoveQuest(AMR_QuestBase* InQuestActor)
{
	if (!InQuestActor || !QuestWidgetMap.Contains(InQuestActor)) return;

	if (UMR_QuestTrackerEntry* WidgetToRemove = QuestWidgetMap[InQuestActor])
	{
		WidgetToRemove->RemoveFromParent();
		QuestWidgetMap.Remove(InQuestActor);
	}
}

void UMR_QuestTracker::RemoveQuestTrackerFromViewPort()
{
	RemoveFromParent();
}

void UMR_QuestTracker::AddQuestTrackerToViewPort()
{
	if (!IsInViewport())
	{
		AddToViewport(-1);
	}
}
