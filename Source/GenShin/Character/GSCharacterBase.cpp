// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GSCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/GSCharacterControlData.h"
#include "Animation/AnimMontage.h"
#include "Character/GSComboActionData.h"

// Sets default values
AGSCharacterBase::AGSCharacterBase()
{
	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/GenShin/Animation/ABP_GSCharacter.ABP_GSCharacter_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	// CharacterControlData
	ConstructorHelpers::FObjectFinder<UGSCharacterControlData> ShoulderDataRef(TEXT("/Script/GenShin.GSCharacterControlData'/Game/GenShin/CharacterControlData/GSC_Shoulder.GSC_Shoulder'"));
	if (ShoulderDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	}

	ConstructorHelpers::FObjectFinder<UGSCharacterControlData> QuaterDataRef(TEXT("/Script/GenShin.GSCharacterControlData'/Game/GenShin/CharacterControlData/GSC_Quater.GSC_Quater'"));
	if (QuaterDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Quater, QuaterDataRef.Object);
	}

}

void AGSCharacterBase::SetCharacterControlData(const UGSCharacterControlData* CharacterControlData)
{
	// Pawn
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	// CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}

void AGSCharacterBase::ProcessComboActionCommand()
{
	// �������� �ƴ϶�� ��Ÿ�� �÷��� ����
	if (CurrentCombo == 0)
	{
		AttackMontageBegin();
		return;
	}

	// �������̰�, �޺� ��ȿ�ð� ���̶�� ���� ���� �÷���
	if (ComboTimerHandle.IsValid())
	{
		HasNextCombo = true;
	}
	else
	{
		HasNextCombo = false;
	}

}

void AGSCharacterBase::AttackMontageBegin()
{
	// Combo Status
	CurrentCombo = 1;

	// Movement Setting
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// Animation Setting
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboAttackMontage);

	FOnMontageEnded OnMontageEnded;
	OnMontageEnded.BindUObject(this, &AGSCharacterBase::AttackMontageEnd);
	AnimInstance->Montage_SetEndDelegate(OnMontageEnded, ComboAttackMontage);

	ComboTimerHandle.Invalidate();
	SetComboTimer();
}

void AGSCharacterBase::AttackMontageEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	check(CurrentCombo != 0);
	CurrentCombo = 0;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AGSCharacterBase::SetComboTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboAttackData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = 1.0f;
	// ���� �ӵ��� �������� �� �ҿ�� �ð�(�ߵ��� �ð�) ��� ex) 17/30/1
	float ComboEffectiveTime = (ComboAttackData->EffectiveFrameCount[ComboIndex] / ComboAttackData->FrameRate) / AttackSpeedRate;
	if (ComboEffectiveTime > 0.0f)
	{
		// ComboEffectiveTime �ð���ŭ ������ ComboCheck�Լ� �� ���� �ߵ� �ǵ��� Ÿ�̸� ����.
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AGSCharacterBase::ComboCheck, ComboEffectiveTime, false);
	}
}

void AGSCharacterBase::ComboCheck()
{
	ComboTimerHandle.Invalidate();
	// �޺�Ÿ�̸Ӱ� �� ���� �� �����. �޺��Է��� ���� �ִٸ� �ٷ� ���� ���� �÷���
	if (HasNextCombo)
	{
		HasNextCombo = false;

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboAttackData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboAttackData->MontageSectionNamePrefix, CurrentCombo);
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_JumpToSection(NextSection, ComboAttackMontage);

		SetComboTimer();
	}
}
