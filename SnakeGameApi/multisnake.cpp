#include "multisnake.h"


MultiSnake::MultiSnake(const SnakeInfo& snakeInfo, const QColor& color)
    : m_playerId(snakeInfo.playerId),
      m_length(snakeInfo.length),
      m_positions(snakeInfo.positions),
      m_score(snakeInfo.score),
      m_color(color)
{
}

void MultiSnake::updateSnake(const SnakeInfo& snakeInfo)
{
    m_playerId = snakeInfo.playerId;
    if(snakeInfo.positions.size() == 1) //只有一个，说明只添加头部
    {
        m_positions.prepend(snakeInfo.positions[0]);
        m_positions.removeLast();
    }
    if(snakeInfo.positions.size() == 2) //有两个， 说明要添加头部， 抹除对应的尾部
    {
        m_positions.prepend(snakeInfo.positions[0]);


    }
    m_score = snakeInfo.score;
}

void MultiSnake::draw(QPainter& painter) const
{
    painter.setBrush(m_color);
    for (const QPoint& point : m_positions) {
        painter.fillRect(point.x() * 10, point.y() * 10, 10, 10, m_color);
    }
}

int MultiSnake::score() const
{
    return m_score;
}

void MultiSnake::setScore(int score)
{
    m_score = score;
}

QVector<QPoint> MultiSnake::positions() const
{
    return m_positions;
}

void MultiSnake::setPositions(const QVector<QPoint> &positions)
{
    m_positions = positions;
}

int MultiSnake::playerId() const
{
    return m_playerId;
}

void MultiSnake::setPlayerId(int playerId)
{
    m_playerId = playerId;
}

QColor MultiSnake::color() const
{
    return m_color;
}

void MultiSnake::setColor(const QColor &color)
{
    m_color = color;
}
