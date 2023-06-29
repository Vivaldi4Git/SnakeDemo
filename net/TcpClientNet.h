#pragma once
#include "INet.h"
#include "TcpClientMediator.h"
#include <map>
#include <list>
#include <mutex>
using namespace std;
class TcpClientNet : public INet
{
public:
    ~TcpClientNet();
    TcpClientNet(INetMediator* pMediator);
	//初始化网络
	bool InitNet();
	//关闭网络
	void UninitNet();
	//发送数据
	bool SendData(long lSendIp, char* buf, int nLen);

protected:
	//接收数据
	void RecvData();

    bool m_isClientStop; //客户端停止标志
    SOCKET m_clientSocket;	//客户端的套接字
    HANDLE m_threadHandle; //线程句柄
    time_t m_nLastHeartbeatTime; //心跳时间
    std::mutex m_heartbeatMutex; // 用于保护 m_nLastHeartbeatTime 的互斥锁

	static unsigned int __stdcall RecvThread(void* lpvoid); //接收数据的线程
};
