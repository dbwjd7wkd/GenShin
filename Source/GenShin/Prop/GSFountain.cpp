// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/GSFountain.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AGSFountain::AGSFountain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 매시 생성 및 계층관계 정리
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Water"));

	RootComponent = Body;
	Water->SetupAttachment(Body);
	Water->SetRelativeLocation(FVector(0.0f, 0.0f, 132.0f));
	
	// 메시 오브젝트 가져와서 셋팅
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/GenShin/Environment/Props/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01'"));
	if (BodyMeshRef.Object)
	{
		Body->SetStaticMesh(BodyMeshRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WaterMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/GenShin/Environment/Props/SM_Plains_Fountain_02.SM_Plains_Fountain_02'"));
	if (WaterMeshRef.Object)
	{
		Water->SetStaticMesh(WaterMeshRef.Object);
	}

}

// Called when the game starts or when spawned
void AGSFountain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGSFountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

