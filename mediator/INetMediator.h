#pragma once
//#include "INet.h"
#include <iostream>
#include <QObject>
using namespace std;

class INet;
class INetMediator : public QObject
{
    Q_OBJECT
public:
	INetMediator() {}
	~INetMediator() {}
	//开启网络
	virtual bool OpenNet() = 0;
	//关闭网络
	virtual void CloseNet() = 0;
	//发送数据
	virtual bool SendData(long lSendlp, char* buf, int nLen) = 0;
	//处理数据
	virtual void DealData(long lSendlp, char* buf, int nLen) = 0;
protected:
	INet* m_pNet;
signals:
    void SIG_ReadyData(long lSendIp, char* buf, int nLen);
};
