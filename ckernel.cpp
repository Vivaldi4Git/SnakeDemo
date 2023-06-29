#include "ckernel.h"
#include <QMessageBox>
#include <QDebug>
#include "packdef.h"
#include <QTextCodec>
#include <QByteArray>
#include <QInputDialog>
#include <QDir>
#include <QTime>
#define NetPackMap(a)  m_NetPackMap[ a - DEF_PACK_BASE ]
//#define netPackFunMap(packName) m_netProtocolMap[packName - _DEF_PROTOCOL_BASE - 10]
CKernel::CKernel(QObject *parent) : QObject(parent)
{

    //登录注册窗口
    m_loginDialog = new LoginDialog;
    m_loginDialog->show();
    //绑定登录信息的信号和槽
    connect(m_loginDialog, SIGNAL(SIG_loginCommit(QString,QString)),
            this, SLOT(slot_loginCommit(QString,QString)));
    //绑定注册信息的信号和槽
    connect(m_loginDialog, SIGNAL(SIG_registerCommit(QString,QString,QString)),
            this, SLOT(slot_registerCommit(QString,QString,QString)));
    //绑定销毁的信号和槽
    connect(m_loginDialog, SIGNAL(SIG_close()), this, SLOT(slot_destroyInstance()));


    //主窗口
    m_mainWnd = new MainWindow;
    //绑定信号和槽
    connect(m_mainWnd, SIGNAL(SIG_userJoinRoom(int,QString&)),
            this, SLOT(slot_userJoinRoom(int,QString&)));

    connect(m_mainWnd, SIGNAL(SIG_getRoomList()),
            this, SLOT(slot_getRoomList()));
    //初始化网络中介者
    m_tcpClient = new TcpClientMediator;
    connect(m_tcpClient, SIGNAL(SIG_ReadyData(long,char*,int)),
            this, SLOT(slot_dealData(long,char*,int)));


    //打开网络
    if(!m_tcpClient->OpenNet())
    {
        QMessageBox::about(m_loginDialog, "warning", "open net failed");
        exit(0);
    }
    setProtocolMap(); //设置协议头函数映射表
}

CKernel::~CKernel()
{
    //回收注册&登录窗口
    if(m_loginDialog)
    {
        m_loginDialog->hide();
        delete m_loginDialog;
        m_loginDialog = nullptr;
    }


    if(m_tcpClient)
    {
        m_tcpClient->CloseNet();
        delete m_tcpClient;
        m_tcpClient = nullptr;
    }


    exit(0);
}

void CKernel::utf8ToGB2312(char *gbBuf, int nLen, QString &utf8)
{
    QTextCodec* gb2312Obj = QTextCodec::codecForName("gb2312"); //转化为待转码对象
    QByteArray ba = gb2312Obj->fromUnicode(utf8); //转化
    strcpy_s(gbBuf, nLen, ba.data()); //复制给gbBuf
}

QString CKernel::GB2312ToUtf8(char *gbBuf)
{
     QTextCodec* gb2312Obj = QTextCodec::codecForName("gb2312"); //转化为待转码对象
     return gb2312Obj->toUnicode(gbBuf);
}

CKernel *CKernel::GetInstance()
{
    static CKernel kernel;
    return &kernel;
}



void CKernel::setProtocolMap()
{
    //1. 初始化映射表
    memset(m_NetPackMap, 0, sizeof(m_NetPackMap));
    //2. 绑定函数地址到协议头
    NetPackMap(DEF_PACK_LOGIN_RS) = &CKernel::dealLoginRs;
    NetPackMap(DEF_PACK_REGISTER_RS) = &CKernel::dealRegisterRs;
    NetPackMap(DEF_PACK_FRIEND_INFO) = &CKernel::dealFriendInfoRq;
    NetPackMap(DEF_PACK_ROOM_INFO) = &CKernel::dealRoomInfo;
    NetPackMap(DEF_PACK_JOINROOM_RQ) = &CKernel::dealUserJoinRoomRq;
    NetPackMap(DEF_PACK_JOINROOM_RS) = &CKernel::dealUserJoinRoomRs;
    NetPackMap(DEF_PACK_GAME_START) = &CKernel::dealGameStart;
    NetPackMap(DEF_PACK_GAME_STATE) = &CKernel::dealGameState;
    NetPackMap(DEF_PACK_GAME_OVER) = &CKernel::dealGameOver;
    NetPackMap(DEF_GET_ROOM_LIST) = &CKernel::dealRoomList;
}


