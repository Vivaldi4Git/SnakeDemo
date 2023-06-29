#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "roomlistwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public:
    void setUserInfo(QString userName);
    void addRoom(int roomId, const QString& roomName);
    void addUserToRoom(int roomId, int userID, const QString& roomName);
    void refreshRoomList(QVector<int>& roomIDs);
signals:
    void SIG_userJoinRoom(int roomid, QString& roomName);
    void SIG_getRoomList();
public slots:
    void slot_userJoinRoom(int roomid, QString &roomName);
private slots:
    void on_pb_snake_clicked();
    void slot_getRoomList();
private:
    Ui::MainWindow *ui;
    // 创建RoomListWidget的实例
    RoomListWidget *m_roomListWidget;

};

#endif // MAINWINDOW_H
