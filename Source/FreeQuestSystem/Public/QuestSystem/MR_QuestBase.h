// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestIDCalled, const FName&)

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Types/QuestTypes.h"
#include "MR_QuestBase.generated.h"

class UMR_QuestNotification;

UCLASS(Blueprintable, BlueprintType)
class AMR_QuestBase : public AActor
{
	GENERATED_BODY()

public:
	AMR_QuestBase();

	void SetQuestID(const FName& InQuestID);
	void SetQuestData(const FDataTableRowHandle& InQuestData);
	
	const FName& GetQuestID() const { return QuestID; };
	const FDataTableRowHandle& GetQuestData() const { return QuestData; }
	bool GetIsCompleted() const { return bIsCompleted; }
	TMap<FName, int32> GetCurrentObjectiveProgress() const { return CurrentObjectiveProgress; }
	const FObjectiveDetails* GetObjectiveDataByID(const FName& InQuestID) const;
	int32 GetCurrentStage() const { return CurrentStage; }
	const FQuestDetails* GetQuestDetails() const;
	bool CanProgressObjective(const FName& ObjectiveID) const;
	void RestoreQuestProgress(const FQuestSaveData& SaveData);
	
	FOnQuestIDCalled OnQuestIDCalled;
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Quest System")
	FName QuestID;

	UPROPERTY(VisibleDefaultsOnly, Category = "Quest System")
	FDataTableRowHandle QuestData;

	UPROPERTY(VisibleDefaultsOnly, Category = "Quest System|Progress")
	int32 CurrentStage {0};

	UPROPERTY(VisibleDefaultsOnly, Category = "Quest System|Progress")
	TMap<FName, int32> CurrentObjectiveProgress;
		
	TMap<FName, int32> LoadCurrentObjectiveProgress;

	UPROPERTY(VisibleDefaultsOnly, Category = "Quest System|Progress")
	bool bIsCompleted {false};
	
	void OnObjectiveIDHeard(const FName& ObjectiveID);
	void CheckObjectivesState(const FDataTableRowHandle& InQuestData);
	bool IsObjectiveComplete(const int32 Quantity, const int32 Value) const;
	void SpawnQuestNotification(const FText& ObjectiveDescription) const;
	void UpdateQuestState();
	void RefreshUI();
	
	UPROPERTY(VisibleDefaultsOnly, Category="Quest System")
	FQuestDetails QuestDetails;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest System", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UMR_QuestNotification> QuestNotificationWidgetClass;
};
