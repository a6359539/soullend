// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "dbGameMode.generated.h"


UCLASS(minimalapi)
class AdbGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AdbGameMode();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int		rank;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int		score;
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		//FSTRING   name;

	UFUNCTION(BlueprintCallable)
		void CallDataBase();

	UFUNCTION(BlueprintImplementableEvent)
		void widget(int ran,FName name,int sco);
};



