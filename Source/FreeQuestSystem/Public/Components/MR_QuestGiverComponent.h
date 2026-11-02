// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/MR_QuestInteractionInterface.h"
#include "MR_QuestGiverComponent.generated.h"

class UMR_QuestGiver;
class AMR_QuestBase;
class UMR_QuestTurnIn;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UMR_QuestGiverComponent : public UActorComponent, public IMR_QuestInteractionInterface
{
	GENERATED_BODY()

public:
	UMR_QuestGiverComponent();
	virtual void BeginPlay() override;
	FDataTableRowHandle GetQuestData() const { return QuestData; }
	
	/* Quest Interaction Interface Start */
	virtual FName InteractWith_Implementation() override;
	virtual FName LookAt_Implementation() override;
	virtual void UnLookAt_Implementation() override;
	/* Quest Interaction Interface End */
	
	UFUNCTION()
	void DisplayTurnInQuest(AMR_QuestBase* QuestActorToTurnIn) const;
private:
	UPROPERTY(EditAnywhere, Category="Quest System")
	FDataTableRowHandle QuestData;
	
	UPROPERTY(EditDefaultsOnly, Category="Quest System")
	FName InteractionMessage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest System", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UMR_QuestGiver> QuestWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest System", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UMR_QuestTurnIn> QuestTurnInWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Quest System", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMR_QuestBase> QuestBaseClass;
	
	UFUNCTION()
	void DisplayQuest() const;

};
