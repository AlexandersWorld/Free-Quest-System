#include "Components/MR_QuestGiverComponent.h"

#include "Components/MR_QuestLogComponent.h"
#include "Kismet/GameplayStatics.h"
#include "QuestSystem/MR_QuestBase.h"
#include "Widgets/QuestSystem/MR_QuestGiver.h"
#include "Widgets/QuestSystem/MR_QuestTurnIn.h"

UMR_QuestGiverComponent::UMR_QuestGiverComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FName UMR_QuestGiverComponent::InteractWith_Implementation()
{
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController) return NAME_None;
	
	if (const APawn* PlayerCharacter = PlayerController->GetPawn())
	{
		if (UMR_QuestLogComponent* QuestLogComponent = PlayerCharacter->FindComponentByClass<UMR_QuestLogComponent>())
		{
			const TArray<FName> CompletedQuests = QuestLogComponent->GetCompletedQuests();

			const bool bIsActiveQuest = QuestLogComponent->QueryActiveQuest(QuestData.RowName);

			const bool bIsCompletedQuest = CompletedQuests.Contains(QuestData.RowName);

			if (bIsActiveQuest || bIsCompletedQuest)
			{
				return NAME_None;
			}

			DisplayQuest();
			
			return QuestData.RowName;
		}
	}

	return NAME_None;
}

void UMR_QuestGiverComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UMR_QuestGiverComponent::DisplayQuest() const
{
	if (!QuestWidgetClass || !QuestData.DataTable)
	{
		return;
	}
	
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (UMR_QuestGiver* QuestGiverWidget = CreateWidget<UMR_QuestGiver>(PlayerController, QuestWidgetClass))
		{
			QuestGiverWidget->SetQuestData(QuestData);
			QuestGiverWidget->SetQuestID(QuestData.RowName);
			QuestGiverWidget->SetQuestBaseClass(QuestBaseClass); 
			QuestGiverWidget->AddToViewport();
		}
	}
}

void UMR_QuestGiverComponent::DisplayTurnInQuest(AMR_QuestBase* QuestActorToTurnIn) const
{
	if (!QuestTurnInWidgetClass || !QuestData.DataTable || !IsValid(QuestActorToTurnIn))
	{
		return;
	}
	
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (UMR_QuestTurnIn* QuestTurnInWidget = CreateWidget<UMR_QuestTurnIn>(PlayerController, QuestTurnInWidgetClass))
		{
			QuestTurnInWidget->SetQuestData(QuestActorToTurnIn->GetQuestData());
			QuestTurnInWidget->SetQuestID(QuestActorToTurnIn->GetQuestID());
			QuestTurnInWidget->SetQuestActor(QuestActorToTurnIn);
			QuestTurnInWidget->AddToViewport();
		}
	}
}

FName UMR_QuestGiverComponent::LookAt_Implementation()
{
	return InteractionMessage.IsNone() ? NAME_None : InteractionMessage;
}

void UMR_QuestGiverComponent::UnLookAt_Implementation()
{
	IMR_QuestInteractionInterface::UnLookAt_Implementation();
}