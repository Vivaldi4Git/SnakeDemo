#pragma once
#include "INet.h"
#include <map>
#include <list>
using namespace std;
class TcpServerNet : public INet
{
public:
	~TcpServerNet();
	TcpServerNet(INetMediator* pMediator);
	//��ʼ������
	bool InitNet();
	//�ر�����
	void UninitNet();
	//��������
	bool SendData(long lSendIp, char* buf, int nLen);

protected:
	//��������
	void RecvData();

	bool m_isServerStop; //����ֹͣ��־
	SOCKET m_listenSocket;	//�������׽���

	map<unsigned int, SOCKET> m_mapThreadIdToSocket; //�߳�ID��socket��ӳ��
	list<HANDLE> m_ThreadHandleList; //�߳̾��

	static unsigned int __stdcall AcceptThread(void* lpvoid); //���������߳�
	static unsigned int __stdcall RecvThread(void* lpvoid); //�������ݵ��߳�
};