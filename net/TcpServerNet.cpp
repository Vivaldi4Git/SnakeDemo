#include "TcpServerNet.h"
#include "../mediator/TcpServerMediator.h"
#include <process.h>
using namespace std;
TcpServerNet::TcpServerNet(INetMediator* pMediator):m_listenSocket(INVALID_SOCKET), m_isServerStop(false)
{
    m_pMediator = pMediator;

}
TcpServerNet::~TcpServerNet()
{

}


//��ʼ������
bool TcpServerNet::InitNet()
{
	//1. ���ؿ�
	WSADATA wsadata; //����Windows Socket API
	SOCKADDR_IN serverAddr; //����server��Socket�������Ϣ
	SOCKADDR_IN clientAddr; //����server��Socket�������Ϣ
	char recvBuf[1024] = { 0 }; //�������ݵĻ�����
	char sendBuf[1024] = { 0 }; //�������ݵĻ�����
	int ret; //����ֵ

	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) //���ز�Ϊ���ʼ��ʧ��
	{
		std::cout << "Windows Socket API Startup FAILED:" << WSAGetLastError() << std::endl;
		return false;
	}

	//2. �����׽���
	m_listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // ���������׽���
	if (m_listenSocket == INVALID_SOCKET) //ʧ��
	{
		std::cout << "Windows Socket Build FAILED:" << WSAGetLastError() << std::endl;
		WSACleanup(); //ж�ؿ�
		return false;
	}

	//3. ��
	//����sockaddr_in
	serverAddr.sin_family = AF_INET; //IPv4Э����
	serverAddr.sin_port = htons(12345); //�˿ں�
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); //�����п���IP
	if (SOCKET_ERROR == bind(m_listenSocket, (const sockaddr*)&serverAddr, sizeof(serverAddr)))//��IP�͵�ַ��Ϣ
	{
		std::cout << "Windows Socket bind FAILED:" << WSAGetLastError() << std::endl;
		closesocket(m_listenSocket); //�ر��׽���
		WSACleanup(); //ж�ؿ�
		return false;
	}

	//4. ����
	if (SOCKET_ERROR == listen(m_listenSocket, SOMAXCONN)) //��������
	{
		std::cout << "Windows Socket listen FAILED:" << WSAGetLastError() << std::endl;
		closesocket(m_listenSocket); //�ر��׽���
		WSACleanup(); //ж�ؿ�
		return false;
	}
	std::cout << "Listening for incoming connections..." << std::endl;

	//5. ���������߳�
	HANDLE hThreadHandle = (HANDLE)_beginthreadex(NULL, 0, &AcceptThread, this, 0, 0);
	if (hThreadHandle)//�����ɹ�
	{
		m_ThreadHandleList.push_back(hThreadHandle);//������, �����������
	}
    return true;
}


