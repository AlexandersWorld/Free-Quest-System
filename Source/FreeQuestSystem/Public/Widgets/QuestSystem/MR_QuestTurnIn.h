// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/MR_UserWidget.h"
#include "MR_QuestTurnIn.generated.h"

class AMR_QuestBase;
class UTextBlock;
class UButton;

UCLASS()
class UMR_QuestTurnIn : public UMR_UserWidget
{
	GENERATED_BODY()
	
public:
    void SetQuestData(const FDataTableRowHandle& InQuestData);
    void SetQuestID(const FName& InQuestID);
    void SetQuestActor(AMR_QuestBase* InQuestActor);
    
    FName GetQuestID() const { return QuestID; }

    UFUNCTION()
    void InitializeQuest() const;
protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

private:
    UPROPERTY(meta=(BindWidget))
    TObjectPtr<UTextBlock> Text_StageDescription;
    
    UPROPERTY(meta=(BindWidget))
    TObjectPtr<UTextBlock> Text_QuestGiverTitle;
    
    UPROPERTY(meta=(BindWidget))
    TObjectPtr<UTextBlock> Text_QuestDescription;
    
    UPROPERTY(meta=(BindWidget))
    TObjectPtr<UButton> Button_Cancel;
    
    UPROPERTY(meta=(BindWidget))
    TObjectPtr<UButton> Button_CollectRewards;
    
    UPROPERTY(VisibleDefaultsOnly, Category = "QuestS System")
    FDataTableRowHandle QuestData;
    
    UPROPERTY(VisibleDefaultsOnly, Category = "QuestS System")
    FName QuestID;
    
    UPROPERTY(VisibleDefaultsOnly, Category = "QuestS System")
    TObjectPtr<AMR_QuestBase> QuestActor;
    
    UFUNCTION()
    void OnCancel();
    
    UFUNCTION()
    void OnGetReward();
};
