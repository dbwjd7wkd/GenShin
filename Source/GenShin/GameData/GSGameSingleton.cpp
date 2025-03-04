// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/GSGameSingleton.h"

DEFINE_LOG_CATEGORY(LOGGSGameSingleton);

UGSGameSingleton::UGSGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/GenShin/GameData/GSCharacterStatTable.GSCharacterStatTable'"));
	if (DataTableRef.Object)
	{
		UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, CharacterStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FGSCharacterStat*>(Value);
			}
		);
	}

	CharacterMaxLevel = CharacterStatTable.Num();
}

UGSGameSingleton& UGSGameSingleton::Get()
{
	UGSGameSingleton* Singleton = Cast<UGSGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LOGGSGameSingleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UGSGameSingleton>();
}
