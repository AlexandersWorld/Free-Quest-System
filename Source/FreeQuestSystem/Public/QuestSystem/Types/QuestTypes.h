#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "QuestTypes.generated.h"

UENUM(BlueprintType)
enum class EQuestType : uint8
{
	Location,
	Kill,
	Interact,
	Collect
};

USTRUCT(BlueprintType)
struct FObjectiveDetails
{
	GENERATED_BODY()
	
	FObjectiveDetails()
	: Type(EQuestType::Interact)
	, Quantity(0)
	, IsOptional(false)
	, BonusXP(0)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsOptional;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BonusXP;
};

USTRUCT(BlueprintType)
struct FStageDetails
{
	GENERATED_BODY()
	
    FStageDetails()
        : XPReward(0)
        , CurrencyRewards(0)
    {
    }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FObjectiveDetails> Objectives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 XPReward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, int32> ItemRewards;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrencyRewards;
};

USTRUCT(BlueprintType)
struct FQuestDetails : public FTableRowBase
{
	GENERATED_BODY()
	
	FQuestDetails()
	: bMain(true), bRequiresTurnIn(true)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText LogDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText TrackingDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMain;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRequiresTurnIn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FStageDetails> StageDetails;
	
	bool IsValid() const { return !Name.IsEmpty(); }
};

USTRUCT(BlueprintType)
struct FQuestSaveData
{
	GENERATED_BODY()
	
	FQuestSaveData()		
	: Progress(), 
	CurrentStage(0)
	{
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, int32> Progress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentStage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FQuestDetails Details;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest System")
	FDataTableRowHandle QuestRowHandle;
};