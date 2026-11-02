// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


DECLARE_MULTICAST_DELEGATE_OneParam(FOnObjectiveIDCalled, const FName&)
DECLARE_MULTICAST_DELEGATE(FOnQuestGiverInteract)

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MR_InteractionComponent.generated.h"

class UMR_QuestLog;
class UMR_HUDWidget;
class UMR_QuestLogComponent;
class UMR_QuestGiverComponent;
class UMR_QuestInteractionInterface;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UMR_InteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMR_InteractionComponent();

	virtual void BeginPlay() override;

	FOnObjectiveIDCalled OnObjectiveIDCalled;
	FOnQuestGiverInteract OnQuestGiverInteract;
	
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void DoInteract();
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void OnToggleQuestLog() const;
private:
	bool CanCompleteQuest(const AActor* Actor);
	bool CanGetQuest(const AActor* Actor) const;
	bool CanCompleteObjective(const AActor* Actor) const;
	FName FindInteractionMessage(const AActor* Actor) const;
	
	UPROPERTY()
	TObjectPtr<UActorComponent> CurrentFocusComponent;
	
	UPROPERTY()
	TObjectPtr<UMR_HUDWidget> HUDWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta=(AllowPrivateAccess = "true"))
	float InteractionTraceLength {500.0f};
	
	AActor* PerformInteractionTrace() const;
	bool HandleQuestCompletion(const AActor* Actor, UMR_QuestLogComponent* QuestLogComponent, const FName& QuestCompleteID);
};
