#include "roomlistwidget.h"
#include "ui_roomlistwidget.h"
#include "roomitem.h"
RoomListWidget::RoomListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoomListWidget)
{
    ui->setupUi(this);
    // 创建垂直布局
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(10, 10, 10, 10);
    m_layout->setSpacing(5); // 设置间距为5像素
    m_layout->setAlignment(Qt::AlignTop);

    setLayout(m_layout);
}

RoomListWidget::~RoomListWidget()
{
    delete ui;
}

void RoomListWidget::addRoom(int roomId, const QString &roomName)
{
    roomitem* roomItem = new roomitem;
    roomItem->setInfo(roomId, roomName);
    connect(roomItem, SIGNAL(SIG_userJoinRoom(int,QString&)), this, SLOT(slot_userJoinRoom(int,QString&)));

    // 设置 roomitem 控件的大小策略和约束
    roomItem->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed); // 水平扩展，垂直固定
    roomItem->setMinimumHeight(50); // 设置最小高度为50像素

    m_layout->addWidget(roomItem);
    m_mapRoomidToRoomItem[roomId] = roomItem;
}

void RoomListWidget::refreshRoomList(QVector<int> &roomIds)
{
    // 根据房间ID排序 RoomItem
    QVector<roomitem*> sortedRoomItems;
    for (int i = 0; i < roomIds.size(); ++i) {
        int roomId = roomIds[i];
        roomitem* roomItem = m_mapRoomidToRoomItem[roomId];
        sortedRoomItems.append(roomItem);
    }

    // 移除布局中的 RoomItem
    for (int i = 0; i < sortedRoomItems.size(); ++i) {
        roomitem* roomItem = sortedRoomItems[i];
        m_layout->removeWidget(roomItem);
    }

    // 根据排序后的 RoomItem 重新添加到布局中
    for (int i = 0; i < sortedRoomItems.size(); ++i) {
        roomitem* roomItem = sortedRoomItems[i];
        m_layout->addWidget(roomItem);
    }
}



//void RoomListWidget::addUserToRoom(int roomId, int userID, const QString &userName)
//{
//    //找出对应的RoomItem
//    if(m_mapRoomidToRoomItem.find(roomId) != m_mapRoomidToRoomItem.end())
//    {
//        //加入对应的用户信息
//        roomitem* roomItem = m_mapRoomidToRoomItem[roomId];
//        roomItem->add
//    }
//}

void RoomListWidget::slot_userJoinRoom(int roomid, QString &roomName)
{
    Q_EMIT SIG_userJoinRoom(roomid, roomName);
}

void RoomListWidget::on_pb_refresh_clicked()
{
    Q_EMIT SIG_getRoomList();
}
