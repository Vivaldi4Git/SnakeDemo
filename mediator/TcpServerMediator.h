#pragma once
#include "INetMediator.h"
class TcpServerMediator : public INetMediator
{
public:
	TcpServerMediator();
	~TcpServerMediator();
	//��������
	bool OpenNet();
	//�ر�����
	void CloseNet();
	//��������
	bool SendData(long lSendIp, char* buf, int nLen);
	//��������
	void DealData(long lSendIp, char* buf, int nLen);
};