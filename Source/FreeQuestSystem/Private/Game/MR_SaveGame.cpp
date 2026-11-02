#include "Game/MR_SaveGame.h"
#include "Components/MR_QuestLogComponent.h"
#include "QuestSystem/MR_QuestBase.h"

void UMR_SaveGame::SaveQuestDetails(const TObjectPtr<AMR_QuestBase> QuestActor)
{
	if (!IsValid(QuestActor)) return;

	FQuestSaveData SaveData;
	SaveData.CurrentStage = QuestActor->GetCurrentStage();
	SaveData.Progress = QuestActor->GetCurrentObjectiveProgress();
    SaveData.QuestRowHandle = QuestActor->GetQuestData();
	SaveData.Details = *QuestActor->GetQuestDetails();
	
	QuestProgress.Add(QuestActor->GetQuestID(), SaveData);
}

void UMR_SaveGame::LoadQuests(UMR_QuestLogComponent* QuestLogComponent, UWorld* WorldContext)
{
	if (!IsValid(QuestLogComponent) || !QuestBaseClass || !IsValid(WorldContext))
	{
		return;
	}

	for (const FName& CurrentActiveQuest : CurrentActiveQuests)
	{
		FQuestSaveData* SaveData = QuestProgress.Find(CurrentActiveQuest);
		if (!SaveData)
		{
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("[LoadQuests] No progress data found for active quest: %s"),
				*CurrentActiveQuest.ToString()
			);
			continue;
		}

		if (SaveData->QuestRowHandle.IsNull() || !SaveData->QuestRowHandle.DataTable)
		{
			UE_LOG(LogTemp, Warning, TEXT("[LoadQuests] Saved QuestRowHandle is null for quest: %s"), *CurrentActiveQuest.ToString());
			continue;
		}

		static const FString ContextString(TEXT("LoadQuestsContext"));
		FQuestDetails* QuestDetails = SaveData->QuestRowHandle.GetRow<FQuestDetails>(ContextString);
		if (!QuestDetails)
		{
			UE_LOG(LogTemp, Warning, TEXT("[LoadQuests] Failed to find FQuestDetails row for: %s"), *CurrentActiveQuest.ToString());
			continue;
		}

		FActorSpawnParameters SpawnParams;
		AMR_QuestBase* QuestActor = WorldContext->SpawnActor<AMR_QuestBase>(QuestBaseClass, SpawnParams);
		if (!IsValid(QuestActor))
		{
			continue;
		}

		QuestActor->SetQuestID(CurrentActiveQuest);
		QuestActor->RestoreQuestProgress(*SaveData);

		QuestLogComponent->SetQuestData(SaveData->QuestRowHandle);
		QuestLogComponent->AddNewQuest(CurrentActiveQuest, QuestActor);
	}
}
