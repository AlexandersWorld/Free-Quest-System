// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/MR_UserWidget.h"
#include "MR_QuestTracker.generated.h"

class UTextBlock;
class UVerticalBox;
class AMR_QuestBase;
class UMR_QuestTrackerEntry;
class UMR_QuestLogEntry_Objective;

/**
 * 
 */
UCLASS()
class UMR_QuestTracker : public UMR_UserWidget
{
	GENERATED_BODY()
	
public:
	void AddOrUpdateQuest(AMR_QuestBase* InQuestActor);
	void RemoveQuest(AMR_QuestBase* InQuestActor);
	void AddQuestTrackerToViewPort();
	void RemoveQuestTrackerFromViewPort();
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox_Quests;

	// The class for the individual quest entry widget
	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	TSubclassOf<UMR_QuestTrackerEntry> QuestEntryWidgetClass;

	UPROPERTY()
	TMap<TObjectPtr<AMR_QuestBase>, TObjectPtr<UMR_QuestTrackerEntry>> QuestWidgetMap;
};
