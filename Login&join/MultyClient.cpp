#pragma comment(lib, "Ws2_32")
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>

unsigned int WINAPI ThreadFunction(void *arg);
bool Quit;
SOCKET servSock;
const int PORT		= 1624;
const int BUFSIZE	= 256;

bool			start = true;
bool			create = false;
bool			login = false;
bool			chat = false;
char			Select[3];
int				readbytes;			// send, receive �ϴ� ������ ũ��



int main()
{
	HANDLE			hThread = NULL;
	DWORD			dwThreadID = NULL;
	DWORD			dw = 0;
	WSADATA			wsaData;

	int				strLen;

	SOCKADDR_IN		servAddr;
	char			ip[] = "127.0.0.1";

	char			buffer[BUFSIZE];
	char			exit[BUFSIZE];
	char			temp[BUFSIZE];
	int			len;
	int			i;
	int			cut = 0;

	char			IP[16] = "127.0.0.1";
	char			ID[BUFSIZE];
	char			PW[BUFSIZE];
	char			ReStart[BUFSIZE]; 
	bool			chatstart=false;




	if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0) 
	{
		printf("WSAStartup() error!");
		return 0;
	}
	printf("���Ӷ��̺귯���ʱ�ȭ\n");

	//Ŭ���̾�Ʈ���ϻ���
	servSock = socket(PF_INET,SOCK_STREAM,0);
	if(servSock == INVALID_SOCKET)
	{
		return 0;
	}
	printf("Ŭ���̾�Ʈ���ϻ���\n");

	memset(&servAddr,0,sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(ip);
	servAddr.sin_port = htons(PORT);

	//�����ο����û
	if( connect(servSock,(SOCKADDR*)&servAddr,sizeof(servAddr)) == SOCKET_ERROR)
	{
		return 0;
	}
	printf("�����������û\n");


	// ���ӵ� ������ IP ��Ȯ��
	printf("success to connect : %d.%d.%d.%d\n",
		servAddr.sin_addr.S_un.S_un_b.s_b1, servAddr.sin_addr.S_un.S_un_b.s_b2,
		servAddr.sin_addr.S_un.S_un_b.s_b3, servAddr.sin_addr.S_un.S_un_b.s_b4);

	//hThread = (HANDLE)_beginthreadex(NULL,0,ThreadFunction,NULL,0,(unsigned*)&dwThreadID);



	while(!Quit)
	{
		printf("1. ȸ������    2. �α���\n");
		scanf("%s", &buffer);
		send(servSock, buffer, sizeof(buffer), 0);
		while(start)
		{
			strcpy(Select, buffer);
			if(Select[0] == '1')
			{
				printf("Join ID : ");
				scanf("%s", &ID);
				send(servSock, ID, sizeof(ID), 0);

				printf("Set PW : ", buffer);
				scanf("%s", &PW);
				send(servSock, PW, sizeof(PW), 0);
				start = false;
				login = true;
			}
			else if(Select[0] == '2')
			{
				start = false;
				login = true;
			}
			else
			{
				printf("�߸��� �Է��Դϴ�. ���α׷��� �����մϴ�.\n");
				start = false;
				create = false;
				login = false;
				chat = false;
				Quit = true;
				break;
			}

			while(login)
			{
				//--------id
				printf("Login ID : ");
				scanf("%s", &ID);
				send(servSock, ID, sizeof(ID), 0);

				//--------pw
				printf("Login PW : ");
				scanf("%s", &PW);
				send(servSock, PW, sizeof(PW), 0);

				readbytes = recv(servSock, buffer, sizeof(buffer),0);
				
				if(buffer[0] == '1')
				{
					printf("Login Failed. Please Join\n");
					strcpy(Select, "1");
					start = true;
					login = false;
				}
				else // �α��� ����
				{
					printf("Login Success. Have a good time\n");
					login = false;
					chat = true;
					memset(buffer, '\0', BUFSIZE);
				}
			}
		}

		if(chat==true)
		{
					hThread = (HANDLE)_beginthreadex(NULL,0,ThreadFunction,NULL,0,(unsigned*)&dwThreadID);

					if(!hThread)
					{
						printf("_beginthreadex Error\n");
						return false;
					}

					printf("�������ڵ�[ %d ]\n",hThread);
					printf("������ID [ %d ]\n",dwThreadID);
		}

		// �������� ������ ���
		while ( chat )
		{
			

			gets(buffer);
			if(!strcmp(buffer,"/quit"))
			{
				Quit = true;
				chat = false;
				strcpy(buffer, "����ڴ��� �����ϼ̽��ϴ�.\n");
			}
			if(!strcmp(buffer, "/remsg"))
			{
				strcpy(buffer, temp);
			}
			strLen = send(servSock,buffer,sizeof(buffer),0); // ����������
			strcpy(temp, buffer);
			if(strLen == SOCKET_ERROR)    
			{
				printf("Sock Error! [send]\n");
				Quit = true;
			}
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
	strcpy(buffer, "-1");
	strLen = send(servSock,buffer,sizeof(buffer),0); // ����������
	closesocket(servSock);
	WSACleanup();
	return 0;
}

unsigned int WINAPI ThreadFunction(void *arg)
{
	int				strLen;
	char			buffer[256];
	while(!Quit)
	{
		memset(buffer,0,sizeof(buffer));
		strLen = recv(servSock,buffer,sizeof(buffer),0);
		if(strLen == SOCKET_ERROR )
		{
			printf("Sock Error! [recv]\n");
			Quit = true;
		}
		printf("����������> %s \n",buffer);


	}
	return 0;
}
