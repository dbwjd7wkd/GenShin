// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/GSItemData.h"
#include "GSPotionItemData.generated.h"

/**
 * 
 */
UCLASS()
class GENSHIN_API UGSPotionItemData : public UGSItemData
{
	GENERATED_BODY()

public:
	UGSPotionItemData();

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("GSItemData", GetFName());
	}

public:
	UPROPERTY(EditAnywhere, Category = Hp)
	float HealAmount;

};
