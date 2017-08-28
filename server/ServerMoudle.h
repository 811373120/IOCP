
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
	SOCKET       m_sockAccept;          // ���I/O������ʹ�õ�Socket��ÿ�����ӵĶ���һ����  
	WSABUF       m_wsaBuf;              // �洢���ݵĻ�����
	char         m_szBuffer[MAX_BUFFER_LEN]; // ��ӦWSABUF��Ļ�����  
	OPERATION_TYPE  m_OpType;               //��������

	// ��ʼ��
	_PER_IO_CONTEXT()
	{
		ZeroMemory(&m_Overlapped, sizeof(m_Overlapped));
		ZeroMemory(m_szBuffer, MAX_BUFFER_LEN);
		m_sockAccept = INVALID_SOCKET;
		m_wsaBuf.buf = m_szBuffer;
		m_wsaBuf.len = MAX_BUFFER_LEN;
		m_OpType = NULL_POST;
	}
	// �ͷ�Socket
	~_PER_IO_CONTEXT()
	{
		if (m_sockAccept != INVALID_SOCKET)
		{
			closesocket(m_sockAccept);
			m_sockAccept = INVALID_SOCKET;
		}
	}
	// ���û�����
	void ResetBuffer()
	{
		ZeroMemory(m_szBuffer, MAX_BUFFER_LEN);
	}

}PER_IO_CONTEXT ,*pPER_IO_CONTEXT;


typedef struct _PER_SOCKET_CONTEXT{
	SOCKET		 m_socket;
	SOCKADDR_IN		m_clientAddr;
	std::vector<PER_IO_CONTEXT *> m_IoVec;
	// ��ʼ��
	_PER_SOCKET_CONTEXT()
	{
		m_socket = INVALID_SOCKET;
		memset(&m_clientAddr, 0, sizeof(m_clientAddr));
	}
	// �ͷ���Դ
	~_PER_SOCKET_CONTEXT()
	{
		if (m_socket != INVALID_SOCKET)
		{
			closesocket(m_socket);
			m_socket = INVALID_SOCKET;
		}
		// �ͷŵ����е�IO����������
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
	//��ʼ��socket��
	bool InitSocketLib();
	//�ر�socket��
	void CloseSocketLib(){ WSACleanup(); };
	int GetNoOfProcessors();

	bool InitListenSocket();

	bool PostRecv(PER_IO_CONTEXT* pIoContext);

	bool PostAccept(PER_IO_CONTEXT* pAcceptIoContext);

	bool PostSend(PER_IO_CONTEXT* pIoContext);

	//��������
	bool AcceptCon();

	//��������
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

