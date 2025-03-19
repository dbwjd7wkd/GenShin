// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/GSItemData.h"
#include "GameData/GSCharacterStat.h"
#include "GSScrollItemData.generated.h"

/**
 * 
 */
UCLASS()
class GENSHIN_API UGSScrollItemData : public UGSItemData
{
	GENERATED_BODY()

public:
	UGSScrollItemData();

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("GSItemData", GetFName());
	}

public:
	UPROPERTY(EditAnywhere, Category = Stat)
	FGSCharacterStat BaseStat;

};
