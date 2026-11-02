
#include "QuestSystem/MR_QuestBase.h"

#include "FreeQuestSystemCharacter.h"
#include "FreeQuestSystemPlayerController.h"
#include "Components/MR_InteractionComponent.h"
#include "Components/MR_QuestLogComponent.h"
#include "Kismet/GameplayStatics.h"
#include "QuestSystem/Types/QuestTypes.h"
#include "Widgets/QuestSystem/MR_QuestNotification.h"

AMR_QuestBase::AMR_QuestBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMR_QuestBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (APawn* PlayerCharacter = PlayerController->GetPawn())
		{
			if (UMR_InteractionComponent* InteractionComponent = PlayerCharacter->FindComponentByClass<UMR_InteractionComponent>())
			{
				InteractionComponent->OnObjectiveIDCalled.AddUObject(this, &AMR_QuestBase::OnObjectiveIDHeard);
			}
			else
			{
				UE_LOG(LogTemp, Warning,
					TEXT("FreeQuestSystem: Missing InteractionComponent on Player Pawn."));

				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(
						-1,
						5.0f,
						FColor::Red,
						TEXT("FreeQuestSystem: Add a InteractionComponent to your Player Pawn!")
					);
				}
			}
		}
	}
}

void AMR_QuestBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{	
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (APawn* PlayerCharacter = PlayerController->GetPawn())
		{
			if (UMR_InteractionComponent* InteractionComponent = PlayerCharacter->FindComponentByClass<UMR_InteractionComponent>())
			{
				InteractionComponent->OnObjectiveIDCalled.RemoveAll(this);
			}
		}
	}
	Super::EndPlay(EndPlayReason);
}

void AMR_QuestBase::OnObjectiveIDHeard(const FName& ObjectiveID)
{
	if (QuestDetails.StageDetails.IsEmpty()) return;
	
	const FObjectiveDetails* ObjectiveDetails = GetObjectiveDataByID(ObjectiveID);
	if (!ObjectiveDetails) return;
	
	if (int32* Value = CurrentObjectiveProgress.Find(ObjectiveID))
	{
		if (*Value < ObjectiveDetails->Quantity)
		{
			++(*Value);
			
			UpdateQuestState();
			
			if (IsObjectiveComplete(ObjectiveDetails->Quantity, *Value))
			{
				SpawnQuestNotification(ObjectiveDetails->Description);
			}
		}
	}
	
	RefreshUI();
	
	if (!QuestDetails.bRequiresTurnIn && GetIsCompleted())
	{
		OnQuestIDCalled.Broadcast(GetQuestID());
	}
}

const FObjectiveDetails* AMR_QuestBase::GetObjectiveDataByID(const FName& InQuestID) const
{
	if (!QuestDetails.StageDetails.IsValidIndex((CurrentStage))) return nullptr;
	
	for (const FObjectiveDetails& ObjectiveDetails : QuestDetails.StageDetails[CurrentStage].Objectives)
	{
		if (ObjectiveDetails.ID == InQuestID)
		{
			return &ObjectiveDetails;
		}
	}
	return nullptr;
}

void AMR_QuestBase::SetQuestID(const FName& InQuestID)
{
	QuestID = InQuestID;
	QuestData.RowName = InQuestID;
}

void AMR_QuestBase::SetQuestData(const FDataTableRowHandle& InQuestData)
{
	QuestData = InQuestData;
	
	CheckObjectivesState(InQuestData);
}

void AMR_QuestBase::CheckObjectivesState(const FDataTableRowHandle& InQuestData)
{
	const FQuestDetails* Row = InQuestData.GetRow<FQuestDetails>(InQuestData.RowName.ToString());
	if (!Row) return;
	
	QuestDetails = *Row;
	
	CurrentObjectiveProgress.Empty();
	
	for (const FStageDetails& Stage : QuestDetails.StageDetails)
	{
		for (const FObjectiveDetails& Objective : Stage.Objectives)
		{
			const int32 SavedProgress = LoadCurrentObjectiveProgress.FindRef(Objective.ID);

			CurrentObjectiveProgress.Add(Objective.ID, SavedProgress);
		}
	}
	UpdateQuestState();
}

bool AMR_QuestBase::IsObjectiveComplete(const int32 Quantity, const int32 Value) const
{
	return (Value >= Quantity);
}

void AMR_QuestBase::SpawnQuestNotification(const FText& ObjectiveDescription) const
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (UMR_QuestNotification* QuestWidgetNotification = CreateWidget<UMR_QuestNotification>(PlayerController, QuestNotificationWidgetClass))
		{
			QuestWidgetNotification->SetObjectiveDescription(ObjectiveDescription);
			QuestWidgetNotification->AddToViewport();
		}
	}
}

void AMR_QuestBase::UpdateQuestState()
{
	// Quest is only complete when every objective in the FINAL stage is done.
	const int32 FinalStageIdx = QuestDetails.StageDetails.Num() - 1;

	if (!QuestDetails.StageDetails.IsValidIndex(FinalStageIdx))
	{
		bIsCompleted = false;
		return;
	}

	for (const FObjectiveDetails& Objective : QuestDetails.StageDetails[FinalStageIdx].Objectives)
	{
		const int32 Progress = CurrentObjectiveProgress.FindRef(Objective.ID);

		if (Progress < Objective.Quantity)
		{
			bIsCompleted = false;
			return;
		}
	}

	bIsCompleted = true;
}

void AMR_QuestBase::RefreshUI()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController) return;
	
	if (const APawn* PlayerCharacter = PlayerController->GetPawn())
	{
		if (UMR_QuestLogComponent* QuestLogComponent = PlayerCharacter->FindComponentByClass<UMR_QuestLogComponent>())
		{
			QuestLogComponent->RefreshQuestUI(this);
		}
		else
		{
			UE_LOG(LogTemp, Warning,
				TEXT("FreeQuestSystem: Missing QuestLogComponent on Player Pawn."));

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,
					5.0f,
					FColor::Red,
					TEXT("FreeQuestSystem: Add a QuestLogComponent to your Player Pawn!")
				);
			}
		}
	}
}

const FQuestDetails* AMR_QuestBase::GetQuestDetails() const
{
	return QuestData.GetRow<FQuestDetails>(QuestData.RowName.ToString());
}

bool AMR_QuestBase::CanProgressObjective(const FName& ObjectiveID) const
{
	if (GetIsCompleted()) return false;
	
	const FObjectiveDetails* Objective = GetObjectiveDataByID(ObjectiveID);
	if (!Objective) return false;
	
	const int32 Progress = CurrentObjectiveProgress.FindRef(ObjectiveID);
	
	return Progress < Objective->Quantity;
}

void AMR_QuestBase::RestoreQuestProgress(const FQuestSaveData& SaveData)
{
	if (!SaveData.Progress.IsEmpty())
	{
		LoadCurrentObjectiveProgress = SaveData.Progress;
	}
	if (SaveData.CurrentStage >= 0)
	{
		CurrentStage = SaveData.CurrentStage;
	}
	if (!SaveData.QuestRowHandle.IsNull())
	{
		SetQuestData(SaveData.QuestRowHandle);
	}
	if (SaveData.Details.IsValid())
	{
		QuestDetails = SaveData.Details;
	}
}
