// Fill out your copyright notice in the Description page of Project Settings.

#include "CPPbasicChar.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"
#include "EngineGlobals.h"
#include "Engine/Engine.h"
#include "UnrealNetwork.h"

// Sets default values
ACPPbasicChar::ACPPbasicChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//카메라 스프링 암 생성.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	//스프링 암 위치 조정
	SpringArm->SetRelativeLocation(FVector(0, 30, 70));

	//스프링 암 길이 조정
	SpringArm->TargetArmLength = 200.0f;

	//카메라 생성
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	//스프링암 절대 회전으로 회전 시킴
	SpringArm->bUsePawnControlRotation = true;

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	/*static ConstructorHelpers::FClassFinder<UAnimInstance> Anim_Class(TEXT("AnimBlueprint'/Game/RPG/CPP/CPP_AB_BasicChar.CPP_AB_BasicChar_C'"));
	if (Anim_Class.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(Anim_Class.Class);
	}*/
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	bReplicates = true;

}

// Called when the game starts or when spawned
void ACPPbasicChar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPPbasicChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//FVector TraceStart
	UWorld* world = GetWorld();
	if (world)
	{
		APlayerController* pc = world->GetFirstPlayerController();
		FVector2D mousePos = FVector2D(0, 0);
		FVector worldpos = FVector(mousePos.X, mousePos.Y, 0);
		FVector dir = FVector(0, 0, 0);
		pc->GetMousePosition(mousePos.X, mousePos.Y);
		pc->DeprojectMousePositionToWorld(worldpos, dir);
		FString testString = "loc " + worldpos.ToString() + " dir " + dir.ToString();
		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, testString);
	}
	

	/*bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
		TraceStart,
		TraceEnd,
		)*/

}
// Called to bind functionality to input
void ACPPbasicChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ACPPbasicChar::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ACPPbasicChar::LookUp);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACPPbasicChar::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACPPbasicChar::MoveForward);

}


void ACPPbasicChar::Turn(float Value) {
	if (Value != 0.0)
	{
		AddControllerYawInput(Value);
	}
}

void ACPPbasicChar::LookUp(float Value) {
	if (Value != 0.0) {
		AddControllerPitchInput(Value);
	}
}
void ACPPbasicChar::MoveRight(float Value) {
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
void ACPPbasicChar::MoveForward(float Value) {
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}
/*
void ACPPbasicChar::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ACPPbasicChar, ReplicatedMovement, COND_AutonomousOnly);
}

void ACPPbasicChar::PreReplication(IRepChangedPropertyTracker & ChangedPropertyTracker)
{
	DOREPLIFETIME_ACTIVE_OVERRIDE(ACPPbasicChar, ReplicatedMovement, bReplicateMovement);
}*/