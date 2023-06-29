#include "TcpServerMediator.h"
#include "../net/TcpServerNet.h"

TcpServerMediator::TcpServerMediator()
{
	m_pNet = new TcpServerNet(this);
}
TcpServerMediator::~TcpServerMediator()
{
	//回收资源
	if (m_pNet)
	{
		delete m_pNet;
		m_pNet = nullptr;
	}
}
//开启网络
bool TcpServerMediator::OpenNet()
{
	if (!m_pNet->InitNet())
	{
		return false;
	}
	return true;

}
//关闭网络
void TcpServerMediator::CloseNet()
{
	m_pNet->UninitNet();
}
//发送数据
bool TcpServerMediator::SendData(long lSendIp, char* buf, int nLen)
{
	if (!m_pNet->SendData(lSendIp, buf, nLen))
	{
		return false;
	}
	return true;
}
//处理数据
void TcpServerMediator::DealData(long lSendIp, char* buf, int nLen)
{
	////std::cout << buf << std::endl;
	////SendData(lSendIp, buf, nLen);
	////TODO:把接收到的数据传给kernel
	//CKernel::pKernel->dealData(lSendIp, buf, nLen);
}