// IOCP_TCPIP_Socket_Server.cpp

#include <WinSock2.h>
#include <Windows.h>
#include <vector>
#include <iostream>
#include "ServerMoudle.h"
using namespace std;

#pragma comment(lib, "Ws2_32.lib")		// Socket������õĶ�̬���ӿ�
#pragma comment(lib, "Kernel32.lib")	// IOCP��Ҫ�õ��Ķ�̬���ӿ�


int main()
{
	ServerMoudle s;
	s.StartServer();
}
