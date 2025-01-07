// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/GSCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/GSCharacterControlData.h"
#include "Animation/AnimMontage.h"
#include "Character/GSComboActionData.h"
#include "Physics/GSCollision.h"
#include "Engine/DamageEvents.h"
#include "CharacterStat/GSCharacterStatComponent.h"
#include "UI/GSWidgetComponent.h"
#include "UI/GSHpBarWidget.h"
#include "CharacterStat/GSCharacterStatComponent.h"

// Sets default values
AGSCharacterBase::AGSCharacterBase()
{
	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_GSCAPSULE);

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
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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

	// Combo Attack
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/GenShin/Animation/AM_ComboAttack.AM_ComboAttack'"));
	if (ComboActionMontageRef.Object)
	{
		ComboAttackMontage = ComboActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UGSComboActionData> ComboActionDataRef(TEXT("/Script/GenShin.GSComboActionData'/Game/GenShin/CharacterActionData/GSA_ComboAttack.GSA_ComboAttack'"));
	if (ComboActionDataRef.Object)
	{
		ComboAttackData = ComboActionDataRef.Object;
	}

	// Attack Hit
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/GenShin/Animation/AM_Dead.AM_Dead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	// Stat Component
	Stat = CreateDefaultSubobject<UGSCharacterStatComponent>(TEXT("Stat"));

	// Widget Component
	HpBar = CreateDefaultSubobject<UGSWidgetComponent>(TEXT("HpBar"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/GenShin/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AGSCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &AGSCharacterBase::SetDead);
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
	// 공격중이 아니라면 몽타주 플레이 시작
	if (CurrentCombo == 0)
	{
		AttackMontageBegin();
		return;
	}

	// 공격중이고, 콤보 유효시간 안이라면 다음 섹션 플레이
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
	// 정상 속도로 진행했을 때 소요될 시간(발동할 시간) 계산 ex) 17/30/1
	float ComboEffectiveTime = (ComboAttackData->EffectiveFrameCount[ComboIndex] / ComboAttackData->FrameRate) / AttackSpeedRate;
	if (ComboEffectiveTime > 0.0f)
	{
		// ComboEffectiveTime 시간만큼 지나면 ComboCheck함수 한 번만 발동 되도록 타이머 설정.
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AGSCharacterBase::ComboCheck, ComboEffectiveTime, false);
	}
}

void AGSCharacterBase::ComboCheck()
{
	ComboTimerHandle.Invalidate();
	// 콤보타이머가 다 됐을 때 실행됨. 콤보입력이 들어와 있다면 바로 다음 섹션 플레이
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

void AGSCharacterBase::AttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this); // 식별자태그, 복잡한 충돌체도 감지할 건지, 무시할 액터

	const float AttackRange = 40.0f;
	const float AttackRadius = 50.0f;
	const float AttackDamage = 30.0f;

	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;
	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_GSACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

#if ENABLE_DRAW_DEBUG
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);

#endif
}

float AGSCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	SetDead();

	return DamageAmount;
}

void AGSCharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);
	HpBar->SetHiddenInGame(true);
}

void AGSCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage, 1.0f);
}

void AGSCharacterBase::SetupCharacterWidget(class UGSUserWidget* InUserWidget)
{
	// HpbarWidget Section
	UGSHpBarWidget* GSHpBarWidget = Cast<UGSHpBarWidget>(InUserWidget);
	if (GSHpBarWidget)
	{
		GSHpBarWidget->SetMaxHp(Stat->GetMaxHp());
		GSHpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
		Stat->OnHpChanged.AddUObject(GSHpBarWidget, &UGSHpBarWidget::UpdateHpBar);
	}

}
