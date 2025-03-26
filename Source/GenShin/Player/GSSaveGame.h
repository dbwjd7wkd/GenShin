// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GSSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class GENSHIN_API UGSSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UGSSaveGame();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	int32 RetryCount;
};
