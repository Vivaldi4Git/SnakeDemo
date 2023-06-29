#pragma once
#include <QPoint>
#include <QVector>
#include <QList>
#define _DEF_TCP_PORT (8000)
#define _DEF_TCP_SERVER_IP ("192.168.182.133")
#define _DEF_NAME_SIZE (100)
#define _DEF_MAX_SIZE (4096)

//协议
#define DEF_PACK_BASE  (10000)
#define DEF_PACK_COUNT  (100)
//注册
#define  DEF_PACK_REGISTER_RQ    (DEF_PACK_BASE + 0)
#define  DEF_PACK_REGISTER_RS    (DEF_PACK_BASE + 1)
//登录
#define  DEF_PACK_LOGIN_RQ    (DEF_PACK_BASE + 2)
#define  DEF_PACK_LOGIN_RS    (DEF_PACK_BASE + 3)
#define  DEF_PACK_FRIEND_INFO (DEF_PACK_BASE + 4)
//创建房间
#define DEF_PACK_CREATEROOM_RQ  (DEF_PACK_BASE + 5)
#define DEF_PACK_CREATEROOM_RS  (DEF_PACK_BASE + 6)
//加入房间
#define DEF_PACK_JOINROOM_RQ  (DEF_PACK_BASE + 7)
#define DEF_PACK_JOINROOM_RS  (DEF_PACK_BASE + 8)
//房间列表请求
#define DEF_PACK_ROOM_MEMBER    (DEF_PACK_BASE + 9)
//房间信息请求
#define DEF_PACK_ROOM_INFO    (DEF_PACK_BASE + 10)
////音频数据
//#define DEF_PACK_AUDIO_FRAME    (DEF_PACK_BASE + 9)
////视频数据
//#define DEF_PACK_VIDEO_FRAME    (DEF_PACK_BASE + 10)
//退出房间请求
#define DEF_PACK_LEAVEROOM_RQ   (DEF_PACK_BASE + 11)
//退出房间回复
#define DEF_PACK_LEAVEROOM_RS   (DEF_PACK_BASE + 12)
//玩家列表
#define DEF_PACK_PLAYER_LIST   (DEF_PACK_BASE + 13)
//游戏开始
#define DEF_PACK_GAME_START   (DEF_PACK_BASE + 14)
//方向改变
#define DEF_PACK_DIRECTION_CHANGE    (DEF_PACK_BASE + 15)
//当前游戏状态
#define DEF_PACK_GAME_STATE    (DEF_PACK_BASE + 16)
//游戏结束
#define DEF_PACK_GAME_OVER    (DEF_PACK_BASE + 17)
//获取房间列表
#define DEF_GET_ROOM_LIST    (DEF_PACK_BASE + 18)
//心跳包
#define DEF_PACK_HEARTBEAT (DEF_PACK_BASE + 25)

//请求结构
//注册请求结果
#define register_success	(0)
#define user_is_exist			(1)
#define parameter_error	(3)
//登录请求结果
#define user_not_exist		(0)
#define password_error		(1)
#define login_success		(3)
//用户状态
#define status_online			(1)
#define status_offline		(0)
//加入房间结果
#define room_no_exist        0
#define join_success         1
#define join_failed          2
//离开房间结果
#define room_no_exist        0
#define leave_success         1
#define leave_failed          2
//添加好友结果
#define add_success			(0)
#define no_this_user			(1)
#define user_refuse			(2)
#define user_offline			(3)
//聊天请求结果
#define send_success			(0)
#define send_fail			(1)
#define user_offline			(3)
//文件发送结果
#define file_send_success   (0)
#define file_send_failed    (1)
#define friend_download_success (2)
#define friend_download_failed (3)
//挂断结果
#define user_disconnect_failed (0)
#define user_disconnect_from_channel (1)

#define MAX_PATH            (260 )
#define MAX_SIZE            (40  )
#define MAX_CONTENT_LEN     (4096 )

