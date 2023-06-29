#include "TcpClientNet.h"
#include "../mediator/TcpServerMediator.h"
#include <process.h>
#include "packdef.h"
using namespace std;
TcpClientNet::TcpClientNet(INetMediator* pMediator):m_clientSocket(INVALID_SOCKET), m_isClientStop(false)
{
    m_pMediator = pMediator;

}
TcpClientNet::~TcpClientNet()
{

}


//初始化网络
bool TcpClientNet::InitNet()
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
    m_clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // 创建监听套接字
    if (m_clientSocket == INVALID_SOCKET) //失败
	{
		std::cout << "Windows Socket Build FAILED:" << WSAGetLastError() << std::endl;
		WSACleanup(); //卸载库
		return false;
	}
    //2.1 设置nodelay
    int enable = 1;
    setsockopt(m_clientSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&enable, sizeof(enable));
    //3. 连接
	//设置sockaddr_in
	serverAddr.sin_family = AF_INET; //IPv4协议族
    serverAddr.sin_port = htons(_DEF_TCP_PORT); //端口号
    serverAddr.sin_addr.s_addr = inet_addr(_DEF_TCP_SERVER_IP); //绑定服务端IP
    ret = connect(m_clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (SOCKET_ERROR == ret)
    {
        cout << "connect error: " << WSAGetLastError() << endl;
        return false;
    }
    cout << "connect successs. " << endl;

//    // 初始化心跳时间

//    m_nLastHeartbeatTime = std::time(nullptr);
    //5. 创建接收线程
    m_threadHandle = (HANDLE)_beginthreadex(NULL, 0, &RecvThread, this, 0, 0);

    return true;
}


//关闭网络 
void TcpClientNet::UninitNet()
{
	//1. 设置标志位表示服务器已关闭
    m_isClientStop = true;

    //2. 释放线程
    if (m_threadHandle) //句柄存在
    {
        if (WAIT_TIMEOUT == WaitForSingleObject(m_threadHandle, 1000)) //等待一段时间等待线程结束
        {
            TerminateThread(m_threadHandle, -1); //终止线程
        }
        CloseHandle(m_threadHandle);//关闭句柄
        m_threadHandle = nullptr; //置空
    }

    //3. 关闭client套接字
    if (m_clientSocket && m_clientSocket != INVALID_SOCKET) //检查
	{
        closesocket(m_clientSocket);
	}
    //4. 卸载库
	WSACleanup();

}
//发送数据
bool TcpClientNet::SendData(long lSendIp, char* buf, int nLen)
{
	//1. 检测传入的参数是否合法
    if (!buf || nLen < 0) //buf为空说明没有数据可发送， nLen < =0
	{
		std::cout << "parameter error. " << std::endl;
		return false;
	}

	//2. 先发包大小
    if (send(m_clientSocket, (char*) & nLen, sizeof(int), 0) <= 0) //发送出错
	{
		std::cout << "send error:" << WSAGetLastError() << std::endl;
		return false;
	}

	//3. 再发包内容
    if (send(m_clientSocket, buf, nLen, 0) <= 0) //发送出错
	{
		std::cout << "send error:" << WSAGetLastError() << std::endl;
		return false;
	}
//    // 更新上次发送数据的时间
//    {
//        std::lock_guard<std::mutex> guard(m_heartbeatMutex);
//        m_nLastHeartbeatTime = std::time(nullptr);
//    }

    return true;
}
//接收数据
void TcpClientNet::RecvData()
{
    //1. 循环接收数据
	int nRecvNum = 0; //实际接收到的数据
	int packSize = 0; //包的大小
	int offset = 0; //偏移量
    while (!m_isClientStop)
	{
        //1.1 接收包大小
        nRecvNum = recv(m_clientSocket, (char*)&packSize, sizeof(int), 0);

		
		if (nRecvNum > 0)//接收到了数据
		{
            // 输出客户端socket和包大小
            std::cout << "Received " << nRecvNum << " bytes of data from server " << m_clientSocket << ", pack size is " << packSize << std::endl;
			char* recvPackBuf = new char[packSize];//接收数据的缓冲区
			offset = 0;


            //1.2 接收包内容
			while (packSize > 0) //包大小不为0
			{
                nRecvNum = recv(m_clientSocket, recvPackBuf + offset, packSize, 0);//接收
				if (nRecvNum > 0) //接收到了数据
				{
					offset += nRecvNum; //更新偏移量
					packSize -= nRecvNum; //读取完后更新包大小
				}
			}
			//3.3 转发给中介者
			if (m_pMediator)
			{
                m_pMediator->DealData(m_clientSocket, recvPackBuf, nRecvNum);
			}
//            // 更新上次接收数据的时间
//            {
//                std::lock_guard<std::mutex> guard(m_heartbeatMutex);
//                m_nLastHeartbeatTime = std::time(nullptr);
//            }

		}
//        if(nRecvNum == 0) //没接收到数据
//        {
//            // 检查是否需要发送心跳包
//            {
//                std::lock_guard<std::mutex> guard(m_heartbeatMutex);
//                std::time_t currentTime = std::time(nullptr);
//                if (currentTime - m_nLastHeartbeatTime >= HEARTBEAT_INTERVAL) {
//                    // 发送心跳包逻辑...
//                    STRU_HEARTBEAT heartbeat_pack;
//                    m_pMediator->SendData(0, (char*)& heartbeat_pack, sizeof(heartbeat_pack));
//                    std::cout << "heartbeat pack send" <<endl;
//                    m_nLastHeartbeatTime = currentTime; // 更新最后一次发送心跳包的时间
//                }
//            }
//        }

		
	}

}


unsigned int __stdcall  TcpClientNet::RecvThread(void* lpvoid)
{
    TcpClientNet* pThis = (TcpClientNet*)lpvoid; //强转成当前类对象的this指针

	pThis->RecvData(); //调用接收数据的函数

	return 0;
}
