// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/MR_UserWidget.h"
#include "MR_InfoMessage.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class UMR_InfoMessage : public UMR_UserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void MessageShow();
	UFUNCTION()
	void MessageHide();
	void SetMessage(const FName& Message);
protected:
	virtual void NativeOnInitialized() override;
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Message;
	UPROPERTY(EditAnywhere, Category = "Quest System")
	float MessageLifeTime {3.7};
	FTimerHandle MessageTimer;
	bool bIsMessageActive {false};
};
