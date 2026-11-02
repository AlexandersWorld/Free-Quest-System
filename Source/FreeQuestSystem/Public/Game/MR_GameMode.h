// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MR_GameMode.generated.h"

/**
 *  Simple GameMode for a first person game
 */
class AMR_PlayerCharacter;
class AInv_PlayerController;

UCLASS(abstract)
class AMR_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMR_GameMode();
	
	virtual void RestartPlayer(AController* NewPlayer) override;
	
protected:
	virtual void BeginPlay() override;
};



