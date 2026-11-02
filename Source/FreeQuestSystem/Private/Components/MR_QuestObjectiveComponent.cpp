// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/MR_QuestObjectiveComponent.h"

#include "Components/MR_QuestLogComponent.h"
#include "Kismet/GameplayStatics.h"
#include "QuestSystem/MR_QuestBase.h"

UMR_QuestObjectiveComponent::UMR_QuestObjectiveComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FName UMR_QuestObjectiveComponent::InteractWith_Implementation()
{
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController) return NAME_None;
	
	if (ObjectiveID.IsNone()) return NAME_None;
	
	if (const APawn* PlayerCharacter = PlayerController->GetPawn())
	{
		if (const UMR_QuestLogComponent* QuestLogComponent = PlayerCharacter->FindComponentByClass<UMR_QuestLogComponent>())
		{
			for (const AMR_QuestBase* Quest : QuestLogComponent->GetCurrentQuestActors())
			{
				if (IsValid(Quest) && Quest->CanProgressObjective(ObjectiveID))
				{
					return ObjectiveID;
				}
			}
		}
	}
	return NAME_None;
}

FName UMR_QuestObjectiveComponent::LookAt_Implementation()
{
	return InteractionMessage.IsNone() ? NAME_None : InteractionMessage;
}

void UMR_QuestObjectiveComponent::UnLookAt_Implementation()
{
	IMR_QuestInteractionInterface::UnLookAt_Implementation();
}
