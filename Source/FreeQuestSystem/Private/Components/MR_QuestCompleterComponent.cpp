// Fill out your copyright notice in the Description page of Project Settings.
#include "Components/MR_QuestCompleterComponent.h"

#include "Components/MR_QuestLogComponent.h"
#include "Kismet/GameplayStatics.h"
#include "QuestSystem/MR_QuestBase.h"

UMR_QuestCompleterComponent::UMR_QuestCompleterComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMR_QuestCompleterComponent::BeginPlay()
{
	Super::BeginPlay();
}

FName UMR_QuestCompleterComponent::InteractWith_Implementation()
{
	if (QuestCompleteID.IsNone()) return NAME_None;
	
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController) return NAME_None;
	
	if (const APawn* PlayerCharacter = PlayerController->GetPawn())
	{
		const UMR_QuestLogComponent* QuestLogComponent = PlayerCharacter->FindComponentByClass<UMR_QuestLogComponent>();
		if (!QuestLogComponent) return NAME_None;
		
		if (QuestLogComponent->GetCompletedQuests().Contains(QuestCompleteID)) 
		{
			return NAME_None;
		}
		
		for (const AMR_QuestBase* CurrentQuests : QuestLogComponent->GetCurrentQuestActors())
		{
			if (CurrentQuests && CurrentQuests->GetIsCompleted() && CurrentQuests->GetQuestID() == QuestCompleteID)
			{
				return QuestCompleteID;
			}
		}
	}
	return NAME_None;
}

FName UMR_QuestCompleterComponent::LookAt_Implementation()
{
	return InteractionMessage.IsNone() ? NAME_None : InteractionMessage;
}

void UMR_QuestCompleterComponent::UnLookAt_Implementation()
{
	IMR_QuestInteractionInterface::UnLookAt_Implementation();
}
