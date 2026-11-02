// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LocationMarker.generated.h"

class UBoxComponent;
class UMR_LocationNotify;

UCLASS()
class ALocationMarker : public AActor
{
	GENERATED_BODY()

public:
	ALocationMarker();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	void SpawnLocationNotify(const FName& InLocationName) const;
private:
	UPROPERTY(EditAnywhere, Category = "Quest System")
	FName QuestName;
	
	UPROPERTY(EditAnywhere, Category = "Quest System")
	FName ObjectiveID;
	
	UPROPERTY(EditAnywhere, Category = "Quest System")
	FName LocationName;
	
	UPROPERTY(VisibleAnywhere, Category = "Quest System")
	TObjectPtr<UBoxComponent> CollisionBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest System", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UMR_LocationNotify> QuestLocationNotifyWidgetClass;
	
	UPROPERTY()
	bool bHasTriggered {false};
};
