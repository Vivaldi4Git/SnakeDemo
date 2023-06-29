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


//初始化网络
bool TcpServerNet::InitNet()
{
	//1. 加载库
	WSADATA wsadata; //创建Windows Socket API
	SOCKADDR_IN serverAddr; //储存server的Socket对象的信息
	SOCKADDR_IN clientAddr; //储存server的Socket对象的信息
	char recvBuf[1024] = { 0 }; //接收数据的缓冲区
	char sendBuf[1024] = { 0 }; //发送数据的缓冲区
	int ret; //返回值

	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) //返回不为零初始化失败
	{
		std::cout << "Windows Socket API Startup FAILED:" << WSAGetLastError() << std::endl;
		return false;
	}

	//2. 创建套接字
	m_listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // 创建监听套接字
	if (m_listenSocket == INVALID_SOCKET) //失败
	{
		std::cout << "Windows Socket Build FAILED:" << WSAGetLastError() << std::endl;
		WSACleanup(); //卸载库
		return false;
	}

	//3. 绑定
	//设置sockaddr_in
	serverAddr.sin_family = AF_INET; //IPv4协议族
	serverAddr.sin_port = htons(12345); //端口号
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); //绑定所有可用IP
	if (SOCKET_ERROR == bind(m_listenSocket, (const sockaddr*)&serverAddr, sizeof(serverAddr)))//绑定IP和地址信息
	{
		std::cout << "Windows Socket bind FAILED:" << WSAGetLastError() << std::endl;
		closesocket(m_listenSocket); //关闭套接字
		WSACleanup(); //卸载库
		return false;
	}

	//4. 监听
	if (SOCKET_ERROR == listen(m_listenSocket, SOMAXCONN)) //开启监听
	{
		std::cout << "Windows Socket listen FAILED:" << WSAGetLastError() << std::endl;
		closesocket(m_listenSocket); //关闭套接字
		WSACleanup(); //卸载库
		return false;
	}
	std::cout << "Listening for incoming connections..." << std::endl;

	//5. 创建连接线程
	HANDLE hThreadHandle = (HANDLE)_beginthreadex(NULL, 0, &AcceptThread, this, 0, 0);
	if (hThreadHandle)//创建成功
	{
		m_ThreadHandleList.push_back(hThreadHandle);//放入句柄, 方便后续操作
	}
    return true;
}


//关闭网络 
void TcpServerNet::UninitNet()
{
	//1. 设置标志位表示服务器已关闭
	m_isServerStop = true;

	//2. 根据储存的线程句柄列表释放线程
	for (auto handleIterator = m_ThreadHandleList.begin(); handleIterator != m_ThreadHandleList.end();)
	{
		if (*handleIterator) //句柄存在
		{
			if (WAIT_TIMEOUT == WaitForSingleObject(*handleIterator, 1000)) //等待一段时间等待线程结束
			{
				TerminateThread(*handleIterator, -1); //终止线程
			}
			CloseHandle(*handleIterator);//关闭句柄
			*handleIterator = nullptr; //置空
		}
		handleIterator = m_ThreadHandleList.erase(handleIterator); //erase
	}

	//3. 关闭server套接字
	if (m_listenSocket && m_listenSocket != INVALID_SOCKET) //检查
	{
		closesocket(m_listenSocket);
	}

	//4. 根据线程ID关闭客户端套接字
	for (auto IDIterator = m_mapThreadIdToSocket.begin(); IDIterator != m_mapThreadIdToSocket.end();)
	{
		if (IDIterator->second && IDIterator->second != INVALID_SOCKET) //检查
		{
			closesocket(IDIterator->second);
		}
		IDIterator = m_mapThreadIdToSocket.erase(IDIterator); //erase
	}

	//5. 卸载库
	WSACleanup();

}
//发送数据
bool TcpServerNet::SendData(long lSendIp, char* buf, int nLen)
{
	//1. 检测传入的参数是否合法
	if (!buf || nLen < 0) //buf为空说明没有数据可发送， nLen < 0
	{
		std::cout << "parameter error. " << std::endl;
		return false;
	}

	//2. 先发包大小
	if (send(lSendIp, (char*) & nLen, sizeof(int), 0) <= 0) //发送出错
	{
		std::cout << "send error:" << WSAGetLastError() << std::endl;
		return false;
	}

	//3. 再发包内容
	if (send(lSendIp, buf, nLen, 0) <= 0) //发送出错
	{
		std::cout << "send error:" << WSAGetLastError() << std::endl;
		return false;
	}
    return true;
}
//接收数据
void TcpServerNet::RecvData()
{
	//1. 取出线程对应的socket
	SOCKET clientSocket = m_mapThreadIdToSocket[GetCurrentThreadId()];

	//2. 检查这个socket
	if (!clientSocket || clientSocket == INVALID_SOCKET)
	{
		return;
	}
	//3. 循环接收数据
	int nRecvNum = 0; //实际接收到的数据
	int packSize = 0; //包的大小
	int offset = 0; //偏移量
	while (!m_isServerStop)
	{
		//3.1 接收包大小
		nRecvNum = recv(clientSocket, (char*)&packSize, sizeof(int), 0);

		
		if (nRecvNum > 0)//接收到了数据
		{
			
			char* recvPackBuf = new char[packSize];//接收数据的缓冲区
			offset = 0;

			//3.2 接收包内容
			while (packSize > 0) //包大小不为0
			{
				nRecvNum = recv(clientSocket, recvPackBuf + offset, packSize, 0);//接收
				if (nRecvNum > 0) //接收到了数据
				{
					offset += nRecvNum; //更新偏移量
					packSize -= nRecvNum; //读取完后更新包大小
				}
			}
			//3.3 转发给中介者
			if (m_pMediator)
			{
				m_pMediator->DealData(clientSocket, recvPackBuf, nRecvNum);
			}
		}

		
	}

}

unsigned int __stdcall TcpServerNet::AcceptThread(void* lpvoid)
{
	TcpServerNet* pThis = (TcpServerNet*)lpvoid; //强转成当前类对象的this指针

	//保存客户端信息的
	sockaddr_in addrClient;
	int addrClientSize = sizeof(addrClient);
	while (!pThis->m_isServerStop)//服务没停止
	{
		SOCKET clientSocket = accept(pThis->m_listenSocket, (sockaddr*)&addrClient, &addrClientSize);//接收连接

		//用于接收的线程ID
		unsigned int nThreadId = 0;
		//创建接收线程
		HANDLE hThreadHandle = (HANDLE)_beginthreadex(NULL, 0, &RecvThread, pThis, 0, &nThreadId);
		if (hThreadHandle) //创建成功
		{
			pThis->m_ThreadHandleList.push_back(hThreadHandle);//添加到线程句柄列表

			pThis->m_mapThreadIdToSocket[nThreadId] = clientSocket;//添加到线程ID到socket的映射表
		}
	}

	return 0;
}
unsigned int __stdcall  TcpServerNet::RecvThread(void* lpvoid)
{
	TcpServerNet* pThis = (TcpServerNet*)lpvoid; //强转成当前类对象的this指针

	pThis->RecvData(); //调用接收数据的函数

	return 0;
}