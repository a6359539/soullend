// Fill out your copyright notice in the Description page of Project Settings.
#define WIN32_LEAN_AND_MEAN  
#define WINDOW_CLASS_NAME L"WINXCLASS"
#define MAKEINTRESOURCE  MAKEINTRESOURCEW
#define	WM_SOCKET				WM_USER + 1
#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#pragma comment (lib, "ws2_32.lib")
#include <windows.h>  
#include <windowsx.h>
#include <vector>
#include <thread>
#include <stdlib.h>
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "protocol.h"
#include "loginGameModeBase.generated.h"


UCLASS(minimalapi)
class AloginGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AloginGameModeBase();
	HWND  hWnd;
	bool connect;
	UFUNCTION(BlueprintCallable)
		bool connectserver();
	UFUNCTION(BlueprintCallable)
		bool destroyend();


	UFUNCTION(BlueprintImplementableEvent)
		void failconnectserver();
};


