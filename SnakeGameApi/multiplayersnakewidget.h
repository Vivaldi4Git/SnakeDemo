#ifndef MULTIPLAYERSNAKEWIDGET_H
#define MULTIPLAYERSNAKEWIDGET_H


#include "QWidget"
#include "packdef.h"
#include "multisnake.h"
#include "food.h"
#include "notificationwidget.h"
#include <QPaintEvent>
#include <QKeyEvent>
class MultiPlayerSnakeWidget : public QWidget
{
    Q_OBJECT
public:
    MultiPlayerSnakeWidget();
    explicit MultiPlayerSnakeWidget(const GameRoomInfo& gameInfo);
public:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent *event);
    void updateGame(const GameRoomInfo& gameInfo);
    int currentPlayerId() const;
    void setCurrentPlayerId(int currentPlayerId);

    void updateTopScore(int newScore);
    void drawTopScore(QPainter &painter);
    void setGameOver();
signals:
    void SIG_directionChanged(Direction);
protected:
    //    QList<MultiSnake*> m_snakes; // 存储多条蛇的列表
    //public slots:
    //    void slot_moveSnakes();
    QMap<int, MultiSnake*> m_snakes;  // 使用QMap来建立玩家ID与蛇对象的映射关系
    Food m_food;
    int m_currentPlayerId;  // 当前玩家的ID
    int topScore = 0;  // 初始化为 0 或者其他合适的值
    bool gameIsOver;
    NotificationWidget* m_notificationWidget;
    QString topScorer;
};

#endif // MULTIPLAYERSNAKEWIDGET_H
