// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GSCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "GSCharacterNonePlayer.generated.h"

/**
 * 
 */
UCLASS(config = ArenaBattle)
class GENSHIN_API AGSCharacterNonePlayer : public AGSCharacterBase
{
	GENERATED_BODY()
	
public:
	AGSCharacterNonePlayer();

protected:
	virtual void PostInitializeComponents() override;

protected:
	void SetDead() override;
	void NPCMeshLoadCompleted();

	UPROPERTY(config)
	TArray<FSoftObjectPath> NPCMeshes;

	TSharedPtr<FStreamableHandle> NPCMeshHandle;

};
