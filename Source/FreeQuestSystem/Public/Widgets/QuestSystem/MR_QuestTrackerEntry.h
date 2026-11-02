// MR_QuestTrackerEntry.h
#pragma once

#include "CoreMinimal.h"
#include "Widgets/MR_UserWidget.h"
#include "MR_QuestTrackerEntry.generated.h"

class UTextBlock;
class UVerticalBox;
class AMR_QuestBase;
class UMR_QuestLogEntry_Objective;

UCLASS()
class UMR_QuestTrackerEntry : public UMR_UserWidget
{
	GENERATED_BODY()

public:
	AMR_QuestBase* GetQuestActor() const { return QuestActor; }
	void SetQuestActor(AMR_QuestBase* InQuestActor);

	void InitializeQuestEntry(AMR_QuestBase* InQuestActor);
	void UpdateQuestEntry(const AMR_QuestBase* InQuestActor);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_QuestName;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox_Objectives;

	UPROPERTY()
	TObjectPtr<AMR_QuestBase> QuestActor;

	UPROPERTY(EditDefaultsOnly, Category = "Quest System")
	TSubclassOf<UMR_QuestLogEntry_Objective> QuestObjectiveWidgetClass;

	UPROPERTY()
	TArray<TObjectPtr<UMR_QuestLogEntry_Objective>> ObjectiveWidgets;
};