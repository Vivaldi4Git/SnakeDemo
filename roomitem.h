#ifndef ROOMITEM_H
#define ROOMITEM_H

#include <QWidget>
#include <QMouseEvent>
namespace Ui {
class roomitem;
}

class roomitem : public QWidget
{
    Q_OBJECT

public:
    explicit roomitem(QWidget *parent = 0);
    ~roomitem();
    void setInfo(int roomid, const QString &roomName);
    void mousePressEvent(QMouseEvent *event);
//    void addUserToRoom(int userID, const QString &userName);
private:
    Ui::roomitem *ui;
signals:
    void SIG_userJoinRoom(int roomid, QString& roomName);
private:
    int m_roomid;
    QString m_roomName;


};

#endif // ROOMITEM_H
