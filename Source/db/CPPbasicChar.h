// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPPbasicChar.generated.h"

UCLASS()
class DB_API ACPPbasicChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPPbasicChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION()
		void Turn(float Value);

	UFUNCTION()
		void LookUp(float Value);

	UFUNCTION()
		void MoveRight(float Value);

	UFUNCTION()
		void MoveForward(float Value);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UCameraComponent* Camera;
	
};
