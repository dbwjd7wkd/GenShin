// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GSCharacterNonPlayer.h"
#include "Engine/AssetManager.h"
#include "AI/GSAIController.h"
#include "CharacterStat/GSCharacterStatComponent.h"

AGSCharacterNonPlayer::AGSCharacterNonPlayer()
{
	GetMesh()->SetHiddenInGame(true);

	AIControllerClass = AGSAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AGSCharacterNonPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ensure(NPCMeshes.Num() > 0);
	int32 RandIndex = FMath::RandRange(0, NPCMeshes.Num() - 1);
	NPCMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(NPCMeshes[RandIndex], FStreamableDelegate::CreateUObject(this, &AGSCharacterNonPlayer::NPCMeshLoadCompleted));
}

void AGSCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
}

void AGSCharacterNonPlayer::NPCMeshLoadCompleted()
{
	if (NPCMeshHandle.IsValid())
	{
		USkeletalMesh* NPCMesh = Cast<USkeletalMesh>(NPCMeshHandle->GetLoadedAsset());
		if (NPCMesh)
		{
			GetMesh()->SetSkeletalMesh(NPCMesh);
			GetMesh()->SetHiddenInGame(false);
		}
	}

	NPCMeshHandle->ReleaseHandle();
}

float AGSCharacterNonPlayer::GetAIPatrolRadius()
{
	return 800.0f;
}

float AGSCharacterNonPlayer::GetAIDetectRange()
{
	return 400.0f;
}

float AGSCharacterNonPlayer::GetAIAttackRange()
{
	return Stat->GetTotalStat().AttackRange + Stat->GetAttackRadius() * 2;
}

float AGSCharacterNonPlayer::GetAITurnSpeed()
{
	return 2.0f;
}

void AGSCharacterNonPlayer::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AGSCharacterNonPlayer::AttackByAI()
{
	ProcessComboActionCommand();
}

void AGSCharacterNonPlayer::NotifyComboActionEnd()
{
	Super::NotifyComboActionEnd();
	OnAttackFinished.ExecuteIfBound();
}
