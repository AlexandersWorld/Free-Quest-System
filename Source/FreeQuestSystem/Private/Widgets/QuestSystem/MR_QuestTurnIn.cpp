#include "Widgets/QuestSystem/MR_QuestTurnIn.h"

#include "Components/Button.h"
#include "Components/MR_QuestLogComponent.h"
#include "Components/TextBlock.h"
#include "QuestSystem/MR_QuestBase.h"
#include "Widgets/QuestSystem/MR_QuestLogEntry_Objective.h"


void UMR_QuestTurnIn::NativeConstruct()
{
	Super::NativeConstruct();
	
	InitializeQuest();
	
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(Button_CollectRewards->TakeWidget());
		
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = true;
	}
	
	Button_Cancel->OnClicked.RemoveAll(this);
	Button_CollectRewards->OnClicked.RemoveAll(this);
	
	Button_Cancel->OnClicked.AddDynamic(this, &UMR_QuestTurnIn::OnCancel);
	Button_CollectRewards->OnClicked.AddDynamic(this, &UMR_QuestTurnIn::OnGetReward);
}

void UMR_QuestTurnIn::NativeDestruct()
{
	Super::NativeDestruct();
	
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
	}
}

void UMR_QuestTurnIn::OnCancel()
{
	RemoveFromParent();
}

void UMR_QuestTurnIn::OnGetReward()
{
	if (!IsValid(QuestActor) ||
		!QuestActor->GetIsCompleted() ||
		QuestID.IsNone())
	{
		return;
	}

	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		if (APawn* PlayerCharacter = PlayerController->GetPawn())
		{
			if (UMR_QuestLogComponent* QuestLog =
				PlayerCharacter->FindComponentByClass<UMR_QuestLogComponent>())
			{
				QuestLog->CompleteQuest(QuestID);
				RemoveFromParent();
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("FreeQuestSystem: Missing QuestLogComponent on Player Pawn."));
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
}

void UMR_QuestTurnIn::SetQuestData(const FDataTableRowHandle& InQuestData)
{
	QuestData = InQuestData;
}

void UMR_QuestTurnIn::SetQuestID(const FName& InQuestID)
{
	QuestID = InQuestID;
}

void UMR_QuestTurnIn::SetQuestActor(AMR_QuestBase* InQuestActor)
{
	QuestActor = InQuestActor;
}

void UMR_QuestTurnIn::InitializeQuest() const
{
	const FQuestDetails* QuestDetails = QuestData.GetRow<FQuestDetails>(QuestID.ToString());
	if (!QuestDetails) return;
	
	Text_QuestGiverTitle->SetText(QuestDetails->Name);
	Text_QuestDescription->SetText(QuestDetails->LogDescription);
	
	if (QuestDetails->StageDetails.Num() > 0)
	{
		Text_StageDescription->SetText(QuestDetails->StageDetails[0].Description);
	}
}