// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/MR_InteractionComponent.h"

#include "Components/MR_QuestCompleterComponent.h"
#include "Components/MR_QuestGiverComponent.h"
#include "Components/MR_QuestLogComponent.h"
#include "Components/MR_QuestObjectiveComponent.h"
#include "QuestSystem/MR_QuestBase.h"
#include "QuestSystem/Types/QuestTypes.h"

UMR_InteractionComponent::UMR_InteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMR_InteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMR_InteractionComponent::DoInteract()
{
	const AActor* Actor = PerformInteractionTrace();
	if (!IsValid(Actor)) return;

	if (CanCompleteObjective(Actor)) return;
	if (CanGetQuest(Actor)) return;
	if (CanCompleteQuest(Actor)) return;
}

AActor* UMR_InteractionComponent::PerformInteractionTrace() const
{
	AActor* Owner = GetOwner();
	if (!IsValid(Owner)) return nullptr;

	FVector TraceStart;
	FRotator TraceRotation;

	Owner->GetActorEyesViewPoint(TraceStart, TraceRotation);

	const FVector TraceEnd =
		TraceStart + (TraceRotation.Vector() * InteractionTraceLength);

	FHitResult HitResult;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Owner);

	GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_Pawn,
		QueryParams
	);

	return HitResult.GetActor();
}

FName UMR_InteractionComponent::FindInteractionMessage(const AActor* Actor) const
{
	if (!IsValid(Actor))
	{
		return NAME_None;
	}

	TArray<UActorComponent*> Components;
	Actor->GetComponents(Components);

	for (UActorComponent* Component : Components)
	{
		if (IsValid(Component) &&
			Component->Implements<UMR_QuestInteractionInterface>())
		{
			return IMR_QuestInteractionInterface::Execute_LookAt(Component);
		}
	}

	return NAME_None;
}

void UMR_InteractionComponent::OnToggleQuestLog() const
{
	if (const AActor* PlayerCharacter = GetOwner())
	{
		if (UMR_QuestLogComponent* QuestLogComponent = PlayerCharacter->FindComponentByClass<UMR_QuestLogComponent>())
		{
			QuestLogComponent->ToggleQuestLog();
		}
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

bool UMR_InteractionComponent::CanCompleteQuest(const AActor* Actor)
{
	const AActor* PlayerCharacter = GetOwner();
	if (!IsValid(PlayerCharacter)) return false;

	if (UMR_QuestCompleterComponent* QuestCompleterComponent = Cast<UMR_QuestCompleterComponent>(
		Actor->GetComponentByClass(UMR_QuestCompleterComponent::StaticClass())))
	{
		if (QuestCompleterComponent->Implements<UMR_QuestInteractionInterface>())
		{
			const FName QuestCompleteID =
				IMR_QuestInteractionInterface::Execute_InteractWith(QuestCompleterComponent);

			if (QuestCompleteID.IsNone()) return false;

			UMR_QuestLogComponent* QuestLogComponent =
				PlayerCharacter->FindComponentByClass<UMR_QuestLogComponent>();

			if (!QuestLogComponent) return false;

			return HandleQuestCompletion(
				Actor,
				QuestLogComponent,
				QuestCompleteID);
		}
	}
	return false;
}

bool UMR_InteractionComponent::HandleQuestCompletion(const AActor* Actor, UMR_QuestLogComponent* QuestLogComponent,
	const FName& QuestCompleteID)
{
	AMR_QuestBase* QuestActor = QuestLogComponent->GetQuestActorByID(QuestCompleteID);
	if (!IsValid(QuestActor)) return false;

	const FQuestDetails* QuestDetails = QuestActor->GetQuestDetails();
	if (!QuestDetails) return false;

	if (QuestDetails->bRequiresTurnIn)
	{
		if (UMR_QuestGiverComponent* QuestGiverComponent = Cast<UMR_QuestGiverComponent>(
			Actor->GetComponentByClass(UMR_QuestGiverComponent::StaticClass())))
		{
			QuestGiverComponent->DisplayTurnInQuest(QuestActor);
		}
	}
	else
	{
		QuestLogComponent->CompleteQuest(QuestCompleteID);
	}

	return true;
}

bool UMR_InteractionComponent::CanGetQuest(const AActor* Actor) const
{
	if (UMR_QuestGiverComponent* QuestGiverComponent = Cast<UMR_QuestGiverComponent>(
		Actor->GetComponentByClass(UMR_QuestGiverComponent::StaticClass())))
	{
		if (QuestGiverComponent->Implements<UMR_QuestInteractionInterface>())
		{
			const FName QuestID = IMR_QuestInteractionInterface::Execute_InteractWith(QuestGiverComponent);
			if (!QuestID.IsNone())
			{
				return true;
			}
		}
	}
	return false;
}

bool UMR_InteractionComponent::CanCompleteObjective(const AActor* Actor) const
{
	if (UMR_QuestObjectiveComponent* QuestObjectiveComponent = Cast<UMR_QuestObjectiveComponent>(
		Actor->GetComponentByClass(UMR_QuestObjectiveComponent::StaticClass())))
	{
		if (QuestObjectiveComponent->Implements<UMR_QuestInteractionInterface>())
		{
			const FName ObjectiveID = IMR_QuestInteractionInterface::Execute_InteractWith(QuestObjectiveComponent);
			if (!ObjectiveID.IsNone())
			{
				OnObjectiveIDCalled.Broadcast(ObjectiveID);
				return true;
			}
		}
	}
	return false;
}
