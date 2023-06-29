#ifndef FOOD_H
#define FOOD_H

#include <QPoint>
#include <QPainter>

class Food {
public:
    Food();

    void generate(const QList<QPoint>& snakeBody);
    virtual void draw(QPainter& painter);

    const QPoint& getPosition() const;

    void setPosition(const QPoint &value);

protected:
    QPoint position;
};

#endif // FOOD_H
