// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "dbGameMode.h"
#include "dbCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Windows.h>
#include <sqlext.h>
#include <iostream>


void HandleDiagnosticRecord(SQLHANDLE      hHandle,
	SQLSMALLINT    hType,
	RETCODE        RetCode)
{
	SQLSMALLINT iRec = 0;
	SQLINTEGER  iError;
	WCHAR       wszMessage[1000];
	WCHAR       wszState[SQL_SQLSTATE_SIZE + 1];


	if (RetCode == SQL_INVALID_HANDLE)
	{
		//	fwprintf(stderr, L"Invalid handle!\n");
		return;
	}

	while (SQLGetDiagRec(hType,
		hHandle,
		++iRec,
		wszState,
		&iError,
		wszMessage,
		(SQLSMALLINT)(sizeof(wszMessage) / sizeof(WCHAR)),
		(SQLSMALLINT *)NULL) == SQL_SUCCESS)
	{
		// Hide data truncated..
		if (wcsncmp(wszState, L"01004", 5))
		{
			//	fwprintf(stderr, L"[%5.5s] %s (%d)\n", wszState, wszMessage, iError);
		}
	}

}

AdbGameMode::AdbGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

}


void AdbGameMode::CallDataBase()
	{
		SQLHENV henv;
		SQLHDBC hdbc;
		SQLHSTMT hstmt;
		SQLRETURN retcode;

		wchar_t    char_name[100];
		
		wchar_t buf[100];
		SQLLEN  pIndicators[2];
		switch (1)
		{
		case 1:
			//swprintf_s(buf, L"EXEC dbo.SELECT_HIGHLEVEL %d", 1);
			break;
		case 2:
			//printf("½ÇÇà");
			//	swprintf_s(buf, L"EXEC dbo.SELECT_SAVE %d,%d,%d,%d,%d,%d", clients[key].avatar.x, clients[key].avatar.y, clients[key].avatar.level, clients[key].avatar.exp, clients[key].avatar.hp, clients[key].avatar.id);
			break;
		default:
			break;
		}

		setlocale(LC_ALL, "korean");

		SQLCHAR * OutConnStr = (SQLCHAR *)malloc(255);
		SQLSMALLINT * OutConnStrLen = (SQLSMALLINT *)malloc(255);

		// Allocate environment handle
		retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);

		// Set the ODBC version environment attribute
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
			retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

			// Allocate connection handle
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
				retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

				// Set login timeout to 5 seconds
				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
					SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);

					// Connect to data source
					retcode = SQLConnect(hdbc, (SQLWCHAR*)L"gameserver_2011180001", SQL_NTS, (SQLWCHAR*)L"sa", SQL_NTS, (SQLWCHAR*)L"dsc159951!", SQL_NTS);

					// Allocate statement handle
					if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
						retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

						// Process data
						if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
							UKismetSystemLibrary::PrintString(GetWorld(), TEXT("DataBase Connection Success"), true, true, FLinearColor(1.f, 1.f, 1.f, 1.f), 10.f);
							//	cout << "DataBase Connection Success!\n";
						
							
								swprintf_s(buf, L"EXEC dbo.SELECT_HIGHLEVEL %d", rank);
								retcode = SQLExecDirect(hstmt,
									(SQLWCHAR*)buf,
									SQL_NTS);
								if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
								{
									SQLBindCol(hstmt, 1, SQL_C_WCHAR, (SQLPOINTER)char_name, sizeof(wchar_t[100]),
										&pIndicators[0]);
									SQLBindCol(hstmt, 2, SQL_C_LONG, (SQLPOINTER)&score, sizeof(score),
										&pIndicators[1]);
									while (SQLFetch(hstmt) == SQL_SUCCESS) {
										//if (getID == char_id) {
										for (int i = 0; i < 10; i++)
										{
											//	if (clients[i].avatar.id == char_id) {
											//		errorcode = 2;
											//return false;
										}
									}
									widget(rank, char_name, score);
								
									//strcpy((char*)name, (char*)char_name);


									//	clients[new_id].avatar.x = char_x;
									//	clients[new_id].avatar.y = char_y;
									//	clients[new_id].avatar.id = getID;
									//	clients[new_id].avatar.hp = char_hp;
									//	clients[new_id].avatar.level = char_level;
									//	wcscpy_s(clients[new_id].avatar.name, char_name);
									//	clients[new_id].avatar.exp = char_exp;
									//	wcout << clients[new_id].avatar.x;
								}
						
								// READ DATA
								
							
							
						}
						else {
							HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
						}
						SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
					}

					SQLDisconnect(hdbc);
				}
				else {
					HandleDiagnosticRecord(hdbc, SQL_HANDLE_DBC, retcode);
				}
				SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
			}
			SQLFreeHandle(SQL_HANDLE_ENV, henv);
		}
	}
