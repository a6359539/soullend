// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Containers/StringConv.h"
#include "Engine/World.h"

#define MAX_BUFF_SIZE   4000
#define MAX_PACKET_SIZE  255
#define OP_RECV  1
#define OP_SEND  2


UMyGameInstance* TempInstance;
APlayerController* activcontroller;
UWorld* temp;
int val = 0;
bool	g_isshutdown = false;
FString te;
int t;

struct Overlap_ex {
	WSAOVERLAPPED original_overlap;
	int operation;
	WSABUF wsabuf;
	unsigned char iocp_buffer[MAX_BUFF_SIZE];
};

HANDLE g_hIocp;

struct Client {
	SOCKET s;
	Overlap_ex recv_overlap;
	int	packet_size;
	int previous_size;
	unsigned char packet_buff[MAX_PACKET_SIZE];
};
Client myclient;

void SendPacket(unsigned char *packet)
{
	Overlap_ex *over = new Overlap_ex;
	memset(over, 0, sizeof(Overlap_ex));
	over->operation = OP_SEND;
	over->wsabuf.buf = reinterpret_cast<CHAR *>(over->iocp_buffer);
	over->wsabuf.len = packet[0];
	memcpy(over->iocp_buffer, packet, packet[0]);

	int ret = WSASend(myclient.s, &over->wsabuf, 1, NULL, 0,
		&over->original_overlap, NULL);
	if (0 != ret) {
		int error_no = WSAGetLastError();
		//error_display("SendPacket:WSASend", error_no);
		while (true);
	}
}


void ProcessPacket(int id, unsigned char buf[])
{
	t = buf[1];
	switch (buf[1])
	{
	case SC_TRYLOGIN:
	{
		sc_packet_loginafter *my_packet = reinterpret_cast<sc_packet_loginafter  *>(buf);

		switch (my_packet->aftertype)
		{
		case 1://ID가 틀린경우
		{
			TempInstance->loginwidgetcontroll(my_packet->aftertype, activcontroller);
			break;
		}
		case 2://비밀번호가 틀린경우
		{
			//
			TempInstance->loginwidgetcontroll(my_packet->aftertype, activcontroller);
			break;
		}
		case 3://최종접속성공
		{
			t = 0;
			TempInstance->loginwidgetcontroll(my_packet->aftertype, activcontroller);
			break;
		}
		case 4://중복로그인 발견되는경우
		{

			TempInstance->loginwidgetcontroll(my_packet->aftertype, activcontroller);
			break;
		}
		default:
		{


		}break;
		}
		break;
	}
	case CSSC_IPPORT:
	{
		
		sc_packet_connetserver *my_packet = reinterpret_cast<sc_packet_connetserver  *>(buf);
		TempInstance->connectconsoltext(my_packet->portoffset);
		//	wchar_t strr[100];
		//	FName a="aaaa";
		//	FText TestHUDText = LOCTEXT("Your Key", "open %s",my_packet->ipport);
			//swprintf_s(strr,L"갸릉", 100);
		//	FString t = my_packet->ipport;
		//	FText c = FText::Format(LOCTEXT("HealthFormatStr", "open {0}"), FText::FromString(t));

			//TempWorld->connectconsoltext();
			//TempWorld->Exec(TempWorld->GetWorld(),TEXT("exit"));
			//const TCHAR* temptext = *ID;

		break;
	}
	case SC_IDSERCH:
	{


	}break;
	case CSSC_LISTENSERVERIP:
	{
		sc_packet_testconnetserver *my_packet = reinterpret_cast<sc_packet_testconnetserver  *>(buf);
		TempInstance->testconnectconsoltext(my_packet->ip);
	}break;
	default:
	{
		UKismetSystemLibrary::PrintString(TempInstance->GetWorld(), TEXT("이상패킷"), true, true, FLinearColor(1.f, 1.f, 1.f, 1.f), 10.f);

	}break;
	}

}
void WorkerThreadStart()
{

	while (false == g_isshutdown)
	{
		DWORD iosize;
		DWORD key;
		Overlap_ex *my_overlap;
		BOOL result = GetQueuedCompletionStatus(g_hIocp,
			&iosize, reinterpret_cast<PULONG_PTR>(&key), reinterpret_cast<LPOVERLAPPED *>(&my_overlap), INFINITE);
		if (FALSE == result) {
			// 에러 처리
		}

		if (0 == iosize) {
			closesocket(myclient.s);
			continue;
		}

		if (OP_RECV == my_overlap->operation) {
			unsigned char *buf_ptr = myclient.recv_overlap.iocp_buffer;
			int remained = iosize;
			while (0 < remained) {
				if (0 == myclient.packet_size)
					myclient.packet_size = buf_ptr[0];
				int required = myclient.packet_size
					- myclient.previous_size;
				if (remained >= required) {
					memcpy(
						myclient.packet_buff + myclient.previous_size,
						buf_ptr, required);
					ProcessPacket(key, myclient.packet_buff);
					buf_ptr += required;
					remained -= required;
					myclient.packet_size = 0;
					myclient.previous_size = 0;
				}
				else {
					memcpy(myclient.packet_buff
						+ myclient.previous_size,
						buf_ptr, remained);
					buf_ptr += remained;
					myclient.previous_size += remained;
					remained = 0;
				}
			}
			DWORD flags = 0;
			WSARecv(myclient.s,
				&myclient.recv_overlap.wsabuf, 1, NULL, &flags,
				&myclient.recv_overlap.original_overlap, NULL);
		}
		else if (OP_SEND == my_overlap->operation) {
			delete my_overlap;
		}
		else {
			printf("Unknown IOCP event!\n");
			exit(-1);
		}
	}
}


