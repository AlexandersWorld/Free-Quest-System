// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MR_GameInstance.generated.h"

class UMR_SaveGame;
/**
 * 
 */
UCLASS()
class UMR_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UMR_GameInstance();
	
	UFUNCTION(BlueprintCallable, Category = "Save System")
	bool SaveGame() const;
	
	UFUNCTION(BlueprintCallable, Category = "Save System")
	bool LoadGame();
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Save System")
	FString SaveSLotName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Save System")	
	int32 SaveUserIndex;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Save System")	
	TSubclassOf<UMR_SaveGame> SaveGameClass;
protected:
	UMR_SaveGame* GetOrCreateSaveGameObject() const;
};
