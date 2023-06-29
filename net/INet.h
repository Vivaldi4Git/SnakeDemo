#pragma once

#include <WinSock2.h>
#include <iostream>
using namespace std;
class INetMediator;		//通过声明类来防止循环包含的问题
class INet
{
public:
	INet() {}
	virtual ~INet() {}	//使用虚析构， 父类指针指向子类对象， 需要使用虚析构防止内存泄漏
	//初始化网络
	virtual bool InitNet() = 0;
	//关闭网络
	virtual void UninitNet() = 0;
	//发送数据
	virtual bool SendData(long lSendlp, char* buf, int nLen) = 0;

protected:
	//接收数据
	virtual void RecvData() = 0;
	INetMediator* m_pMediator;	//中介者类指针， 用于将数据传递给中介者类
};
