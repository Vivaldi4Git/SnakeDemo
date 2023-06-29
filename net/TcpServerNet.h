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
	//初始化网络
	bool InitNet();
	//关闭网络
	void UninitNet();
	//发送数据
	bool SendData(long lSendIp, char* buf, int nLen);

protected:
	//接收数据
	void RecvData();

	bool m_isServerStop; //服务停止标志
	SOCKET m_listenSocket;	//监听的套接字

	map<unsigned int, SOCKET> m_mapThreadIdToSocket; //线程ID到socket的映射
	list<HANDLE> m_ThreadHandleList; //线程句柄

	static unsigned int __stdcall AcceptThread(void* lpvoid); //接受连接线程
	static unsigned int __stdcall RecvThread(void* lpvoid); //接收数据的线程
};