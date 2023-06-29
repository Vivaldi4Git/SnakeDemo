#ifndef ROOMLISTWIDGET_H
#define ROOMLISTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include "roomitem.h"
namespace Ui {
class RoomListWidget;
}

class RoomListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RoomListWidget(QWidget *parent = 0);
    ~RoomListWidget();
    void addRoom(int roomId, const QString& roomName);
//    void addUserToRoom(int roomId, int userID, const QString &userName);
    void refreshRoomList(QVector<int>& roomIds);
private:
    Ui::RoomListWidget *ui;
    QVBoxLayout* m_layout;
    QMap<int, roomitem*> m_mapRoomidToRoomItem;
public slots:
    void slot_userJoinRoom(int roomid, QString& roomName);
signals:
    void SIG_userJoinRoom(int roomid, QString& roomName);
    void SIG_getRoomList();
private slots:
    void on_pb_refresh_clicked();
};

#endif // ROOMLISTWIDGET_H
