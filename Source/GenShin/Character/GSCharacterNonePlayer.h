// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GSCharacterBase.h"
#include "GSCharacterNonePlayer.generated.h"

/**
 * 
 */
UCLASS()
class GENSHIN_API AGSCharacterNonePlayer : public AGSCharacterBase
{
	GENERATED_BODY()
	
public:
	AGSCharacterNonePlayer();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dead)
	float DeadEventDelayTime = 5.0f;

	virtual void SetDead() override;

};
