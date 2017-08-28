// IOCP_TCPIP_Socket_Server.cpp

#include <WinSock2.h>
#include <Windows.h>
#include <vector>
#include <iostream>
#include "ServerMoudle.h"
using namespace std;

#pragma comment(lib, "Ws2_32.lib")		// Socket编程需用的动态链接库
#pragma comment(lib, "Kernel32.lib")	// IOCP需要用到的动态链接库


int main()
{
	ServerMoudle s;
	s.StartServer();
}
