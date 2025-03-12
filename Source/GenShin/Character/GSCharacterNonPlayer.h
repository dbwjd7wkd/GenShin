// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GSCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "Interface/GSCharacterAIInterface.h"
#include "GSCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS(config = ArenaBattle)
class GENSHIN_API AGSCharacterNonPlayer : public AGSCharacterBase, public IGSCharacterAIInterface
{
	GENERATED_BODY()
	
public:
	AGSCharacterNonPlayer();

protected:
	virtual void PostInitializeComponents() override;

protected:
	void SetDead() override;
	void NPCMeshLoadCompleted();

	UPROPERTY(config)
	TArray<FSoftObjectPath> NPCMeshes;

	TSharedPtr<FStreamableHandle> NPCMeshHandle;

	// AI Section
protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;

	FAICharacterAttackFinished OnAttackFinished;

	// ÄÞº¸°ø°Ý ³¡³¯ ¶§ ½ÇÇàµÊ.
	virtual void NotifyComboActionEnd() override;

};