#define THEME_COLOR "#5663f0"
#define HEARTBEAT_INTERVAL 30
//协议结构
typedef int PackType;
//注册请求: 协议头， 昵称， 手机号， 密码
typedef struct STRU_REGISTER_RQ
{
    STRU_REGISTER_RQ() :m_nType(DEF_PACK_REGISTER_RQ)
    {
        memset(m_name, 0, MAX_SIZE);
        memset(m_tel, 0, MAX_SIZE);
        memset(m_password, 0, MAX_SIZE);
    }
    PackType m_nType;
    char m_name[MAX_SIZE];
    char m_tel[MAX_SIZE];
    char m_password[MAX_SIZE];
}STRU_REGISTER_RQ;
//注册回复: 协议头，注册的结果
typedef struct STRU_REGISTER_RS
{
    STRU_REGISTER_RS() :m_nType(DEF_PACK_REGISTER_RS), m_lResult(register_success)
    {

    }
    PackType m_nType;
    int m_lResult;
}STRU_REGISTER_RS;
//登录请求: 协议头，手机号， 密码
typedef struct STRU_LOGIN_RQ
{
    STRU_LOGIN_RQ() :type(DEF_PACK_LOGIN_RQ)
    {
        memset(m_tel, 0, MAX_SIZE);
        memset(m_password, 0, MAX_SIZE);
    }
    PackType type;
    char m_tel[MAX_SIZE];
    char m_password[MAX_SIZE];
}STRU_LOGIN_RQ;
//登录回复: 协议头，登陆结果， 用户id
typedef struct STRU_LOGIN_RS
{
    STRU_LOGIN_RS() :m_nType(DEF_PACK_LOGIN_RS), m_lResult(login_success), m_userid(0)
    {

    }
    PackType m_nType;
    int m_lResult;
    int m_userid;
}STRU_LOGIN_RS;
//获取好友信息: 协议头，用户id、 昵称、 签名、头像id、 用户状态
typedef struct STRU_FRIEND_INFO
{
    STRU_FRIEND_INFO() :m_nType(DEF_PACK_FRIEND_INFO), m_userid(0)
    {
        memset(m_name, 0, MAX_SIZE);
//        memset(m_feeling, 0, MAX_SIZE);
    }
    PackType m_nType;
    int m_userid;
//    int m_iconid;
//    int m_state;
    char m_name[MAX_SIZE];
//    char m_feeling[MAX_SIZE];
}STRU_FRIEND_INFO;
//房间信息请求
typedef struct STRU_ROOM_INFO
{
    STRU_ROOM_INFO()
    {
        m_nType= DEF_PACK_ROOM_INFO;
        m_RoomID = 0;
        memset(m_RoomName,0,MAX_SIZE);
    }
    PackType m_nType;   //包类型
    int m_RoomID;
    char m_RoomName[MAX_SIZE];
}STRU_ROOM_INFO;
typedef struct STRU_JOINROOM_RQ
{
    STRU_JOINROOM_RQ()
    {
        m_nType = DEF_PACK_JOINROOM_RQ;
        m_UserID = 0;
        m_RoomID = 0;
        memset(m_name,0,MAX_SIZE);
    }

    PackType m_nType;   //包类型
    int m_UserID;
    int m_RoomID;
    char m_name[MAX_SIZE];
}STRU_JOINROOM_RQ;

//加入房间回复
typedef struct STRU_JOINROOM_RS
{
    STRU_JOINROOM_RS()
    {
        m_nType= DEF_PACK_JOINROOM_RS;
        m_lResult = 0;
        m_RoomID = 0;
        m_UserID = 0;
        memset(m_name,0,MAX_SIZE);
    }
    PackType m_nType;   //包类型
    int  m_lResult ;
    int m_RoomID;
    int m_UserID;
    char m_name[MAX_SIZE];
}STRU_JOINROOM_RS;
//离开房间请求
typedef struct STRU_LEAVEROOM_RQ
{
    STRU_LEAVEROOM_RQ()
    {
        m_nType = DEF_PACK_LEAVEROOM_RQ;
        m_nUserId = 0;
        m_RoomId = 0;
        memset(m_name,0,MAX_SIZE);
    }
    PackType   m_nType;   //包类型
    int    m_nUserId; //用户ID
    int    m_RoomId;
    char   m_name[MAX_SIZE];
}STRU_LEAVEROOM_RQ;

// 玩家列表信息
typedef struct STRU_PLAYER_LIST
{
    STRU_PLAYER_LIST() : m_nType(DEF_PACK_PLAYER_LIST), m_RoomId(0), m_playerCount(0) {}
    PackType m_nType;          // 包类型
    int m_RoomId;
    int m_playerCount;
    int m_playerIDs[1]; // 使用动态数组存储玩家ID，实际长度由 m_playerCount 决定
} STRU_PLAYER_LIST;

