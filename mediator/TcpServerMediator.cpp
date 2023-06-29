#include "TcpServerMediator.h"
#include "../net/TcpServerNet.h"

TcpServerMediator::TcpServerMediator()
{
	m_pNet = new TcpServerNet(this);
}
TcpServerMediator::~TcpServerMediator()
{
	//������Դ
	if (m_pNet)
	{
		delete m_pNet;
		m_pNet = nullptr;
	}
}
//��������
bool TcpServerMediator::OpenNet()
{
	if (!m_pNet->InitNet())
	{
		return false;
	}
	return true;

}
//�ر�����
void TcpServerMediator::CloseNet()
{
	m_pNet->UninitNet();
}
//��������
bool TcpServerMediator::SendData(long lSendIp, char* buf, int nLen)
{
	if (!m_pNet->SendData(lSendIp, buf, nLen))
	{
		return false;
	}
	return true;
}
//��������
void TcpServerMediator::DealData(long lSendIp, char* buf, int nLen)
{
	////std::cout << buf << std::endl;
	////SendData(lSendIp, buf, nLen);
	////TODO:�ѽ��յ������ݴ���kernel
	//CKernel::pKernel->dealData(lSendIp, buf, nLen);
}