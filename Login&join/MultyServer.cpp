#pragma comment(lib, "Ws2_32")
#include <stdio.h>
#include <string.h>
#include <WinSock2.h>

#define BUFSIZE 256
const int PORT		= 1624;

struct Information
{
	int		sockNum;
	char	id[BUFSIZE];
};


int main(int argc, char *argv[])
{
	Information			Info[10];
	WSADATA				wsaData;
	SOCKET				servSock = 0;
	SOCKADDR_IN			servAddr = {0};
	int					arrIndex = 0;
	int					clntLen = {0};
	int					sendIndex = 0;
	SOCKET				clntSock = 0;
	SOCKADDR_IN			clntAddr = {0};

	fd_set				reads = {0};
	fd_set				temps = {0};


	int					strLen = 0;
	TIMEVAL				timeout = {0};

	char			buffer[BUFSIZE] = {0};	// send, receive 하는 데이터
	char			temp[BUFSIZE] = {0};
	char			ID[BUFSIZE] = {0};
	char			PW[BUFSIZE] = {0};
	char			data[BUFSIZE] = {0};
	char			Select[BUFSIZE] = {0};
	char			Join[BUFSIZE]		= "1";
	char			Login[BUFSIZE]	= "2";
	char			ReStart[BUFSIZE] = {0}; 
	char			*line_p;
	char			Save[BUFSIZE] = {0};

	char			slince[BUFSIZE] = {0};
	bool			start[10];

	bool			login[10]		= {false};
	int				count = -1;
	int				cut = 0;
	bool			id_Search[10]	= {true};
	bool			search[10]		= {true};
	bool			chat[10]		= {false};
	bool			ReEnter[10]		= {false};

	for(int i=0; i<10; i++)
	{
		start[i] = true;
		search[i] = true;
		id_Search[i] = true;
	}

	FILE						*UD;
	memset(buffer, '\0', BUFSIZE);
	memset(ID, '\0', BUFSIZE);
	memset(PW, '\0', BUFSIZE);
	memset(data, '\0', BUFSIZE);
	memset(ReStart, '\0', BUFSIZE);

	// 프로그램 정보 출력
	printf("TCP Server Ver 0.1\n\n");
	if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
	{
		printf("WSAStartup Error \n");
	}

	servSock = socket(PF_INET, SOCK_STREAM, 0);

	if(servSock == INVALID_SOCKET)
	{
		printf("socketError\n");
	}

	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(PORT);

	if(bind(servSock,(SOCKADDR*)&servAddr,sizeof(servAddr)) == SOCKET_ERROR)
	{
		printf("bind Error\n");
	}
	if(listen(servSock,5) == SOCKET_ERROR)
	{
		printf("listen Error\n");
	}
	FD_ZERO(&reads);
	FD_SET(servSock,&reads);
	// 현재 클라이언트의 접속을 기다린다고 표시
	printf("wait for connecting\n");

	while(1)
	{
		temps = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

		if(select(0,&temps,0,0,&timeout) == SOCKET_ERROR)
		{
			printf("select Error\n");
		}

		for(arrIndex = 0; arrIndex < reads.fd_count; arrIndex++) 
		{
			if(FD_ISSET(reads.fd_array[arrIndex], &temps)) 
			{
				if(reads.fd_array[arrIndex] == servSock)
				{
					clntLen = sizeof(clntAddr);
					clntSock = accept(servSock,(SOCKADDR*)&clntAddr,&clntLen);
					FD_SET(clntSock,&reads);
					printf("Connect Client Sock : %d\n",clntSock);
					count++;
				}
				else
				{
					
					/*while(start)
					//{
					//	//-------------------------------------------//
					//	strLen = recv(clntSock, Select, sizeof(Select), 0);
					//	if(Select[0] == '1')
					//	{
					//		UD = fopen("UserData.txt", "a");
					//		printf("Open File\n");
					//		if(UD==NULL)
					//		{
					//			printf("파일이 없습니다.\n");
					//			closesocket(clntSock);
					//			closesocket(servSock);
					//			WSACleanup();
					//		}

					//		strLen = recv(clntSock, ID, sizeof(ID), 0);
					//		printf("ID : %s\n", ID);
					//		fputs(ID, UD);
					//		fputs("\n", UD);

					//		strLen = recv(clntSock, PW, sizeof(PW), 0);
					//		printf("PW : %s\n", PW);
					//		fputs(PW, UD);
					//		fputs("\n", UD);

					//		fclose(UD);
					//		start = false;
					//		login = true;
					//	}
					//	else if(Select[0] == '2')
					//	{
					//		start = false;
					//		login = true;
					//	}
					//	else
					//	{
					//		
					//		closesocket(clntSock);
					//		closesocket(servSock);
					//		WSACleanup();
					//	}


					//	while(login)
					//	{
					//		UD = fopen("UserData.txt", "a");
					//		fclose(UD);
					//		UD = fopen("UserData.txt", "r");
					//		if(UD==NULL)
					//		{
					//			printf("파일이 없습니다.\n");
					//			closesocket(clntSock);
					//			closesocket(servSock);
					//			WSACleanup();
					//		}
					//		fclose(UD);
					//		UD = fopen("UserData.txt", "r");
					//		if(UD==NULL)
					//		{
					//			printf("파일이 없습니다.\n");
					//			closesocket(clntSock);
					//			closesocket(servSock);
					//			WSACleanup();
					//		}
					//		//ID,PW 받기
					//		strLen = recv(clntSock, ID, sizeof(ID), 0);
					//		printf("Login ID : %s\n", ID);

					//		strLen = recv(clntSock, PW, sizeof(PW), 0);
					//		printf("Login PW : %s\n", PW);

					//		//검사
					//		while(search)
					//		{
					//			while(!feof(UD))
					//			{
					//				ReEnter[count] = true; // 
					//				fgets(data, sizeof(ID), UD);
					//				if((line_p = strchr(data, '\n')) != NULL)*line_p ='\0';

					//				if(strcmp(ID, data)==0)
					//				{
					//					fgets(data, sizeof(PW), UD);
					//					if((line_p = strchr(data, '\n')) != NULL)*line_p ='\0';
					//					if(strcmp(PW, data)==0)
					//					{
					//						strcpy(buffer, "Login Success");
					//						send(clntSock, buffer, sizeof(buffer), 0);
					//						search = false;
					//						ReEnter[count] = false;
					//						login = false;
					//						chat[count] = true;
					//						break;
					//					}
					//				}									
					//				login = false;
					//				chat[count] = true;
					//				search = false;
					//			}
					//		}


					//		while(ReEnter[count])
					//		{
					//			memset(buffer, '\0', BUFSIZE);
					//			strcpy(buffer,"1");
					//			send(clntSock, buffer, sizeof(buffer), 0);
					//			strcpy(Select, "1");
					//			start = true;
					//			login = false;
					//			search = true;
					//			ReEnter[count] = false;
					//			memset(buffer, '\0', BUFSIZE);
					//		}
					//		fclose(UD);

					//	}
					//}
					//strLen = recv(reads.fd_array[arrIndex],buffer,BUFSIZE,0);
					//if(strLen == -1)
					//{
					//	FD_CLR(reads.fd_array[arrIndex],&reads);
					//	closesocket(temps.fd_array[arrIndex]);
					//	printf("Exit Client Sock : %d\n",temps.fd_array[arrIndex]);
					//}
					//else
					//{

					//	printf("%d Client : Recvbuffer : %s\n",reads.fd_array[arrIndex],buffer);

					//	for(sendIndex = 0; sendIndex < reads.fd_count; sendIndex++) 
					//	{
					//		if( reads.fd_array[sendIndex] != servSock && reads.fd_array[sendIndex] != reads.fd_array[arrIndex])
					//		{
					//			send(reads.fd_array[sendIndex],buffer,strLen,0);
					//			printf("%d Client : Sendbuffer : %s\n",reads.fd_array[sendIndex],buffer);
					//		}
					//	}
					//}

					//}*/
					strLen = recv(reads.fd_array[arrIndex],buffer,BUFSIZE,0);
					strcpy(Select, buffer);

					if(strLen == -1)
					{
						FD_CLR(reads.fd_array[arrIndex],&reads);
						closesocket(temps.fd_array[arrIndex]);
						printf("Exit Client Sock : %d\n",temps.fd_array[arrIndex]);
					}
					else
					{
						while(start[count])
						{
							//-------------------------------------------//							
							if(Select[0] == '1')
							{
								UD = fopen("UserData.txt", "a");
								printf("Open File\n");
								if(UD==NULL)
								{
									printf("파일이 없습니다.\n");
									closesocket(clntSock);
									closesocket(servSock);
									WSACleanup();
								}

								strLen = recv(clntSock, ID, sizeof(ID), 0);
								printf("ID : %s\n", ID);
								fputs(ID, UD);
								fputs("\n", UD);

								strLen = recv(clntSock, PW, sizeof(PW), 0);
								printf("PW : %s\n", PW);
								fputs(PW, UD);
								fputs("\n", UD);

								fclose(UD);
								start[count] = false;
								login[count] = true;
							}
							else if(Select[0] == '2')
							{
								start[count] = false;
								login[count] = true;
							}
							else
							{

								closesocket(clntSock);
								closesocket(servSock);
								WSACleanup();
							}


							while(login[count])
							{
								UD = fopen("UserData.txt", "a");
								fclose(UD);
								UD = fopen("UserData.txt", "r");
								if(UD==NULL)
								{
									printf("파일이 없습니다.\n");
									closesocket(clntSock);
									closesocket(servSock);
									WSACleanup();
								}
								fclose(UD);
								UD = fopen("UserData.txt", "r");
								if(UD==NULL)
								{
									printf("파일이 없습니다.\n");
									closesocket(clntSock);
									closesocket(servSock);
									WSACleanup();
								}
								//ID,PW 받기
								strLen = recv(clntSock, ID, sizeof(ID), 0);
								printf("Login ID : %s\n", ID);

								strLen = recv(clntSock, PW, sizeof(PW), 0);
								printf("Login PW : %s\n", PW);

								//검사
								while(search[count])
								{
									while(!feof(UD))
									{
										ReEnter[count] = true; // 
										fgets(data, sizeof(ID), UD);
										if((line_p = strchr(data, '\n')) != NULL)*line_p ='\0';

										if(strcmp(ID, data)==0)
										{
											fgets(data, sizeof(PW), UD);
											if((line_p = strchr(data, '\n')) != NULL)*line_p ='\0';
											if(strcmp(PW, data)==0)
											{
												strcpy(buffer, "Login Success");
												send(clntSock, buffer, sizeof(buffer), 0);
												search[count] = false;
												ReEnter[count] = false;
												login[count] = false;
												chat[count] = true;
												strcpy(Info[count].id, ID);
												Info[count].sockNum = clntSock;
												printf("ID : %s, sockNum : %d\n", Info[count].id, Info[count].sockNum);
												break;
											}
										}									
										login[count] = false;
										chat[count] = true;
										search[count] = false;

									}
								}


								while(ReEnter[count])
								{
									memset(buffer, '\0', BUFSIZE);
									strcpy(buffer,"1");
									send(clntSock, buffer, sizeof(buffer), 0);
									strcpy(Select,"1");
									start[count] = true;
									login[count] = false;
									search[count] = true;
									ReEnter[count] = false;
								}
								fclose(UD);
							}
						}
						printf("%d Client : Recvbuffer : %s\n",reads.fd_array[arrIndex],buffer);




						for(sendIndex = 0; sendIndex < reads.fd_count; sendIndex++) 
						{						
							if(chat[sendIndex]==true)
							{
								memcpy(Save, buffer, 2);
								if(strcmp(Save,"/w") == 0)
								{
									memset(Save, '\0', BUFSIZE);
									memcpy(Save, buffer+3, strlen(ID));
									for(int k = 0; k <= count+2 ; k++)
									{
										if(strcmp(Save, Info[k].id) == 0)
										{
											send(Info[k].sockNum,buffer,strLen,0);
											printf("%d Client<Whisper> : Sendbuffer : %s\n",reads.fd_array[sendIndex],buffer);
											memset(Save, '\0', BUFSIZE);
											memset(buffer, '\0', BUFSIZE);
										}
									}

								}
								else
								{
									if( reads.fd_array[sendIndex] != servSock && reads.fd_array[sendIndex] != reads.fd_array[arrIndex])
									{							
										send(reads.fd_array[sendIndex],buffer,strLen,0);
										printf("%d Client : Sendbuffer : %s\n",reads.fd_array[sendIndex],buffer);
									}
								}

							}
						}
					}
				}
			}
		}
	}
	WSACleanup();
	return 0;
}