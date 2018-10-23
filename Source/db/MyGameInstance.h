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

USTRUCT(BlueprintType)
struct FMyItemTable
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> itemnum;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> amount;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> enhance;
};




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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FVector	location;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int	MAINLEVEL;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int LEVELEXP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FName nickname;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FText text;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> USEWEAPONLEVEL;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> USEWEAPONLEVELEXP;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FMyItemTable itemtable;


	UFUNCTION(BlueprintImplementableEvent)
		void firstsetting();

	UFUNCTION(BlueprintImplementableEvent)
		void secondsetting();

	UFUNCTION(BlueprintCallable)
		bool saveinventory();


	UFUNCTION(BlueprintCallable)
		bool checkpossibletext(FString Tempstring );

	UFUNCTION(BlueprintCallable)
		bool Trycreatenickname(FString Tempstring);

	UFUNCTION(BlueprintImplementableEvent)
		void afternickname(int type);

	UFUNCTION(BlueprintImplementableEvent)
		void loadinventory();
};