void CKernel::dealLoginRs(long lSendIp, char *buf, int nLen)
{
    qDebug() << __func__ << endl;
    //1. 拆包
    STRU_LOGIN_RS* rs = (STRU_LOGIN_RS*)buf;
    qDebug() << rs->m_lResult << endl;
    //2. 根据结果显示
    switch(rs->m_lResult)
    {
        case login_success:
            {
                m_loginDialog->hide();
                m_mainWnd->showNormal();

                m_UserID = rs->m_userid;


            }
            QMessageBox::about(this->m_loginDialog, "info", "登录成功");

        break;
        case user_not_exist:
            QMessageBox::about(this->m_loginDialog, "info", "登录失败， 用户不存在");
            break;
        case password_error:
            QMessageBox::about(this->m_loginDialog, "info", "登录失败， 密码错误");
            break;


    }
}
void CKernel::dealRegisterRs(long lSendIp, char* buf, int nLen)
{
    qDebug() << __func__ << endl;

    // 解析注册回复消息
    STRU_REGISTER_RS* rs = reinterpret_cast<STRU_REGISTER_RS*>(buf);
    int result = rs->m_lResult;

    // 根据注册结果显示相应的消息框
    switch (result)
    {
    case register_success:
        QMessageBox::about(m_loginDialog, "Info", "注册成功");
        break;
    case user_is_exist:
        QMessageBox::about(m_loginDialog, "Info", "用户已存在");
        break;
    case parameter_error:
        QMessageBox::about(m_loginDialog, "Info", "参数错误");
        break;
    default:
        QMessageBox::about(m_loginDialog, "Info", "注册失败");
        break;
    }
}

void CKernel::dealRoomInfo(long lSendIp, char *buf, int nLen)
{
    qDebug() << __func__ << endl;
    STRU_ROOM_INFO* roomInfo = reinterpret_cast<STRU_ROOM_INFO*>(buf);

    int roomID = roomInfo->m_RoomID;
    QString roomName = roomInfo->m_RoomName;

    m_mainWnd->addRoom(roomID, roomName);
}

void CKernel::dealUserJoinRoomRq(long lSendIp, char *buf, int nLen)
{
    qDebug() << __func__ << endl;
    STRU_JOINROOM_RQ* userInfo = reinterpret_cast<STRU_JOINROOM_RQ*>(buf);


    int roomID = userInfo->m_RoomID;
    int userID = userInfo->m_UserID;
    QString userName = userInfo->m_name;
    m_roomWnd->addPlayer(userID, userName);
    m_roomWnd->update();
}

void CKernel::dealUserJoinRoomRs(long lSendIp, char *buf, int nLen)
{
    qDebug() << __func__ << endl;
    STRU_JOINROOM_RS* userInfo = reinterpret_cast<STRU_JOINROOM_RS*>(buf);


    int roomID = userInfo->m_RoomID;
    int userID = userInfo->m_UserID;
    QString userName = userInfo->m_name;
    m_curJoinRoomid = roomID;
    m_mainWnd->hide();
    m_roomWnd = new RoomWidget;
    //玩家离开
    connect(m_roomWnd, SIGNAL(SIG_playerLeave()), this, SLOT(slot_playerLeave()));
    //开始游戏
    connect(m_roomWnd, SIGNAL(SIG_gameStart(QVector<int>&)), this, SLOT(slot_gameStart(QVector<int>&)));
    //方向改变
    connect(m_roomWnd, SIGNAL(SIG_directionChanged(Direction)), this, SLOT(slot_directionChanged(Direction)));
    m_roomWnd->addPlayer(userID, userName);
    m_roomWnd->show();



}

