#include "snakewidget.h"

#include "ui_snakewidget.h"
SnakeWidget::SnakeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SnakeWidget), m_foodCount(0)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: #333333"); // 使用深灰色作为背景色
    setSnake();
    setFood();
}

SnakeWidget::~SnakeWidget()
{
    delete ui;
}

void SnakeWidget::setSnake()
{
    m_snake = new Snake;
    update();
    // 创建定时器，设置Snake的移动速度
    m_timer = new QTimer;
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_moveSnake()));
    m_timer->start(200); // 每200毫秒移动一次Snake
}

void SnakeWidget::setFood()
{
    m_food = new Food;
    generateFoodPosition();
}

void SnakeWidget::generateFoodPosition()
{
    m_food->generate(m_snake->getBody());
}

void SnakeWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    m_snake->draw(painter);
    m_food->draw(painter);
    m_score.draw(painter);
}

void SnakeWidget::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Up:
            m_snake->setDirection(Direction::Up);
            break;
        case Qt::Key_Down:
            m_snake->setDirection(Direction::Down);
            break;
        case Qt::Key_Left:
            m_snake->setDirection(Direction::Left);
            break;
        case Qt::Key_Right:
            m_snake->setDirection(Direction::Right);
            break;
        default:
            QWidget::keyPressEvent(event);
            return;
    }
}


void SnakeWidget::slot_moveSnake()
{
    m_snake->move();
    if (m_snake->checkCollision()) {
        // 处理贪吃蛇碰撞的逻辑
        // ...
    }
    // 检查是否吃到食物
    if (m_snake->getHead() == m_food->getPosition()) {
        m_snake->grow();
        generateFoodPosition();
        //特殊食物吃下
        if(dynamic_cast<SpecialFood*>(m_food))
        {
            m_foodCount = 0;
            m_score.increase(5);
            setFood();
        }

        // 增加普通食物计数
        if (!dynamic_cast<SpecialFood*>(m_food)) {
            m_foodCount++;
            m_score.increase(1);
        }
        //达到一定数量，生成特殊食物， 重置计数
        if(m_foodCount % 5 == 0)
        {
            delete m_food;
            m_food = new SpecialFood;
            generateFoodPosition();
            m_foodCount = 0;
        }

    }

    update();
}
