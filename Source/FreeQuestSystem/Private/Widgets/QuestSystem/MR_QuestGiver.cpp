// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/QuestSystem/MR_QuestGiver.h"

#include "Components/Button.h"
#include "Components/MR_QuestLogComponent.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "QuestSystem/MR_QuestBase.h"
#include "QuestSystem//Types/QuestTypes.h"
#include "Widgets/QuestSystem/MR_QuestLogEntry_Objective.h"

void UMR_QuestGiver::NativeConstruct()
{
	Super::NativeConstruct();
	
	InitializeQuest();
	
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(Button_Accept->TakeWidget());
		
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = true;
	}
	
	Button_Decline->OnClicked.RemoveAll(this);
	Button_Accept->OnClicked.RemoveAll(this);
	
	Button_Decline->OnClicked.AddDynamic(this, &UMR_QuestGiver::OnDeclineQuest);
	Button_Accept->OnClicked.AddDynamic(this, &UMR_QuestGiver::OnAcceptQuest);
}

void UMR_QuestGiver::NativeDestruct()
{
	
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
	}
	
	Super::NativeDestruct();
}

void UMR_QuestGiver::OnDeclineQuest()
{
	RemoveFromParent();
}

void UMR_QuestGiver::OnAcceptQuest()
{
	if (!CachedQuestBaseClass) return;
	
	const APlayerController* PlayerController = GetOwningPlayer();
	if (!PlayerController) return;
	
	if (const APawn* PlayerCharacter = PlayerController->GetPawn())
	{
		if (UMR_QuestLogComponent* QuestLogComponent = PlayerCharacter->FindComponentByClass<UMR_QuestLogComponent>())
		{
			if (AMR_QuestBase* SpawnedActor = GetWorld()->SpawnActor<AMR_QuestBase>(CachedQuestBaseClass))
			{
				QuestLogComponent->SetQuestData(QuestData);
				QuestLogComponent->AddNewQuest(QuestID, SpawnedActor);
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
	RemoveFromParent();
}

void UMR_QuestGiver::SetQuestData(const FDataTableRowHandle& InQuestData)
{
	QuestData = InQuestData;
}

void UMR_QuestGiver::SetQuestID(const FName& InQuestID)
{
	QuestID = InQuestID;
}

void UMR_QuestGiver::SetQuestBaseClass(TSubclassOf<AMR_QuestBase> InQuestClass)
{
	CachedQuestBaseClass = InQuestClass;
}

void UMR_QuestGiver::InitializeQuest() const
{
	const FQuestDetails* QuestDetails = QuestData.GetRow<FQuestDetails>(QuestID.ToString());
	if (!QuestDetails) return;

	QuestGiver_VerticalBox_Objectives->ClearChildren();
	Text_QuestGiverTitle->SetText(QuestDetails->Name);
	Text_QuestDescription->SetText(QuestDetails->LogDescription);

	if (QuestDetails->StageDetails.IsValidIndex(0))
	{
		const FStageDetails& Stage = QuestDetails->StageDetails[0];

		Text_StageDescription->SetText(Stage.Description);

		for (const FObjectiveDetails& QuestObjectiveDetails : Stage.Objectives)
		{
			UMR_QuestLogEntry_Objective* QuestObjectiveWidget =
				CreateWidget<UMR_QuestLogEntry_Objective>(GetWorld(), QuestObjectiveWidgetClass);

			QuestObjectiveWidget->SetObjectiveData(QuestObjectiveDetails);
			QuestGiver_VerticalBox_Objectives->AddChildToVerticalBox(QuestObjectiveWidget);
		}
	}
}
