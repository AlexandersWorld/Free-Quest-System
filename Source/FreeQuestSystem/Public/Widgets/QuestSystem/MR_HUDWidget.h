// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/MR_UserWidget.h"
#include "MR_HUDWidget.generated.h"

class UMR_InfoMessage;

/**
 * 
 */
UCLASS()
class UMR_HUDWidget : public UMR_UserWidget
{
	GENERATED_BODY()
	
public:
	void ShowInteractionMessage(const FName& Message) const;
	void HideInteractionMessage() const;
protected:
	virtual void NativeOnInitialized() override;
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMR_InfoMessage> InfoMessage;
};
