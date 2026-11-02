#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Engine/DataTable.h"
#include "QuestSystem/Types/QuestTypes.h"
#include "MR_SaveGame.generated.h"

class AMR_QuestBase;
class UMR_QuestLogComponent;

UCLASS()
class  UMR_SaveGame : public USaveGame
{
	GENERATED_BODY()
    
public:
	void SaveQuestDetails(const TObjectPtr<AMR_QuestBase> QuestActor);
    
	void LoadQuests(UMR_QuestLogComponent* QuestLogComponent, UWorld* WorldContext);

public:
	UPROPERTY(VisibleDefaultsOnly, Category = "Quest System")
	TArray<FName> CurrentActiveQuests;
    
	UPROPERTY(VisibleDefaultsOnly, Category = "Quest System")
	TArray<FName> CompletedQuests;
    
	UPROPERTY(VisibleDefaultsOnly, Category = "Quest System")
	TMap<FName, FQuestSaveData> QuestProgress;
    
	UPROPERTY(EditAnywhere, Category = "Quest System")
	TObjectPtr<UDataTable> QuestDataTable;
    
	UPROPERTY(EditAnywhere, Category = "Quest System")
	TSubclassOf<AMR_QuestBase> QuestBaseClass;
};