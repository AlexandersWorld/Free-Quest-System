// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class UTextBlock;

#include "CoreMinimal.h"
#include "Widgets/MR_UserWidget.h"
#include "MR_QuestNotification.generated.h"

/**
 * 
 */
UCLASS()
class UMR_QuestNotification : public UMR_UserWidget
{
	GENERATED_BODY()
	
public:
	void SetObjectiveDescription(const FText& InObjectiveDescription);
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_ObjectiveDescription;
	
	UPROPERTY()
	FText ObjectiveDescription;
};
