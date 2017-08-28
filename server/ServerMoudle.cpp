#include "ServerMoudle.h"


ServerMoudle::ServerMoudle()
{
}


ServerMoudle::~ServerMoudle()
{
}
int ServerMoudle::GetNoOfProcessors(){
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwNumberOfProcessors;
}
bool ServerMoudle::InitIOCP(){

	m_hIOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);
	if (m_hIOCompletionPort == NULL){
		return false;
	}
	m_Threads = 2 * GetNoOfProcessors();
	// 为工作者线程初始化句柄
	m_phWorkerThreads = new HANDLE[m_Threads];

	// 根据计算出来的数量建立工作者线程
	DWORD nThreadID;
	for (int i = 0; i < m_Threads; i++)
	{
		m_phWorkerThreads[i] = ::CreateThread(0, 0, WorkerThread, NULL, 0, &nThreadID);
	}

	return	true;
}

bool ServerMoudle::InitSocketLib(){
	WSAData wsa;
	if (0==WSAStartup(MAKEWORD(2, 2), &wsa)){

		return true;
	}
	return false;
}
bool ServerMoudle::InitListenSocket(){

	GUID GuidAcceptEx = WSAID_ACCEPTEX;
	GUID GuidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS;
	struct sockaddr_in ServerAddress;

	m_pListenContext = new PER_SOCKET_CONTEXT();

	
	m_pListenContext->m_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

}
bool ServerMoudle::StartServer(){

	if (InitSocketLib() == false)return false;

	if(InitIOCP()==false)return false;


}