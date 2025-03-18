// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GSCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/GSCharacterControlData.h"
#include "UI/GSHUDWidget.h"
#include "CharacterStat/GSCharacterStatComponent.h"
#include "Player/GSPlayerController.h"

AGSCharacterPlayer::AGSCharacterPlayer()
{
	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Input
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionChangeControlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/GenShin/Input/Actions/IA_ChangeControl.IA_ChangeControl'"));
	if (InputActionChangeControlRef.Object)
	{
		ChangeControlAction = InputActionChangeControlRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/GenShin/Input/Actions/IA_Jump.IA_Jump'"));
	if (InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/GenShin/Input/Actions/IA_ShoulderLook.IA_ShoulderLook'"));
	if (InputActionShoulderLookRef.Object)
	{
		SoulderLookAction = InputActionShoulderLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/GenShin/Input/Actions/IA_ShoulderMove.IA_ShoulderMove'"));
	if (InputActionShoulderMoveRef.Object)
	{
		SoulderMoveAction = InputActionShoulderMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/GenShin/Input/Actions/IA_QuaterMove.IA_QuaterMove'"));
	if (InputActionQuaterMoveRef.Object)
	{
		QuaterMoveAction = InputActionQuaterMoveRef.Object;
	}

	CurrentCharacterControlType = ECharacterControlType::Quater;

	static ConstructorHelpers::FObjectFinder<UInputAction> AttackActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/GenShin/Input/Actions/IA_Attack.IA_Attack'"));
	if (AttackActionRef.Object)
	{
		AttackAction = AttackActionRef.Object;
	}

}

void AGSCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	AGSPlayerController* PlayerController = Cast<AGSPlayerController>(GetController());
	if (PlayerController)
	{
		EnableInput(PlayerController);
	}

	SetCharacterControl(CurrentCharacterControlType);
}

void AGSCharacterPlayer::SetDead()
{
	AGSPlayerController* PlayerController = Cast<AGSPlayerController>(GetController());
	if (PlayerController)
	{
		DisableInput(PlayerController);
	}
}

void AGSCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ������Ʈ ���ÿ��� EnhancedInputComponent�� ����Ѵٰ� �����߱⶧����, ������ �ʴ� ��쿣 ������ �߻��ϵ��� CastChecked �Լ� ���.
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &AGSCharacterPlayer::ChangeCharacterControl);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(SoulderLookAction, ETriggerEvent::Triggered, this, &AGSCharacterPlayer::SoulderLook);
	EnhancedInputComponent->BindAction(SoulderMoveAction, ETriggerEvent::Triggered, this, &AGSCharacterPlayer::SoulderMove);
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &AGSCharacterPlayer::QuaterMove);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AGSCharacterPlayer::Attack);
}

void AGSCharacterPlayer::ChangeCharacterControl()
{
	if (CurrentCharacterControlType == ECharacterControlType::Quater)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	else if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quater);
	}
}

void AGSCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	// ���ο� Ÿ���� �� ������ ������Ʈ ������
	UGSCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);
	// ������ �� ������ ������Ʈ�� ����
	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		// ������ �� �������� ��ǲ�������ؽ�Ʈ ����
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}
	// ���� �� Ÿ�� ����
	CurrentCharacterControlType = NewCharacterControlType;
}

void AGSCharacterPlayer::SetCharacterControlData(const class UGSCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

void AGSCharacterPlayer::SoulderLook(const FInputActionValue& Value)
{
	// FInputActionValue���� ���콺�� XY���� ������ �� ��Ʈ�ѷ��� ȸ���� ���������ν�, ���������� �ش� ��Ʈ�ѷ��� �ٶ󺸵��� ����.
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AGSCharacterPlayer::SoulderMove(const FInputActionValue& Value)
{
	// FInputActionValue���� Ű������ XY���� ������ ��, �̰͵��� �����Ʈ ������Ʈ�� �����ؼ� ���������� ĳ���͸� �̵�.

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// ��ǲ�׼� Modifiers �ɼǿ� "Swizzle Input Axis Values" �߰��ϸ�, XY �ٲ㾲�� �ʾƵ� ��.
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	FVector2D MovementVector = Value.Get<FVector2D>();
	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AGSCharacterPlayer::QuaterMove(const FInputActionValue& Value)
{
	// FVector2D: Ű���� XY��
	FVector2D MovementVector = Value.Get<FVector2D>();

	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.0f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	// ��ǲ�׼� Modifiers �ɼǿ� "Swizzle Input Axis Values" �߰��ϸ�, XY �ٲ㾲�� �ʾƵ� ��.
	FVector MoveDirection = FVector(MovementVector.Y, MovementVector.X, 0.0f);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, MovementVectorSize);
}

void AGSCharacterPlayer::Attack()
{
	ProcessComboActionCommand();
}

void AGSCharacterPlayer::SetupHUDWidget(UGSHUDWidget* InHUDWidget)
{
	// ������ �����͸� �Ѱ��ְ� ���ȿ� �ִ� ��������Ʈ�� ���ε� ������.
	if (InHUDWidget)
	{
		InHUDWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
		InHUDWidget->UpdateHpBar(Stat->GetCurrentHp());

		Stat->OnStatChanged.AddUObject(InHUDWidget, &UGSHUDWidget::UpdateStat);
		Stat->OnHpChanged.AddUObject(InHUDWidget, &UGSHUDWidget::UpdateHpBar);
	}
}

