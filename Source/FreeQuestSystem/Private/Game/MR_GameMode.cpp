// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game/MR_GameMode.h"
#include "Game/MR_GameInstance.h"
#include "Kismet/GameplayStatics.h"

AMR_GameMode::AMR_GameMode()
{
}

void AMR_GameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AMR_GameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
	
	if (const APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (PC->IsLocalController())
		{
			if (UMR_GameInstance* GameInstance = GetGameInstance<UMR_GameInstance>())
			{
				GameInstance->LoadGame();
			}
		}
	}
}
