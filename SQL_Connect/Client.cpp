#pragma comment(lib, "Ws2_32")

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>
//#include <winsock2.h>
unsigned int WINAPI ThreadFunction(void *arg);
bool Quit;
SOCKET hSocket;

HANDLE hMutex;

int main()
{
	HANDLE			hThread = NULL;
	DWORD			dwThreadID = NULL;
	DWORD			dw = 0;
	WSADATA			wsaData;
	SOCKADDR_IN		servAddr;

	char			message[256];
	int			strLen;	
	char			port[] = "1624";
	char			ip[] = "127.0.0.1";


	hMutex = CreateMutex(NULL, FALSE, NULL);
	if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0) 
	{
		printf("WSAStartup() error!");
		return 0;
	}
	printf("윈속라이브러리초기화\n");


	//클라이언트소켓생성
	hSocket = socket(PF_INET,SOCK_STREAM,0);
	if(hSocket == INVALID_SOCKET)
	{
		return 0;
	}
	printf("클라이언트소켓생성\n");

	memset(&servAddr,0,sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(ip);
	servAddr.sin_port = htons(atoi(port));
	//서버로연결요청
	if( connect(hSocket,(SOCKADDR*)&servAddr,sizeof(servAddr)) == SOCKET_ERROR)
	{
		return 0;
	}
	printf("서버에연결요청\n");
	hThread = (HANDLE)_beginthreadex(NULL,0,ThreadFunction,NULL,0,(unsigned*)&dwThreadID);
	if(!hThread)
	{
		printf("_beginthreadex Error\n");
		return FALSE;
	}
	printf("쓰레드핸들[ %d ]\n",hThread);
	printf("쓰레드ID [ %d ]\n",dwThreadID);
	while(!Quit)
	{
		gets(message);
		if(!strcmp(message,"exit"))
		{
			Quit = true;
		}
		strLen = send(hSocket,message,sizeof(message),0); // 데이터전송
		if(strLen == SOCKET_ERROR)    
		{
			printf("소켓에러! [send]\n");
			Quit = true;
		}
	}
	Quit = true;
	dw = WaitForSingleObject(hThread,INFINITE);
	if(dw == WAIT_FAILED) 
	{
		printf("WaitForSingleObject Error\n");
		return FALSE;
	}
	printf("main 함수종료!!\n");
	CloseHandle(hMutex);
	CloseHandle(hThread);
	closesocket(hSocket);
	WSACleanup();
	return 0;
}


unsigned int WINAPI ThreadFunction(void *arg)
{
	int strLen;
	char message[256];
	while(!Quit)
	{
		strLen = recv(hSocket,message,sizeof(message),0);
		if(strLen == SOCKET_ERROR )
		{
			printf("소켓에러! [recv]\n");
			Quit = true;
		}
		else if ( strLen > 0 )
		{
			printf("받은데이터> %s \n",message);
		}
		if(WaitForSingleObject(hMutex,INFINITE) == WAIT_FAILED) 
			printf("WaitForSingleObject Error\n");
		if( ReleaseMutex(hMutex) == FALSE ) 
			printf("ReleaseMutex Error\n");
	}
	return 0;
}
