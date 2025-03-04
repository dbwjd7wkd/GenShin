// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameData/GSCharacterStat.h"
#include "GSGameSingleton.generated.h"

/**
 * 
 */
DECLARE_LOG_CATEGORY_EXTERN(LOGGSGameSingleton, Error, All);

UCLASS()
class GENSHIN_API UGSGameSingleton : public UObject
{
	GENERATED_BODY()

public:
	UGSGameSingleton();

	static UGSGameSingleton& Get();

public:
	FORCEINLINE FGSCharacterStat GetCharacterStat(int32 InLevel) { return CharacterStatTable.IsValidIndex(InLevel - 1) ? CharacterStatTable[InLevel - 1] : FGSCharacterStat(); }

	int32 CharacterMaxLevel;

private:
	TArray<FGSCharacterStat> CharacterStatTable;
	
};
