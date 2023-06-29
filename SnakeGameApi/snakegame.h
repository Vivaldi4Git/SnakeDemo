#ifndef SNAKEGAME_H
#define SNAKEGAME_H
#include <QMainWindow>
#include <QKeyEvent>
#include <QPainter>
#include <QTimer>

const int WIDTH = 64;  // 游戏区域宽度 1280 / 20 = 64
const int HEIGHT = 42; // 游戏区域高度 840 / 20 = 42
const int CELL_SIZE = 20; // 每个方格的大小

class SnakeGame : public QMainWindow
{
    Q_OBJECT

public:
    SnakeGame(QWidget *parent = nullptr)
        : QMainWindow(parent),
          gameTimer(new QTimer(this)),
          snakeX(0),
          snakeY(0),
          foodX(0),
          foodY(0),
          direction(Right)
    {
        gameTimer->setInterval(200); // 每200毫秒更新一次游戏
        connect(gameTimer, SIGNAL(timeout()), this, SLOT(updateGame()));
        gameTimer->start();
    }

protected:
    void keyPressEvent(QKeyEvent *event) override
    {
        // 处理按键事件
        switch (event->key()) {
        case Qt::Key_Left:
            direction = Left;
            break;
        case Qt::Key_Right:
            direction = Right;
            break;
        case Qt::Key_Up:
            direction = Up;
            break;
        case Qt::Key_Down:
            direction = Down;
            break;
        default:
            break;
        }
    }

    void paintEvent(QPaintEvent *event) override
    {
        QPainter painter(this);

        // 绘制背景
        painter.fillRect(0, 0, WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, Qt::white);

        // 绘制蛇
        painter.fillRect(snakeX * CELL_SIZE, snakeY * CELL_SIZE, CELL_SIZE, CELL_SIZE, Qt::green);

        // 绘制食物
        painter.fillRect(foodX * CELL_SIZE, foodY * CELL_SIZE, CELL_SIZE, CELL_SIZE, Qt::red);
    }

private slots:
    void updateGame()
    {
        // 更新蛇的位置和食物的位置
        switch (direction) {
        case Left:
            snakeX--;
            break;
        case Right:
            snakeX++;
            break;
        case Up:
            snakeY--;
            break;
        case Down:
            snakeY++;
            break;
        }

        // 如果蛇吃到了食物，重新生成食物位置
        if (snakeX == foodX && snakeY == foodY) {
            foodX = qrand() % WIDTH;
            foodY = qrand() % HEIGHT;
        }

        // 重绘窗口
        update();
    }

private:
    enum Direction {
        Left,
        Right,
        Up,
        Down
    };

    QTimer *gameTimer;
    int snakeX;
    int snakeY;
    int foodX;
    int foodY;
    Direction direction;
};

#endif // SNAKEGAME_H
