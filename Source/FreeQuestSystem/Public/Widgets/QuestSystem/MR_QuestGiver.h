// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/MR_UserWidget.h"
#include "MR_QuestGiver.generated.h"

class AMR_QuestBase;
class UTextBlock;
class UButton;
class UScrollBox;
class UVerticalBox;
class UMR_QuestLogEntry_Objective;

/**
 * 
 */
UCLASS()
class UMR_QuestGiver : public UMR_UserWidget
{
	GENERATED_BODY()
	
public:
	void SetQuestData(const FDataTableRowHandle& InQuestData);
	void SetQuestID(const FName& InQuestID);
	
	FName GetQuestID() const { return QuestID; }
	void SetQuestBaseClass(TSubclassOf<AMR_QuestBase> InQuestClass);
	
	UFUNCTION()
	void InitializeQuest() const;
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnDeclineQuest();
	UFUNCTION()
	void OnAcceptQuest();
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_StageDescription;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> QuestGiver_VerticalBox_Objectives;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_QuestGiverTitle;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_QuestDescription;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Decline;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Accept;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "QuestS System")
	FDataTableRowHandle QuestData;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "QuestS System")
	FName QuestID;
	
	UPROPERTY(EditDefaultsOnly, Category = "QuestS System")
	TSubclassOf<UMR_QuestLogEntry_Objective> QuestObjectiveWidgetClass;
	
	UPROPERTY()
	TSubclassOf<AMR_QuestBase> CachedQuestBaseClass;
};
