#ifndef SNAKE_H
#define SNAKE_H
#include <QList>
#include <QPoint>
#include <QPainter>
enum class Direction {
    Up,
    Down,
    Left,
    Right
};
class Snake {
public:
    Snake();
    Snake(const QPoint& position);

    void reset();

    void move();
    void grow();
    void setDirection(Direction direction);
    bool checkCollision();

    void draw(QPainter& painter);

    const QList<QPoint>& getBody() const;
    const QPoint& getHead() const;
    Direction getDirection() const;

    int getScore() const;
    void setScore(int value);

    void reset(const QPoint &position);
protected:
    QList<QPoint> body;
    Direction direction;
    bool growing;
    int score;

    // 根据当前移动方向更新下一个位置
    QPoint getNextPosition() const;

    // 检查贪吃蛇头部是否与边界或自身碰撞
    bool checkBoundaryCollision() const;
    bool checkSelfCollision() const;
};

#endif // SNAKE_H