UMyGameInstance::UMyGameInstance()
{
	TempInstance = this;


}



bool UMyGameInstance::connectserver()
{

	if (!connect) {
		connect = true;
		WSADATA	wsadata;
		WSAStartup(MAKEWORD(2, 2), &wsadata);
		g_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);

		myclient.s = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

		SOCKADDR_IN ServerAddr;
		ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
		ServerAddr.sin_family = AF_INET;
		ServerAddr.sin_port = htons(4000);
		ServerAddr.sin_addr.s_addr = inet_addr("211.178.34.106");


		HANDLE iocp = CreateIoCompletionPort((HANDLE)myclient.s, g_hIocp, 0, 0);
		if ((WSAConnect(myclient.s, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr), NULL, NULL, NULL, NULL)) == SOCKET_ERROR) {
			UMyGameInstance::failconnectserver();
			connect = false;
			return 0;
		}


		ZeroMemory(&myclient.recv_overlap.original_overlap, sizeof(Overlap_ex));
		myclient.recv_overlap.operation = OP_RECV;
		myclient.packet_size = 0;
		myclient.previous_size = 0;
		myclient.recv_overlap.wsabuf.buf = reinterpret_cast<CHAR *>(myclient.recv_overlap.iocp_buffer);
		myclient.recv_overlap.wsabuf.len = sizeof(myclient.recv_overlap.iocp_buffer);



		DWORD flag = 0;
		int result = WSARecv(myclient.s, &myclient.recv_overlap.wsabuf, 1,
			NULL, &flag, reinterpret_cast<LPWSAOVERLAPPED>(&myclient.recv_overlap.original_overlap), NULL);
		if (result == SOCKET_ERROR) {
			if (WSAGetLastError() != WSA_IO_PENDING) {
				UMyGameInstance::failconnectserver();
				connect = false;
				return 0;
			}
		}



		for (auto i = 0; i < 2; ++i)
			worker_threads.push_back(new thread{ WorkerThreadStart });
	}

	return 1;
}

bool UMyGameInstance::destroyend() //  강제종료시 메모리 누수없음 정상종료떄 쓰레드 중지 시도 
{
	//for (auto th : worker_threads) {
	//	th->join();
	//	delete th;
	//}
	WSACleanup();
	return false;
}

bool UMyGameInstance::trylogin(APlayerController* pc)
{
	activcontroller = pc;
	cs_packet_login enter_packet;
	enter_packet.type = CS_TRYLOGIN;
	enter_packet.size = 500;
	//swprintf_s(enter_packet.id, TEXT("abcd"));
	//swprintf_s(enter_packet.password, UTF8_TO_TCHAR("abcdASSAAS!D"));
	const TCHAR* temptext = *ID;
	wcsncpy_s(enter_packet.id, temptext, MAX_STR_SIZE);
	temptext = *PASSWORD;
	wcsncpy_s(enter_packet.password, temptext, MAX_STR_SIZE);

	//UKismetSystemLibrary::PrintString(GetWorld(), enter_packet.id, true, true, FLinearColor(1.f, 1.f, 1.f, 1.f), 10.f);
	//UKismetSystemLibrary::PrintString(GetWorld(), enter_packet.password, true, true, FLinearColor(1.f, 1.f, 1.f, 1.f), 10.f);
	//wcsncpy_s(enter_packet.message, clients[key].avatar.name, MAX_STR_SIZE);
	SendPacket(reinterpret_cast<unsigned char *>(&enter_packet));


	return true;
}

bool UMyGameInstance::connectdedicateserver(int typeserver)
{

	cs_packet_connetserver enter_packet;
	enter_packet.type = CSSC_IPPORT;
	enter_packet.size = sizeof(cs_packet_connetserver);
	enter_packet.servertype = typeserver;
	SendPacket(reinterpret_cast<unsigned char *>(&enter_packet));

	return true;
}

bool UMyGameInstance::trylistenservercreate()
{
	cs_simpletry enter_packet;
	enter_packet.type = CSSC_LISTENSERVERIP;
	enter_packet.size = sizeof(cs_simpletry);
	SendPacket(reinterpret_cast<unsigned char *>(&enter_packet));
	return true;
}
