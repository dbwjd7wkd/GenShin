// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GSItemData.generated.h"

UENUM()
enum class EItemType : uint8
{
	None = 0,
	Weapon,
	Potion,
	Scroll
};

UCLASS()
class GENSHIN_API UGSItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UGSItemData() : Type(EItemType::None) {}
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	EItemType Type;

};
