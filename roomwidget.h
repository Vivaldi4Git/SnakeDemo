#ifndef ROOMWIDGET_H
#define ROOMWIDGET_H

#include <QWidget>
#include <QVector>
#include "SnakeGameApi/multiplayersnakewidget.h"
namespace Ui {
class RoomWidget;
}

class RoomWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RoomWidget(QWidget *parent = 0);
    ~RoomWidget();
    void addPlayer(int playerid, QString& playerName);
    void keyPressEvent(QKeyEvent *event);
    void startGame(const GameRoomInfo& gameInfo, int playerid);
    void updateGame(const GameRoomInfo& gameInfo);
    void setGameOver();
signals:
    void SIG_playerLeave();
signals:
    void SIG_gameStart(QVector<int>& playerVector);
    void SIG_directionChanged(Direction);
private slots:
    void on_pb_leaving_clicked();

    void on_pb_startGame_clicked();

    void slot_directionChanged(Direction);

private:
    Ui::RoomWidget *ui;
    MultiPlayerSnakeWidget* m_snakeGameWnd;
};

#endif // ROOMWIDGET_H