//蛇的信息
struct SnakeInfo
{
    int playerId;
    int length;
    QVector<QPoint> positions;
    int score;
};
//游戏房间信息
struct GameRoomInfo
{
    int roomid;
    int playerCount;
    QVector<SnakeInfo> snakes;
    QPoint foodPosition;
};

enum class Direction {
    Up,
    Down,
    Left,
    Right
};

struct DirectionChangePackage {
    PackType m_nType;     // 包类型，取值为DEF_PACK_DIRECTION_CHANGE
    int roomId;          // 游戏房间ID
    int playerId;        // 玩家ID
    int direction;       // 移动方向，使用枚举或整数表示
};
struct STRU_GAME_OVER
{
    STRU_GAME_OVER() : m_nType(DEF_PACK_GAME_OVER), m_RoomId(0){}

    int m_nType;
    int m_RoomId;
};
// 心跳包结构体
typedef struct STRU_HEARTBEAT
{
    STRU_HEARTBEAT() : m_nType(DEF_PACK_HEARTBEAT)
    {

    }
    PackType m_nType;
} STRU_HEARTBEAT;
//// 音频群聊请求: 协议头，用户id，群组id，频道id，音频内容
//typedef struct STRU_GROUP_CHAT_AUDIO_RQ
//{
//    STRU_GROUP_CHAT_AUDIO_RQ() : m_nType(DEF_PACK_GROUP_CHAT_AUDIO_RQ), m_userid(0), m_groupid(0), m_channelid(0)
//    {

//    }
//    PackType m_nType;
//    int m_userid;
//    int m_groupid;
//    int m_channelid;
//    //时间， 用于同步
//    int hour;
//    int min;
//    int sec;
//    int msec;
//    QByteArray m_audioData; // 音频内容
//} STRU_GROUP_CHAT_AUDIO_RQ;
// 音频群聊回复: 协议头，请求结果，用户id，群组id，频道id
//typedef struct STRU_GROUP_CHAT_AUDIO_RS
//{
//    STRU_GROUP_CHAT_AUDIO_RS() : m_nType(DEF_PACK_GROUP_CHAT_AUDIO_RS), m_userid(0), m_groupid(0), m_channelid(0), m_lResult(send_success)
//    {

//    }
//    PackType m_nType;
//    int m_userid;
//    int m_groupid;
//    int m_channelid;
//    int m_lResult;
//} STRU_GROUP_CHAT_AUDIO_RS;
////加入音频房间请求
//typedef struct STRU_GROUP_CHAT_AUDIO_USERJOIN_RQ
//{
//    STRU_GROUP_CHAT_AUDIO_USERJOIN_RQ() : m_nType(DEF_PACK_GROUP_CHAT_AUDIO_USERJOIN_RQ), m_userid(0), m_channelid(0), m_groupid(0)
//    {

//    }
//    PackType m_nType;
//    int m_userid;
//    int m_channelid;
//    int m_groupid;
//} STRU_GROUP_CHAT_AUDIO_USERJOIN_RQ;
//// 加入音频房间响应
//typedef struct STRU_GROUP_CHAT_AUDIO_USERJOIN_RS
//{
//    STRU_GROUP_CHAT_AUDIO_USERJOIN_RS() : m_nType(DEF_PACK_GROUP_CHAT_AUDIO_USERJOIN_RS), m_userid(0), m_channelid(0), m_groupid(0), m_lResult(false) {

//    }
//    PackType m_nType;
//    int m_userid;
//    int m_channelid;
//    int m_groupid;
//    bool m_lResult;
//} STRU_GROUP_CHAT_AUDIO_USERJOIN_RS;
//// 用户挂断请求
//typedef struct STRU_USER_DISCONNECT_RQ
//{
//    STRU_USER_DISCONNECT_RQ() : m_nType(DEF_PACK_USER_DISCONNECT_RQ), m_userid(0), m_channelid(0), m_groupid(0)
//    {

//    }
//    PackType m_nType;
//    int m_userid;
//    int m_channelid;
//    int m_groupid;
//} STRU_USER_DISCONNECT_RQ;

//// 用户挂断响应
//typedef struct STRU_USER_DISCONNECT_RS {
//    STRU_USER_DISCONNECT_RS() : m_nType(DEF_PACK_USER_DISCONNECT_RS), m_userid(0), m_channelid(0), m_groupid(0), m_lResult(false) {
//    }
//    PackType m_nType;
//    int m_userid;
//    int m_channelid;
//    int m_groupid;
//    int m_lResult;

//} STRU_USER_DISCONNECT_RS;