void CKernel::dealGameStart(long lSendIp, char *buf, int nLen)
{
    qDebug() << __func__ << endl;
    //1. 移动指针
    QByteArray byteArray(buf + sizeof(int), nLen - sizeof(int));
    //2. 取出房间信息
    GameRoomInfo gameinfo = deserializeGameInfo(byteArray);
    //4. 根据信息调整m_roomwnd
    m_roomWnd->startGame(gameinfo, m_UserID);

}

void CKernel::dealGameState(long lSendIp, char *buf, int nLen)
{
    qDebug() << __func__ << endl;
    //1. 移动指针
    QByteArray byteArray(buf + sizeof(int), nLen - sizeof(int));
    //2. 取出房间信息
    GameRoomInfo gameinfo = deserializeGameInfo(byteArray);
    //3. 更新房间
    m_roomWnd->updateGame(gameinfo);
}

void CKernel::dealGameOver(long lSendIp, char *buf, int nLen)
{
    qDebug() << __func__ << endl;
    STRU_GAME_OVER* gameoverInfo = reinterpret_cast<STRU_GAME_OVER*>(buf);
    m_roomWnd->setGameOver();
}

void CKernel::dealRoomList(long lSendIp, char *buf, int nLen)
{
    qDebug() << __func__ << endl;


    char* cur = buf;


    int header = *(int*)cur;
    cur += sizeof(int);

    int roomCount = *(int*)cur;
    cur += sizeof(int);

    QVector<int> roomIDs;

    for (int i = 0; i < roomCount; ++i) {
        int roomId = *(int*)cur;
        cur += sizeof(int);
        roomIDs.push_back(roomId);
    }

    m_mainWnd->refreshRoomList(roomIDs);

}

void CKernel::dealFriendInfoRq(long lSendIp, char *buf, int nLen)
{
    qDebug() << __func__ << endl;
    //1. 拆包
    STRU_FRIEND_INFO* userInfo = (STRU_FRIEND_INFO*)buf;

    //2. 编码格式转换
//    QString strFeeling = GB2312ToUtf8(userInfo->m_feeling);
//    QString strName = GB2312ToUtf8(userInfo->m_name);
//    QString strFeeling = userInfo->m_feeling;
    QString strName = userInfo->m_name;
    //3. 查看id是不是自己的
    qDebug() << m_UserID << endl;
    qDebug() << userInfo->m_userid << endl;
    //3.1 是，更新界面， 保存信息
    if(m_UserID == userInfo->m_userid)
    {
        m_mainWnd->setUserInfo(userInfo->m_name);

        m_name = strName;

        return;
    }
}


void CKernel::slot_dealData(long lSendIp, char *buf, int nLen)
{
    qDebug() << __func__ << endl;
    //1. 取出协议头
    int packType = *(int*)buf;

    //打印协议头
    qDebug() << "Received protocol header:" << packType << endl;
    //2. 检测协议头范围
   if( (packType >= DEF_PACK_BASE) && ( packType < DEF_PACK_BASE + DEF_PACK_COUNT) )
    {
        //3. 根据协议头调用对应的函数
        PFUN pf = NetPackMap(packType);
        if(pf)
        {
            (this->*pf)( lSendIp,buf, nLen);
        }
    }
}

void CKernel::slot_loginCommit(QString tel, QString password)
{
    //1. 转化后取出内容, 放入包中
   STRU_LOGIN_RQ rq;
   strcpy(rq.m_tel, tel.toStdString().c_str());
   strcpy(rq.m_password, password.toStdString().c_str());
   qDebug() << "login password: " << rq.m_password << endl;
   m_tcpClient->SendData(0, (char*)&rq, sizeof(rq));
}

void CKernel::slot_registerCommit(QString tel, QString password, QString nickName)
{
    //1. 转化后取出内容, 放入包中
   STRU_REGISTER_RQ rq;
   strcpy(rq.m_tel, tel.toStdString().c_str());
   strcpy(rq.m_password, password.toStdString().c_str());
   strcpy(rq.m_name, nickName.toStdString().c_str());

   m_tcpClient->SendData(0, (char*)&rq, sizeof(rq));
}

