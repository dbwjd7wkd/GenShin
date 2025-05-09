// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/GSItemData.h"
#include "GameData/GSCharacterStat.h"
#include "GSWeaponItemData.generated.h"

/**
 * 
 */
UCLASS()
class GENSHIN_API UGSWeaponItemData : public UGSItemData
{
	GENERATED_BODY()

public:
	UGSWeaponItemData();

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("GSItemData", GetFName());
	}
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;

	UPROPERTY(EditAnywhere, Category = Stat)
	FGSCharacterStat ModifierStat;
};
