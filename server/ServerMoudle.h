
#pragma once
#include <winsock2.h>
#include <MSWSock.h>
#include <vector>

#define MAX_BUFFER_LEN 1024*8
enum OPERATION_TYPE
{
	ACCEPT,
	POST,
	RECV,
	NULL_POST
};
typedef struct _PER_IO_CONTEXT{
	OVERLAPPED   m_Overlapped;                      
	SOCKET       m_sockAccept;          // 这个I/O操作所使用的Socket，每个连接的都是一样的  
	WSABUF       m_wsaBuf;              // 存储数据的缓冲区
	char         m_szBuffer[MAX_BUFFER_LEN]; // 对应WSABUF里的缓冲区  
	OPERATION_TYPE  m_OpType;               //操作类型

	// 初始化
	_PER_IO_CONTEXT()
	{
		ZeroMemory(&m_Overlapped, sizeof(m_Overlapped));
		ZeroMemory(m_szBuffer, MAX_BUFFER_LEN);
		m_sockAccept = INVALID_SOCKET;
		m_wsaBuf.buf = m_szBuffer;
		m_wsaBuf.len = MAX_BUFFER_LEN;
		m_OpType = NULL_POST;
	}
	// 释放Socket
	~_PER_IO_CONTEXT()
	{
		if (m_sockAccept != INVALID_SOCKET)
		{
			closesocket(m_sockAccept);
			m_sockAccept = INVALID_SOCKET;
		}
	}
	// 重置缓冲区
	void ResetBuffer()
	{
		ZeroMemory(m_szBuffer, MAX_BUFFER_LEN);
	}

}PER_IO_CONTEXT ,*pPER_IO_CONTEXT;


typedef struct _PER_SOCKET_CONTEXT{
	SOCKET		 m_socket;
	SOCKADDR_IN		m_clientAddr;
	std::vector<PER_IO_CONTEXT *> m_IoVec;
	// 初始化
	_PER_SOCKET_CONTEXT()
	{
		m_socket = INVALID_SOCKET;
		memset(&m_clientAddr, 0, sizeof(m_clientAddr));
	}
	// 释放资源
	~_PER_SOCKET_CONTEXT()
	{
		if (m_socket != INVALID_SOCKET)
		{
			closesocket(m_socket);
			m_socket = INVALID_SOCKET;
		}
		// 释放掉所有的IO上下文数据
		for (int i = 0; i<m_IoVec.size(); i++)
		{
			delete m_IoVec[i];
		}
		m_IoVec.clear();
	}
}PER_SOCKET_CONTEXT,*pPER_SOCKET_CONTEXT;
class ServerMoudle
{
public:
	ServerMoudle();
	//初始化socket库
	bool InitSocketLib();
	//关闭socket库
	void CloseSocketLib(){ WSACleanup(); };
	int GetNoOfProcessors();

	bool InitListenSocket();

	bool PostRecv(PER_IO_CONTEXT* pIoContext);

	bool PostAccept(PER_IO_CONTEXT* pAcceptIoContext);

	bool PostSend(PER_IO_CONTEXT* pIoContext);

	//接受连接
	bool AcceptCon();

	//接收数据
	bool RecvData();

	bool SendData();

	bool StartServer();

	bool StopServer();

	bool InitIOCP();
	static DWORD WINAPI WorkerThread(LPVOID lpParam);

	virtual ~ServerMoudle();
private:
	HANDLE m_hIOCompletionPort;

	int m_Threads;
	
	PER_SOCKET_CONTEXT* m_pListenContext;
	HANDLE *m_phWorkerThreads;
};

