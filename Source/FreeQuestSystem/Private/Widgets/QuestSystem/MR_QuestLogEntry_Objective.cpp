// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/QuestSystem/MR_QuestLogEntry_Objective.h"
#include "Components/TextBlock.h"
#include "Components/CheckBox.h"
#include "QuestSystem/MR_QuestBase.h"

void UMR_QuestLogEntry_Objective::NativeConstruct()
{
	Super::NativeConstruct();
	
	UpdateUI();
}

void UMR_QuestLogEntry_Objective::SetObjectiveData(const FObjectiveDetails& InObjectiveData)
{
	ObjectiveData = InObjectiveData;
	UpdateUI();
}

void UMR_QuestLogEntry_Objective::SetQuestActor(AMR_QuestBase* InQuestActor)
{
	QuestActor = InQuestActor;
	UpdateUI();
}

void UMR_QuestLogEntry_Objective::UpdateUI() const
{
	if (!Text_QuestDescription || !CheckBox_IsCompleted) return;

	int32 CurrentProgressValue = 0;

	if (const AMR_QuestBase* QA = QuestActor.Get())
	{
		const TMap<FName, int32>& CurrentObjectiveProgress = QA->GetCurrentObjectiveProgress();
		if (const int32* Progress = CurrentObjectiveProgress.Find(ObjectiveData.ID))
		{
			CurrentProgressValue = *Progress;
		}
	}

	Text_QuestDescription->SetText(
		FText::Format(
			FText::FromString("{0} {1}/{2}"),
			ObjectiveData.Description,
			CurrentProgressValue,
			ObjectiveData.Quantity
		)
	);

	CheckBox_IsCompleted->SetCheckedState(
		CurrentProgressValue >= ObjectiveData.Quantity
			? ECheckBoxState::Checked
			: ECheckBoxState::Unchecked
	);
}