#include "snake.h"

Snake::Snake()
{
    reset();
}
Snake::Snake(const QPoint& position)
{
    reset(position);
}

void Snake::reset() {
    // 初始化贪吃蛇的初始状态
    body.clear();
    body << QPoint(5, 5) << QPoint(5, 6) << QPoint(5, 7);
    direction = Direction::Right;
    growing = false;
}
void Snake::reset(const QPoint& position)
{
    body.clear();
    body << position << QPoint(position.x(), 6) << QPoint(position.x(), 7);
    direction = Direction::Right;
    growing = false;
}

void Snake::move()
{
    if (growing) {
        // 如果蛇正在增长，只移动身体，不改变头部位置
        body.prepend(getNextPosition());
        growing = false;
    } else {
        // 如果蛇不在增长，则移动头部和身体
        body.removeLast();
        body.prepend(getNextPosition());
    }
}


void Snake::grow() {
    growing = true;
}

void Snake::setDirection(Direction direction) {
    // 禁止贪吃蛇掉头移动
    if ((direction == Direction::Up && this->direction != Direction::Down) ||
        (direction == Direction::Down && this->direction != Direction::Up) ||
        (direction == Direction::Left && this->direction != Direction::Right) ||
        (direction == Direction::Right && this->direction != Direction::Left)) {
        this->direction = direction;
    }
}

bool Snake::checkCollision() {
    return checkBoundaryCollision() || checkSelfCollision();
}

void Snake::draw(QPainter& painter)
{
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制Snake的身体
    for (const QPoint& point : body) {
        painter.fillRect(point.x() * 10, point.y() * 10, 10, 10, QColor("#006400")); // 使用深绿色
    }

    // 绘制Snake的头部
    const QPoint& head = body.first();
    painter.fillRect(head.x() * 10, head.y() * 10, 10, 10, QColor("#00FF00")); // 使用浅绿色
}

const QList<QPoint>& Snake::getBody() const {
    return body;
}

const QPoint& Snake::getHead() const {
    return body.first();
}

Direction Snake::getDirection() const {
    return direction;
}

int Snake::getScore() const
{
    return score;
}

void Snake::setScore(int value)
{
    score = value;
}

QPoint Snake::getNextPosition() const {
    QPoint nextPosition = getHead();
    
    // 根据移动方向更新下一个位置
    switch (direction) {
        case Direction::Up:
            nextPosition += QPoint(0, -1);
            break;
        case Direction::Down:
            nextPosition += QPoint(0, 1);
            break;
        case Direction::Left:
            nextPosition += QPoint(-1, 0);
            break;
        case Direction::Right:
            nextPosition += QPoint(1, 0);
            break;
    }

    return nextPosition;
}

bool Snake::checkBoundaryCollision() const {
    QPoint head = getHead();

    // 检查头部是否超出边界
    if (head.x() < 0 || head.x() >= 10 || head.y() < 0 || head.y() >= 10) {
        return true;
    }

    return false;
}

bool Snake::checkSelfCollision() const {
    QPoint head = getHead();

    // 检查头部是否与身体的其他坐标重叠
    for (int i = 1; i < body.size(); i++) {
        if (head == body.at(i)) {
            return true;
        }
    }

    return false;
}
