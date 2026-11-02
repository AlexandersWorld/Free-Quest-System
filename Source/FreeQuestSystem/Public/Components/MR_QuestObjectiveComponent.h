// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/MR_QuestInteractionInterface.h"
#include "MR_QuestObjectiveComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UMR_QuestObjectiveComponent : public UActorComponent, public IMR_QuestInteractionInterface
{
	GENERATED_BODY()

public:
	UMR_QuestObjectiveComponent();
	
	/* Quest Interaction Interface Start */
	virtual FName InteractWith_Implementation() override;
	virtual FName LookAt_Implementation() override;
	virtual void UnLookAt_Implementation() override;
	/* Quest Interaction Interface End */

private:
	UPROPERTY(EditAnywhere, Category = "QuestSystem")
	FName ObjectiveID;
	
	UPROPERTY(EditDefaultsOnly, Category="Quest System")
	FName InteractionMessage;
};
