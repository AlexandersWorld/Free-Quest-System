#include "Game/MR_GameInstance.h"

#include "Components/MR_QuestLogComponent.h"
#include "Game/MR_SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "QuestSystem/MR_QuestBase.h"

UMR_GameInstance::UMR_GameInstance()
{
	SaveSLotName = TEXT("PlayerSaveSlot");
	SaveUserIndex = 0;
}

UMR_SaveGame* UMR_GameInstance::GetOrCreateSaveGameObject() const
{
	if (!SaveGameClass)
	{
		UE_LOG(LogTemp, Error, TEXT("[GameInstance] SaveGameClass is not assigned! Set it in your GI Blueprint details."));
		return nullptr;
	}
	
	if (UGameplayStatics::DoesSaveGameExist(SaveSLotName, SaveUserIndex))
	{
		return Cast<UMR_SaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSLotName, SaveUserIndex));
	}
	
	return Cast<UMR_SaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
}

bool UMR_GameInstance::SaveGame() const
{
	// 1. Create a fresh save game object based on your SaveGameClass blueprint
	UMR_SaveGame* SaveObject = GetOrCreateSaveGameObject();
	if (!IsValid(SaveObject)) 
	{
		UE_LOG(LogTemp, Error, TEXT("[GameInstance] Failed to create Save Game Object!"));
		return false;
	}
    
	// 2. Fetch the player character and quest log
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController) return false;
	
	if (const APawn* PlayerCharacter = PlayerController->GetPawn())
	{
		if (UMR_QuestLogComponent* QuestLogComponent = PlayerCharacter->FindComponentByClass<UMR_QuestLogComponent>())
		{
			// Clear lists in the save object
			SaveObject->CurrentActiveQuests.Empty();
			SaveObject->QuestProgress.Empty();

			// Save active quest IDs
			const TArray<FName>& ActiveQuests = QuestLogComponent->GetCurrentActiveQuests();
			for (const FName& ActiveQuest : ActiveQuests)
			{
				SaveObject->CurrentActiveQuests.Add(ActiveQuest);
			}

			// Save individual quest actor stages and progress
			const TArray<TObjectPtr<AMR_QuestBase>>& CurrentQuestActors = QuestLogComponent->GetCurrentQuestActors();
			for (const TObjectPtr<AMR_QuestBase>& QuestActor : CurrentQuestActors)
			{
				if (IsValid(QuestActor))
				{
					SaveObject->SaveQuestDetails(QuestActor);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning,
				TEXT("FreeQuestSystem: Missing QuestLogComponent on Player Pawn."));

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,
					5.0f,
					FColor::Red,
					TEXT("FreeQuestSystem: Add a QuestLogComponent to your Player Pawn!")
				);
			}
		}
	}

	// 3. Write to slot
	const bool bSuccess = UGameplayStatics::SaveGameToSlot(SaveObject, SaveSLotName, SaveUserIndex);
	if (bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("[GameInstance] Game saved successfully to slot: %s"), *SaveSLotName);
	}
	return bSuccess;
}

bool UMR_GameInstance::LoadGame()
{
	if (!UGameplayStatics::DoesSaveGameExist(SaveSLotName, SaveUserIndex)) return false;
	
	UMR_SaveGame* SaveObject = GetOrCreateSaveGameObject();
	if (!IsValid(SaveObject)) return false;
	
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController) return false;
	
	if (const APawn* PlayerCharacter = PlayerController->GetPawn())
	{
		if (UMR_QuestLogComponent* QuestLogComponent = PlayerCharacter->FindComponentByClass<UMR_QuestLogComponent>())
		{
			SaveObject->LoadQuests(QuestLogComponent, GetWorld());
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Warning,
				TEXT("FreeQuestSystem: Missing QuestLogComponent on Player Pawn."));

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,
					5.0f,
					FColor::Red,
					TEXT("FreeQuestSystem: Add a QuestLogComponent to your Player Pawn!")
				);
			}
		}
	}
	return false;
}