//�ر����� 
void TcpServerNet::UninitNet()
{
	//1. ���ñ�־λ��ʾ�������ѹر�
	m_isServerStop = true;

	//2. ���ݴ�����߳̾���б��ͷ��߳�
	for (auto handleIterator = m_ThreadHandleList.begin(); handleIterator != m_ThreadHandleList.end();)
	{
		if (*handleIterator) //�������
		{
			if (WAIT_TIMEOUT == WaitForSingleObject(*handleIterator, 1000)) //�ȴ�һ��ʱ��ȴ��߳̽���
			{
				TerminateThread(*handleIterator, -1); //��ֹ�߳�
			}
			CloseHandle(*handleIterator);//�رվ��
			*handleIterator = nullptr; //�ÿ�
		}
		handleIterator = m_ThreadHandleList.erase(handleIterator); //erase
	}

	//3. �ر�server�׽���
	if (m_listenSocket && m_listenSocket != INVALID_SOCKET) //���
	{
		closesocket(m_listenSocket);
	}

	//4. �����߳�ID�رտͻ����׽���
	for (auto IDIterator = m_mapThreadIdToSocket.begin(); IDIterator != m_mapThreadIdToSocket.end();)
	{
		if (IDIterator->second && IDIterator->second != INVALID_SOCKET) //���
		{
			closesocket(IDIterator->second);
		}
		IDIterator = m_mapThreadIdToSocket.erase(IDIterator); //erase
	}

	//5. ж�ؿ�
	WSACleanup();

}
//��������
bool TcpServerNet::SendData(long lSendIp, char* buf, int nLen)
{
	//1. ��⴫��Ĳ����Ƿ�Ϸ�
	if (!buf || nLen < 0) //bufΪ��˵��û�����ݿɷ��ͣ� nLen < 0
	{
		std::cout << "parameter error. " << std::endl;
		return false;
	}

	//2. �ȷ�����С
	if (send(lSendIp, (char*) & nLen, sizeof(int), 0) <= 0) //���ͳ���
	{
		std::cout << "send error:" << WSAGetLastError() << std::endl;
		return false;
	}

	//3. �ٷ�������
	if (send(lSendIp, buf, nLen, 0) <= 0) //���ͳ���
	{
		std::cout << "send error:" << WSAGetLastError() << std::endl;
		return false;
	}
    return true;
}
//��������
void TcpServerNet::RecvData()
{
	//1. ȡ���̶߳�Ӧ��socket
	SOCKET clientSocket = m_mapThreadIdToSocket[GetCurrentThreadId()];

	//2. ������socket
	if (!clientSocket || clientSocket == INVALID_SOCKET)
	{
		return;
	}
	//3. ѭ����������
	int nRecvNum = 0; //ʵ�ʽ��յ�������
	int packSize = 0; //���Ĵ�С
	int offset = 0; //ƫ����
	while (!m_isServerStop)
	{
		//3.1 ���հ���С
		nRecvNum = recv(clientSocket, (char*)&packSize, sizeof(int), 0);

		
		if (nRecvNum > 0)//���յ�������
		{
			
			char* recvPackBuf = new char[packSize];//�������ݵĻ�����
			offset = 0;

			//3.2 ���հ�����
			while (packSize > 0) //����С��Ϊ0
			{
				nRecvNum = recv(clientSocket, recvPackBuf + offset, packSize, 0);//����
				if (nRecvNum > 0) //���յ�������
				{
					offset += nRecvNum; //����ƫ����
					packSize -= nRecvNum; //��ȡ�����°���С
				}
			}
			//3.3 ת�����н���
			if (m_pMediator)
			{
				m_pMediator->DealData(clientSocket, recvPackBuf, nRecvNum);
			}
		}

		
	}

}

unsigned int __stdcall TcpServerNet::AcceptThread(void* lpvoid)
{
	TcpServerNet* pThis = (TcpServerNet*)lpvoid; //ǿת�ɵ�ǰ������thisָ��

	//����ͻ�����Ϣ��
	sockaddr_in addrClient;
	int addrClientSize = sizeof(addrClient);
	while (!pThis->m_isServerStop)//����ûֹͣ
	{
		SOCKET clientSocket = accept(pThis->m_listenSocket, (sockaddr*)&addrClient, &addrClientSize);//��������

		//���ڽ��յ��߳�ID
		unsigned int nThreadId = 0;
		//���������߳�
		HANDLE hThreadHandle = (HANDLE)_beginthreadex(NULL, 0, &RecvThread, pThis, 0, &nThreadId);
		if (hThreadHandle) //�����ɹ�
		{
			pThis->m_ThreadHandleList.push_back(hThreadHandle);//��ӵ��߳̾���б�

			pThis->m_mapThreadIdToSocket[nThreadId] = clientSocket;//��ӵ��߳�ID��socket��ӳ���
		}
	}

	return 0;
}
unsigned int __stdcall  TcpServerNet::RecvThread(void* lpvoid)
{
	TcpServerNet* pThis = (TcpServerNet*)lpvoid; //ǿת�ɵ�ǰ������thisָ��

	pThis->RecvData(); //���ý������ݵĺ���

	return 0;
}