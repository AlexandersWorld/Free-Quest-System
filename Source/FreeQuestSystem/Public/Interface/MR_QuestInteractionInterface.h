// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MR_QuestInteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UMR_QuestInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IMR_QuestInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction")
	FName LookAt();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction")
	void UnLookAt();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction")
	FName InteractWith();
};
