// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/MR_UserWidget.h"
#include "MR_QuestLog_QuestEntry.generated.h"

class UTextBlock;
class UButton;
class UCheckBox;
class AMR_QuestBase;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnQuestSelected, const FName& /* QuestID */, AMR_QuestBase* /* QuestActor */);

/**
 * 
 */
UCLASS()
class UMR_QuestLog_QuestEntry : public UMR_UserWidget
{
	GENERATED_BODY()
	
public:
	void SetQuestID(const FName& InQuestID);
	void SetQuestData(const FDataTableRowHandle& InQuestData);
	void SetQuestActor(AMR_QuestBase* InQuestActor);
	
	FName GetQuestID() const { return QuestID; }
	TObjectPtr<AMR_QuestBase> GetQuestActor() const { return QuestActor; }
	
	FOnQuestSelected QuestSelected;
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_QuestName;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_CheckBox;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCheckBox> CheckBox_IsCompleted;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "QuestS System")
	TObjectPtr<AMR_QuestBase> QuestActor;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "QuestS System")
	FName QuestID;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "QuestS System")
	FDataTableRowHandle QuestData;
	
	UFUNCTION()
	void HandleButtonClicked();
	void IsQuestCompleted() const;
};
