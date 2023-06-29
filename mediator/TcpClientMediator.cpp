#include "TcpClientMediator.h"
#include "TcpClientNet.h"
TcpClientMediator::TcpClientMediator()
{
	m_pNet = new TcpClientNet(this);
}
TcpClientMediator::~TcpClientMediator()
{
	//回收资源
	if (m_pNet)
	{
		delete m_pNet;
		m_pNet = NULL;
	}
}
//开启网络
bool TcpClientMediator::OpenNet()
{
	if (!m_pNet->InitNet())
	{
		return false;
	}
	return true;

}
//关闭网络
void TcpClientMediator::CloseNet()
{
	m_pNet->UninitNet();
}
//发送数据
bool TcpClientMediator::SendData(long lSendIp, char* buf, int nLen)
{
	if (!m_pNet->SendData(lSendIp, buf, nLen))
	{
		return false;
	}
	return true;
}
//处理数据
void TcpClientMediator::DealData(long lSendIp, char* buf, int nLen)
{
    //把接收到的数据传给kernel
    Q_EMIT SIG_ReadyData(lSendIp, buf, nLen);
}
