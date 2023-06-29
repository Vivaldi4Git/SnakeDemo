#include "multiplayersnakewidget.h"

MultiPlayerSnakeWidget::MultiPlayerSnakeWidget()
{

}


MultiPlayerSnakeWidget::MultiPlayerSnakeWidget(const GameRoomInfo &gameInfo)
{
    gameIsOver = false;
    QVector<SnakeInfo> snakesInfo = gameInfo.snakes;
    for(auto snakeInfo : snakesInfo)
    {
        MultiSnake* snake = new MultiSnake(snakeInfo, Qt::red);
        if(m_snakes.size() == 1)
        {
            snake->setColor(Qt::green);
        }
        else
        {
            snake->setColor(Qt::blue);
        }
        m_snakes[snakeInfo.playerId] = snake;
    }
    m_notificationWidget = new NotificationWidget;
}

void MultiPlayerSnakeWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);


    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if(gameIsOver)
    {
        // 绘制"Game Over"文本
        painter.setFont(QFont("Arial", 24, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, "Game Over");
    }
    // 绘制所有蛇的身体和头部
    for (MultiSnake* snake : m_snakes) {
        snake->draw(painter);
    }

    // 绘制食物
    m_food.draw(painter);

    // 绘制当前最高分数
    drawTopScore(painter);

}
#include <QDebug>
void MultiPlayerSnakeWidget::keyPressEvent(QKeyEvent* event)
{
    switch (event->key()) {
        case Qt::Key_W:
            Q_EMIT SIG_directionChanged(Direction::Up);
            break;
        case Qt::Key_S:
            Q_EMIT SIG_directionChanged(Direction::Down);
            break;
        case Qt::Key_A:
            Q_EMIT SIG_directionChanged(Direction::Left);
            break;
        case Qt::Key_D:
            Q_EMIT SIG_directionChanged(Direction::Right);
            break;
        default:
            QWidget::keyPressEvent(event);
            return;
    }

}


void MultiPlayerSnakeWidget::updateTopScore(int newScore) {
    if (newScore > topScore) {
        topScore = newScore;
        // 在这里执行更新 topScore 的相关操作，如更新界面显示等
    }
}
void MultiPlayerSnakeWidget::updateGame(const GameRoomInfo &gameInfo)
{
    // 更新蛇的信息
    QVector<SnakeInfo> snakesInfo = gameInfo.snakes;
    for (auto snakeInfo : snakesInfo)
    {
        MultiSnake* snake = m_snakes[snakeInfo.playerId];
        snake->updateSnake(snakeInfo);
    }
    // 更新食物
    m_food.setPosition(gameInfo.foodPosition);
    qDebug() << m_food.getPosition() << endl;

    // 找到最高分数
    int maxScore = 0;
     // 保存最高分的玩家id字符串
    for (const SnakeInfo& snakeInfo : snakesInfo) {
       qDebug() << "snakeInfo.playerId: " << snakeInfo.playerId << "snakeInfo.score: "<< snakeInfo.score;
       if (snakeInfo.score > maxScore) {
           qDebug() << "current maxScore: " << maxScore;
           maxScore = snakeInfo.score;
           topScorer = "Player " + QString::number(snakeInfo.playerId);
       }
    }

    // 更新 topScore
    updateTopScore(maxScore);

//    // 创建通知消息
//    QString notificationMessage = "最高分：" + QString::number(maxScore) + "，当前领先者：" + topScorer;

//    // 显示通知
//    m_notificationWidget->showNotification(notificationMessage);

    update();
}

void MultiPlayerSnakeWidget::drawTopScore(QPainter& painter)
{
    QString scoreText = "Top Score: " + QString::number(topScore) + " Playerid: " + topScorer;
    QFont font("Arial", 14, QFont::Bold);
    painter.setFont(font);
    QRect rect = QRect(0, 0, width(), 30);
    painter.drawText(rect, Qt::AlignCenter && Qt::AlignRight, scoreText);
}

void MultiPlayerSnakeWidget::setGameOver()
{

    // 执行清屏操作
    update();
    gameIsOver = true;
}

int MultiPlayerSnakeWidget::currentPlayerId() const
{
    return m_currentPlayerId;
}

void MultiPlayerSnakeWidget::setCurrentPlayerId(int currentPlayerId)
{
    m_currentPlayerId = currentPlayerId;
}
