// tcptest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <winsock2.h>
#pragma comment(lib,"Ws2_32")
int _tmain(int argc, _TCHAR* argv[])
{
	WSAData wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	SOCKET listen = socket(AF_INET,SOCK_STREAM,0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("47.94.196.157");	
//	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8888);

	if (connect(listen, (SOCKADDR*)&addrSrv, sizeof(addrSrv)) == 0){
		printf("111");
		char str[200];
		send(listen, str, 200, 0);
	}
	else printf("222");



	system("pause");
	return 0;
}

