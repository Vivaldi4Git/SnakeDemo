#include "food.h"
#include <random>

Food::Food()
{
}

void Food::generate(const QList<QPoint>& snakeBody)
{
//    // 生成食物的位置，确保不与贪吃蛇的身体重叠
//    std::random_device rd;
//    std::mt19937 gen(rd());
//    std::uniform_int_distribution<int> dis(0, 9);

//    do {
//        position.setX(dis(gen));
//        position.setY(dis(gen));
//    } while (snakeBody.contains(position));
}

void Food::draw(QPainter& painter)
{
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(position.x() * 10, position.y() * 10, 10, 10, QColor("#FFA500")); // 使用橙色
}

const QPoint& Food::getPosition() const
{
    return position;
}

void Food::setPosition(const QPoint &value)
{
    position = value;
}
