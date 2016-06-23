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
	printf("���Ӷ��̺귯���ʱ�ȭ\n");


	//Ŭ���̾�Ʈ���ϻ���
	hSocket = socket(PF_INET,SOCK_STREAM,0);
	if(hSocket == INVALID_SOCKET)
	{
		return 0;
	}
	printf("Ŭ���̾�Ʈ���ϻ���\n");

	memset(&servAddr,0,sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(ip);
	servAddr.sin_port = htons(atoi(port));
	//�����ο����û
	if( connect(hSocket,(SOCKADDR*)&servAddr,sizeof(servAddr)) == SOCKET_ERROR)
	{
		return 0;
	}
	printf("�����������û\n");
	hThread = (HANDLE)_beginthreadex(NULL,0,ThreadFunction,NULL,0,(unsigned*)&dwThreadID);
	if(!hThread)
	{
		printf("_beginthreadex Error\n");
		return FALSE;
	}
	printf("�������ڵ�[ %d ]\n",hThread);
	printf("������ID [ %d ]\n",dwThreadID);
	while(!Quit)
	{
		gets(message);
		if(!strcmp(message,"exit"))
		{
			Quit = true;
		}
		strLen = send(hSocket,message,sizeof(message),0); // ����������
		if(strLen == SOCKET_ERROR)    
		{
			printf("���Ͽ���! [send]\n");
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
	printf("main �Լ�����!!\n");
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
			printf("���Ͽ���! [recv]\n");
			Quit = true;
		}
		else if ( strLen > 0 )
		{
			printf("����������> %s \n",message);
		}
		if(WaitForSingleObject(hMutex,INFINITE) == WAIT_FAILED) 
			printf("WaitForSingleObject Error\n");
		if( ReleaseMutex(hMutex) == FALSE ) 
			printf("ReleaseMutex Error\n");
	}
	return 0;
}
