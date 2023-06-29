#ifndef SNAKEWIDGET_H
#define SNAKEWIDGET_H

#include <QWidget>
#include <QTimer>
#include "snake.h"
#include "food.h"
#include "specialfood.h"
#include "score.h"
#include "QPainter"
#include <QList>
#include <QPoint>
#include <QKeyEvent>
#include <QDebug>
namespace Ui {
class SnakeWidget;
}

class SnakeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SnakeWidget(QWidget *parent = 0);
    ~SnakeWidget();
    virtual void setSnake();
    void setFood();
    virtual void generateFoodPosition();
protected:
    void paintEvent(QPaintEvent* event) override;
    //键盘事件
    void keyPressEvent(QKeyEvent *event);
protected:
    Ui::SnakeWidget *ui;
    Snake* m_snake;
    Food* m_food;
    QTimer* m_timer;

    int m_foodCount;
    Score m_score; // 记录当前分数
public slots:
    void slot_moveSnake();
};

#endif // SNAKEWIDGET_H
