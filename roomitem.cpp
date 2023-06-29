#include "roomitem.h"
#include "ui_roomitem.h"
#include <QHBoxLayout>
#include <QDebug>
roomitem::roomitem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::roomitem)
{
    ui->setupUi(this);


    // 设置小部件的样式
    setStyleSheet("background-color: white; border: 1px solid gray;");
}

roomitem::~roomitem()
{
    delete ui;
}

void roomitem::setInfo(int roomid, const QString &roomName)
{
    ui->lb_roomName->setText(roomName);
    m_roomName = roomName;
    m_roomid = roomid;
}
void roomitem::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // 在这里处理房间项被点击的逻辑
        // 可以发出信号通知其他部分处理点击事件
//        emit clicked(roomId, roomName);
        qDebug() << "pressOn: " << m_roomName << endl;
        Q_EMIT SIG_userJoinRoom(m_roomid, m_roomName);
    }
    QWidget::mousePressEvent(event);
}


