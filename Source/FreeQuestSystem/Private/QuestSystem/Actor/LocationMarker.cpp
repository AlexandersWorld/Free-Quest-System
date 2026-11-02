// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestSystem/Actor/LocationMarker.h"

#include "FreeQuestSystemCharacter.h"
#include "FreeQuestSystemPlayerController.h"
#include "Components/BoxComponent.h"
#include "Components/MR_InteractionComponent.h"
#include "Components/MR_QuestLogComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/QuestSystem/MR_LocationNotify.h"

ALocationMarker::ALocationMarker()
{
	PrimaryActorTick.bCanEverTick = false;
	CollisionBox = CreateDefaultSubobject<UBoxComponent>("CollisionBox");
	SetRootComponent(CollisionBox);
	
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ALocationMarker::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (bHasTriggered) return;

	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController) return;
	
	const APawn* PlayerCharacter = PlayerController->GetPawn();
	if (!PlayerCharacter || OtherActor != PlayerCharacter) return;

 	UMR_QuestLogComponent* QuestLogComponent = PlayerCharacter->FindComponentByClass<UMR_QuestLogComponent>(); 	
	if (!QuestLogComponent) return;

	if (!QuestLogComponent->QueryActiveQuest(QuestName)) return;

	const UMR_InteractionComponent* InteractionComponent = PlayerCharacter->FindComponentByClass<UMR_InteractionComponent>();
	if (!InteractionComponent) return;

	bHasTriggered = true;
	InteractionComponent->OnObjectiveIDCalled.Broadcast(ObjectiveID);
	SpawnLocationNotify(LocationName);
}

void ALocationMarker::SpawnLocationNotify(const FName& InLocationName) const
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (UMR_LocationNotify* QuestWidgetLocationNotify = CreateWidget<UMR_LocationNotify>(PlayerController, QuestLocationNotifyWidgetClass))
		{
			QuestWidgetLocationNotify->SetLocationName(InLocationName);
			QuestWidgetLocationNotify->AddToViewport();
		}
	}
}

