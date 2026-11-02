// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/MR_UserWidget.h"
#include "MR_QuestLog.generated.h"

UENUM()
enum ESwitcher : uint8
{
	SwitchInvisible,
	SwitchVisible, 
};

class UButton;
class UTextBlock;
class UScrollBox;
class UWidgetSwitcher;
class UMR_QuestLog_QuestEntry;
class UMR_QuestLogEntry_Objective;
class UMR_QuestLogComponent;
class UVerticalBox;
class AMR_QuestBase;

/**
 * 
 */
UCLASS()
class UMR_QuestLog : public UMR_UserWidget
{
	GENERATED_BODY()
	
public:
	void SetQuestData(const FDataTableRowHandle& InQuestData);
	void SetQuestID(const FName& InQuestID) { QuestID = InQuestID; }
	bool GetIsOpened() const { return bIsOpened; };
	const FName& GetQuestID() const { return QuestID; };
	void PopulateQuestList(const TArray<AMR_QuestBase*>& InQuestActors);
	void UpdateQuestLog(AMR_QuestBase* InQuestActor);
	
	UFUNCTION()
	void CloseQuestLog();
	void OpenQuestLog();
protected:
	virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Close;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_StageDescription;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_QuestLogTitle;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_QuestDescription;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UScrollBox> Scroll_MainQuest;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UScrollBox> Scroll_SideQuest;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> QuestLog_VerticalBox_Objectives;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> Switcher;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "QuestS System")
	FName QuestID;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "QuestS System")
	FDataTableRowHandle QuestData;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "QuestS System")
	TArray<TObjectPtr<AMR_QuestBase>> QuestActors;
	
	UPROPERTY(EditDefaultsOnly, Category = "QuestS System")
	TSubclassOf<UMR_QuestLog_QuestEntry> QuestEntryWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "QuestS System")
	TSubclassOf<UMR_QuestLogEntry_Objective> QuestObjectiveWidgetClass;
	
	UPROPERTY()
	TObjectPtr<AMR_QuestBase> CurrentDisplayedQuest;
	
	void OnQuestSelected(const FName& InQuestID, AMR_QuestBase* QuestActor);
	
	void DisplayQuest(const FName& InQuestID, AMR_QuestBase* QuestActor);
	void RefreshQuestLog() const;
	
	bool bIsOpened;
};
