#ifndef CKERNEL_H
#define CKERNEL_H

#include <QObject>

#include "logindialog.h"
#include "mainwindow.h"
#include "TcpClientMediator.h"
#include "packdef.h"
#include "roomwidget.h"
#include <QMap>
class CKernel;
typedef void (CKernel::*PFUN) (long lSendIp, char* buf, int nLen);
class CKernel : public QObject
{
    Q_OBJECT
public:

    explicit CKernel(QObject *parent = 0);
    ~CKernel();
public:
    //将utf8的QString转化为GB2312的char*
    static void utf8ToGB2312(char* gbBuf, int nLen, QString &utf8);
    //将GB2312的char*的QString转化为utf8
    static QString GB2312ToUtf8(char* gbBuf);
public:
    //单例模式
    static CKernel* GetInstance();
    //处理登录回复
    void dealLoginRs(long lSendIp, char* buf, int nLen);
    //处理注册回复
    void dealRegisterRs(long lSendIp, char* buf, int nLen);
    //处理房间信息
    void dealRoomInfo(long lSendIp, char* buf, int nLen);

    //处理用户加入请求
    void dealUserJoinRoomRq(long lSendIp, char* buf, int nLen);
    //处理用户加入回复
    void dealUserJoinRoomRs(long lSendIp, char* buf, int nLen);
    //处理游戏开始信息
    void dealGameStart(long lSendIp, char* buf, int nLen);
    //处理游戏信息
    void dealGameState(long lSendIp, char* buf, int nLen);
    //处理游戏结束信息
    void dealGameOver(long lSendIp, char* buf, int nLen);
    //处理房间列表
    void dealRoomList(long lSendIp, char* buf, int nLen);

    void dealFriendInfoRq(long lSendIp, char *buf, int nLen);
    GameRoomInfo deserializeGameInfo(const QByteArray &byteArray);
    void printGameInfo(const GameRoomInfo &gameInfo);
private:
    void setProtocolMap();//设置协议映射表

signals:
public slots:
    void slot_dealData(long lSendIp, char* buf, int nLen);

    //处理登录信息提交的槽
    void slot_loginCommit(QString tel, QString password);
    //处理注册信息提交的槽
    void slot_registerCommit(QString tel, QString password, QString nickName);
    //玩家退出房间的槽
    void slot_playerLeave();
    //开始游戏的槽
    void slot_gameStart(QVector<int>& playerVector);
    //方向改变
    void slot_directionChanged(Direction direction);
    //获取房间列表
    void slot_getRoomList();
public slots:
    void slot_userJoinRoom(int roomid, QString &roomName);

private:

    LoginDialog* m_loginDialog; //登录窗口
    MainWindow* m_mainWnd; //主界面
    RoomWidget* m_roomWnd; //房间界面
    //网络中介者
    INetMediator* m_tcpClient;
    //个人信息
    int m_UserID;//当前用户id
    QString m_name;
    int m_curJoinRoomid;

private:
    PFUN m_NetPackMap[DEF_PACK_COUNT]; //协议头 - >函数指针映射表

};

#endif // CKERNEL_H
