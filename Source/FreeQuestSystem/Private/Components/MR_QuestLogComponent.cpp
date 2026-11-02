// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/MR_QuestLogComponent.h"

#include "QuestSystem/MR_QuestBase.h"
#include "Widgets/QuestSystem/MR_QuestLog.h"
#include "Widgets/QuestSystem/MR_QuestTracker.h"

UMR_QuestLogComponent::UMR_QuestLogComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMR_QuestLogComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UMR_QuestLogComponent::QueryActiveQuest(const FName& QuestID) const
{
	return CurrentActiveQuests.Contains(QuestID);
}

void UMR_QuestLogComponent::AddNewQuest(const FName& QuestID, const TObjectPtr<AMR_QuestBase> NewQuest)
{
	if (!IsValid(NewQuest) || !QuestID.IsValid()) return;

	if (CurrentActiveQuests.Contains(QuestID)) return;

	CurrentActiveQuests.AddUnique(QuestID);
	CurrentQuestActors.AddUnique(NewQuest);

	NewQuest->SetQuestID(QuestID);
	NewQuest->SetQuestData(QuestData);
	NewQuest->OnQuestIDCalled.AddUObject(this, &UMR_QuestLogComponent::OnQuestIDHeard);

	if (!IsValid(QuestTracker))
	{
		if (!QuestTrackerClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("QuestTracker is NULL! Cannot create tracker."));
			return;
		}

		QuestTracker = CreateWidget<UMR_QuestTracker>(GetWorld(), QuestTrackerClass);
		if (QuestTracker)
		{
			QuestTracker->AddQuestTrackerToViewPort();
		}
	}

	if (QuestTracker)
	{
		QuestTracker->AddOrUpdateQuest(NewQuest);
	}
}
void UMR_QuestLogComponent::SetQuestData(const FDataTableRowHandle& InQuestData)
{
	QuestData = InQuestData;
}

void UMR_QuestLogComponent::OnQuestIDHeard(const FName& InQuestID)
{
	const AMR_QuestBase* QuestActor = GetQuestActorByID(InQuestID);
	if (!QuestActor || !QuestActor->GetIsCompleted())
	{
		return;
	}
	CompleteQuest(InQuestID);
}

bool UMR_QuestLogComponent::InitializeQuestLog()
{
	if (!IsValid(QuestLogWidgetClass)) return false;
	if (IsValid(QuestLogWidget)) return true;
	
	QuestLogWidget = CreateWidget<UMR_QuestLog>(GetWorld(), QuestLogWidgetClass);
	return IsValid(QuestLogWidget);
}

void UMR_QuestLogComponent::ToggleQuestLog()
{
	if (!InitializeQuestLog()) return;

	if (QuestLogWidget->GetIsOpened())
	{
		QuestLogWidget->CloseQuestLog();
		return;
	}
	QuestLogWidget->SetQuestData(QuestData);
	QuestLogWidget->PopulateQuestList(CurrentQuestActors);
	
	QuestLogWidget->OpenQuestLog();
}

void UMR_QuestLogComponent::RefreshQuestUI(AMR_QuestBase* InQuestActor) const
{
	if (IsValid(QuestTracker))
	{
		QuestTracker->AddOrUpdateQuest(InQuestActor);
	}

	if (IsValid(QuestLogWidget))
	{
		QuestLogWidget->UpdateQuestLog(InQuestActor);
	}
}

void UMR_QuestLogComponent::CompleteQuest(const FName& InQuestID)
{
	AMR_QuestBase* QuestActor = GetQuestActorByID(InQuestID);
	
	if (!IsValid(QuestActor) || !QuestActor->GetIsCompleted()) return;
	
	if (const FQuestDetails* Details = QuestActor->GetQuestDetails())
	{
		for (const FStageDetails& Stage : Details->StageDetails)
		{
			UE_LOG(LogTemp, Warning, TEXT("REWARDS : %d"), Stage.XPReward);
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,
					5.0f,
					FColor::Red,
					FString::Printf(TEXT("REWARDS : %d"), Stage.XPReward)
				);
			}
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("QUEST COMPLETED: %s"), *InQuestID.ToString());
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.0f,
			FColor::Red,
            FString::Printf(TEXT("QUEST COMPLETED: %s"), *InQuestID.ToString())
		);
	}
	
	if (IsValid(QuestTracker))
	{
		QuestTracker->RemoveQuest(QuestActor);
	}
	
	CompletedQuests.AddUnique(InQuestID);
	CurrentActiveQuests.Remove(InQuestID);
	
	QuestActor->OnQuestIDCalled.RemoveAll(this);
	CurrentQuestActors.Remove(QuestActor);
	
	if (CurrentQuestActors.Num() == 0 && IsValid(QuestTracker))
	{
		QuestTracker->RemoveQuestTrackerFromViewPort();
		QuestTracker = nullptr;
	}
}

void UMR_QuestLogComponent::TrackQuest()
{
	//TODO: Logic for tracking specific quest
}

AMR_QuestBase* UMR_QuestLogComponent::GetQuestActorByID(const FName& InQuestID)
{
	for (AMR_QuestBase* QuestActor : CurrentQuestActors)
	{
		if (QuestActor->GetQuestID() == InQuestID)
		{
			return QuestActor;
		}
	}
	return nullptr;
}


