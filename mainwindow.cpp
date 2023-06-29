#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStackedLayout>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 创建RoomListWidget的实例
    m_roomListWidget = new RoomListWidget(this);
    connect(m_roomListWidget, SIGNAL(SIG_userJoinRoom(int,QString&)), this, SLOT(slot_userJoinRoom(int,QString&)));
    connect(m_roomListWidget, SIGNAL(SIG_getRoomList()), this, SLOT(slot_getRoomList()));

    // 将RoomListWidget添加到StackedLayout中
    ui->wdg_stack->addWidget(m_roomListWidget);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUserInfo(QString userName)
{
    ui->lb_userName->setText(userName);
}

void MainWindow::addRoom(int roomId, const QString &roomName)
{
    m_roomListWidget->addRoom(roomId, roomName);
}

void MainWindow::addUserToRoom(int roomId, int userID, const QString &roomName)
{

}

void MainWindow::refreshRoomList(QVector<int> &roomIDs)
{
    m_roomListWidget->refreshRoomList(roomIDs);
}

void MainWindow::on_pb_snake_clicked()
{
    // 切换到RoomListWidget
    ui->wdg_stack->setCurrentWidget(m_roomListWidget);

}

void MainWindow::slot_getRoomList()
{
    //发送
    Q_EMIT SIG_getRoomList();
}
void MainWindow::slot_userJoinRoom(int roomid, QString &roomName)
{
    Q_EMIT SIG_userJoinRoom(roomid, roomName);
}
