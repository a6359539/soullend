// Fill out your copyright notice in the Description page of Project Settings.
#define WIN32_LEAN_AND_MEAN  
#define WINDOW_CLASS_NAME L"WINXCLASS"
#define MAKEINTRESOURCE  MAKEINTRESOURCEW
#define	WM_SOCKET				WM_USER + 1
#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS



#include <WinSock2.h>
#pragma comment (lib, "ws2_32.lib")
#pragma once
#include <windows.h>  
#include <windowsx.h>
#include <vector>
#include <thread>
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "protocol.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
using namespace std;

UCLASS()
class DB_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UMyGameInstance();
	HWND  hWnd;
	bool connect;
	vector <thread *> worker_threads;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString	ID;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString		PASSWORD;
	UFUNCTION(BlueprintCallable)
		bool connectserver();
	UFUNCTION(BlueprintCallable)
		bool destroyend();
	UFUNCTION(BlueprintCallable)
		bool trylogin(APlayerController* pc);
	UFUNCTION(BlueprintImplementableEvent)
		void failconnectserver();

	UFUNCTION(BlueprintImplementableEvent)
		void loginwidgetcontroll(int type, APlayerController* pc);

	UFUNCTION(BlueprintImplementableEvent)
		void connectconsoltext(int offset);


	UFUNCTION(BlueprintImplementableEvent)
		void testconnectconsoltext(FName nameE);

	UFUNCTION(BlueprintCallable)
		bool connectdedicateserver(int typeserver);
	
	UFUNCTION(BlueprintCallable)
		bool trylistenservercreate();

};