void CKernel::slot_playerLeave()
{
    qDebug() << __func__ << endl;
    m_roomWnd->hide();
    m_roomWnd->deleteLater();
    STRU_LEAVEROOM_RQ rq;
    rq.m_nUserId = m_UserID;
    rq.m_RoomId = m_curJoinRoomid;
    strcpy(rq.m_name, m_name.toStdString().c_str());
    m_tcpClient->SendData(0, (char*)&rq, sizeof(rq));
}

void CKernel::slot_gameStart(QVector<int>& playerVector)
{
    qDebug() << __func__ << endl;

    int playerCount = playerVector.size();
    STRU_PLAYER_LIST* playerList = (STRU_PLAYER_LIST*)new char[sizeof(STRU_PLAYER_LIST) + sizeof(int) * (playerCount - 1)];
    playerList->m_nType = DEF_PACK_PLAYER_LIST;
    playerList->m_RoomId = m_curJoinRoomid;
    playerList->m_playerCount = playerCount;

    for (int i = 0; i < playerCount; i++) {
        playerList->m_playerIDs[i] = playerVector[i];
        qDebug() << "Player ID: " << playerList->m_playerIDs[i];
    }

    int packSize = sizeof(int) * 3 + sizeof(int) * playerCount;
    m_tcpClient->SendData(0, (char*)playerList, packSize);

    delete[] (char*)playerList;
}

void CKernel::slot_directionChanged(Direction direction)
{
    qDebug() << __func__ << endl;
    // 发送改变方向的包
    DirectionChangePackage pack;
    pack.m_nType = DEF_PACK_DIRECTION_CHANGE;
    pack.roomId = m_curJoinRoomid;
    pack.playerId = m_UserID;
    pack.direction = static_cast<int>(direction);
     qDebug() << "playerId: " << pack.playerId << endl;
    //序列化
     m_tcpClient->SendData(0, (char*)&pack, sizeof(pack));
}

void CKernel::slot_getRoomList()
{
    qDebug() << __func__ << endl;
    int header = DEF_GET_ROOM_LIST;
    m_tcpClient->SendData(0, (char*)&header, sizeof(header));
}





void CKernel::slot_userJoinRoom(int roomid, QString &roomName)
{
    qDebug() << __func__ << endl;
    STRU_JOINROOM_RQ rq;
    rq.m_RoomID = roomid;
    rq.m_UserID = m_UserID;
    strcpy(rq.m_name, m_name.toStdString().c_str());
    m_tcpClient->SendData(0, (char*)&rq, sizeof(rq));
}
GameRoomInfo CKernel::deserializeGameInfo(const QByteArray& byteArray) {
    GameRoomInfo gameInfo;
    QDataStream dataStream(byteArray);
    dataStream >> gameInfo.roomid;
    dataStream >> gameInfo.playerCount;

    for (int i = 0; i < gameInfo.playerCount; i++) {
        SnakeInfo snakeInfo;
        dataStream >> snakeInfo.playerId;
        dataStream >> snakeInfo.length;
        dataStream >> snakeInfo.positions;
        dataStream >> snakeInfo.score;
        gameInfo.snakes.push_back(snakeInfo);
    }

    dataStream >> gameInfo.foodPosition;

    return gameInfo;
}
void CKernel::printGameInfo(const GameRoomInfo& gameInfo) {
    qDebug() << "Player Count: " << gameInfo.playerCount;
    qDebug() << "Snakes:";

    for (const SnakeInfo& snake : gameInfo.snakes) {
        qDebug() << "Player ID: " << snake.playerId;
        qDebug() << "Length: " << snake.length;
        qDebug() << "Positions:";

        for (const QPoint& position : snake.positions) {
            qDebug() << "  X: " << position.x() << ", Y: " << position.y();
        }

        qDebug() << "Score: " << snake.score;
    }

    qDebug() << "Food Position: X: " << gameInfo.foodPosition.x() << ", Y: " << gameInfo.foodPosition.y();
}
