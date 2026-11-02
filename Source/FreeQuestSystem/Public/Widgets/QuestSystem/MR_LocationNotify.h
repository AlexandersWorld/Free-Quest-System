// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/MR_UserWidget.h"
#include "MR_LocationNotify.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class UMR_LocationNotify : public UMR_UserWidget
{
	GENERATED_BODY()
public:
	void SetLocationName(const FName& InLocationName) const;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_LocationName;
};
