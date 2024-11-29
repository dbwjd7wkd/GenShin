// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GSCharacterNonePlayer.h"

AGSCharacterNonePlayer::AGSCharacterNonePlayer()
{

}

void AGSCharacterNonePlayer::SetDead()
{
	Super::SetDead();

	// ������ 5�� �� �����
	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
}
