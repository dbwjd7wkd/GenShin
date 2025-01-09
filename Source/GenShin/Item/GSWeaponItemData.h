// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/GSItemData.h"
#include "GSWeaponItemData.generated.h"

/**
 * 
 */
UCLASS()
class GENSHIN_API UGSWeaponItemData : public UGSItemData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;

};
