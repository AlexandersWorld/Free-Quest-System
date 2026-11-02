// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MR_QuestLogComponent.generated.h"

class UMR_QuestTracker;
class AMR_QuestBase;
class UMR_QuestLog;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UMR_QuestLogComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMR_QuestLogComponent();
	virtual void BeginPlay() override;
	
	bool QueryActiveQuest(const FName& QuestID) const;
	FDataTableRowHandle GetQuestData() const { return QuestData; }
	TArray<FName> GetCompletedQuests() const { return CompletedQuests; }
	void AddNewQuest(const FName& QuestID, const TObjectPtr<AMR_QuestBase> NewQuest);
	void SetQuestData(const FDataTableRowHandle& InQuestData);
	TArray<FName> GetCurrentActiveQuests() const { return CurrentActiveQuests; }
	TArray<TObjectPtr<AMR_QuestBase>> GetCurrentQuestActors() const { return CurrentQuestActors; }
	AMR_QuestBase* GetQuestActorByID(const FName& InQuestID);
	void ToggleQuestLog();
	void RefreshQuestUI(AMR_QuestBase* InQuestActor) const;

	UFUNCTION()
	void CompleteQuest(const FName& InQuestID);
	
protected:
private:
	UPROPERTY(VisibleDefaultsOnly, Category="Quest System")
	TArray<FName> CurrentActiveQuests;
	UPROPERTY(VisibleDefaultsOnly, Category="Quest System")
	TArray<FName> CompletedQuests;
	UPROPERTY(VisibleDefaultsOnly, Category="Quest System")
	FName CurrentTrackedQuest;
	UPROPERTY(VisibleDefaultsOnly, Category="Quest System")
	TArray<TObjectPtr<AMR_QuestBase>> CurrentQuestActors;
	UPROPERTY(VisibleDefaultsOnly, Category = "Quest System")
	FDataTableRowHandle QuestData;
	
	UPROPERTY(EditDefaultsOnly, Category = "Quest System")
	TSubclassOf<UMR_QuestLog> QuestLogWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Quest System")
	TSubclassOf<UMR_QuestTracker> QuestTrackerClass;
	
	UPROPERTY()
	TObjectPtr<UMR_QuestLog> QuestLogWidget;
	
	UPROPERTY()
	TObjectPtr<UMR_QuestTracker> QuestTracker;
	
	UFUNCTION()
	void TrackQuest();
	
	void OnQuestIDHeard(const FName& InQuestID);
	bool InitializeQuestLog();
};
