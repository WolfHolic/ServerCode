#pragma comment(lib, "Ws2_32")
#pragma comment(lib, "libmySQL.lib")


#include <my_global.h>
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>
//#include <winsock2.h>
unsigned int WINAPI ThreadFunction(void *arg);
bool Quit;
SOCKET hClntSock;
const char *host = "localhost";
const char *user = "root";
const char *pw = "k2011y!^@$b4152";
const char *db = "information";

//-----------mysql------------//
MYSQL *connection = NULL;
MYSQL conn;
MYSQL_RES *sql_result;
MYSQL_ROW sql_row;

int main(int argc, char **argv)
{	
	HANDLE			hThread = NULL;
	DWORD			dwThreadID = NULL;
	DWORD			dw = 0;
	WSADATA			wsaData; 
	SOCKET			hServSock;
	SOCKADDR_IN		servAddr;
	SOCKADDR_IN		clntAddr;
	int				szClntAddr;
	int				strLen;
	char				message[256];
	char				port[] = "1624";
	char				ip[] = "127.0.0.1";
	

	if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0) 
	{
		return 0;
	}
	hServSock = socket(PF_INET,SOCK_STREAM,0);
	if(hServSock == INVALID_SOCKET)      
	{
		return 0;
	}

	if(mysql_init(&conn) == NULL)
	{
		printf("MYSQL INIT Error!");
		return 0;
	}

	memset(&servAddr,0,sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(port));

	if( bind(hServSock,(SOCKADDR*) &servAddr,sizeof(servAddr)) == SOCKET_ERROR)
	{
		printf("bind Error\n");
		return 0;
	}
	if(listen(hServSock,5) == SOCKET_ERROR)     
	{
		return 0;
	}
	szClntAddr = sizeof(clntAddr);
	hClntSock = accept(hServSock,(SOCKADDR*)&clntAddr,&szClntAddr);

	connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char*)NULL, 0);
	if(connection == NULL)
	{
		printf("%d 에러 : %s, %d\n", mysql_errno(&conn), mysql_error(&conn));
		return 1;
	}
	else
	{
		printf("sql connect success\n");

		if(mysql_select_db(&conn, db))
		{
			printf("%d 에러 : %s, %d\n", mysql_errno(&conn), mysql_error(&conn));
			return 1;
		}
	}


	printf("연결요청수락\n");


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
		strLen = send(hClntSock,message,sizeof(message),0); // 데이터전송
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
	closesocket(hClntSock);
	mysql_close(connection);
	WSACleanup();
	return 0;
}

unsigned int WINAPI ThreadFunction(void *arg)
{

	int strLen;
	char message[256];
	char recvedMessage[1024] = {0};
	char info[1000] = {0};
	int rowsize0 = 0;
	int rowsize1 = 0;
	int rowsize2 = 0;
	int rowsize3 = 0;
	int cnt = 0;
	while(!Quit)
	{
		memset(message, 0, sizeof(message) );
		strLen = recv(hClntSock,message,sizeof(message),0);

		if(strLen == SOCKET_ERROR )
		{
			printf("소켓에러! [recv]\n");
			Quit = true;
		}
		else if ( strLen == 0 )
		{
			printf("클라이언트 종료!");
			Quit = true;
			continue;
		}			
		/*short len = 0 ;
		memset(recvedMessage, 0, 1024);
		memcpy( &len, message, 2 );

		short type = 0;
		memcpy( &type, message +2, 2);
		
		if ( type == 1 )
		{
		}
		if ( type == 2 )
		{
			//attack
		}

		memcpy( &len, message + 4, 2 );
		memcpy( recvedMessage, message +6, len);
		printf("받은데이터> %s \n",recvedMessage);*/
		printf("받은데이터> %s \n",message);
		if(strcmp(message, "/info") == 0)
		{

			char *query = "select * from student";
			int state = 0;

			state = mysql_query(connection, query);
			if(state == 0)
			{
				sql_result = mysql_store_result(connection);

				
				memset(info,0,sizeof(info));
				char* temp = info;
				
				while((sql_row = mysql_fetch_row(sql_result))!=NULL)
				{
					printf("%s %s %s %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
				
					for ( int i = 0 ; i < sql_result->row_count; ++i )
					{
						auto size = strlen(sql_row[i]);
						memcpy(temp, sql_row[i], size );
						temp += size;
					}			

				}

				if ( temp != info )
					send(hClntSock, info, temp - info + 1, 0 );
				
				
				mysql_free_result(sql_result);
			}
		}
	}
	return 0;
}