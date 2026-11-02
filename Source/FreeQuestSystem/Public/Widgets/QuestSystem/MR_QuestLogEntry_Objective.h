#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestSystem/Types/QuestTypes.h"
#include "MR_QuestLogEntry_Objective.generated.h"

class UTextBlock;
class UCheckBox;
class AMR_QuestBase;

UCLASS()
class UMR_QuestLogEntry_Objective : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetObjectiveData(const FObjectiveDetails& InObjectiveData);
	
	void SetQuestActor(AMR_QuestBase* InQuestActor);

protected: 
	virtual void NativeConstruct() override;

private:
	void UpdateUI() const;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_QuestDescription;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> CheckBox_IsCompleted;

	FObjectiveDetails ObjectiveData;
	TWeakObjectPtr<AMR_QuestBase> QuestActor;
};