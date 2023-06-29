#ifndef MULTISNAKE_H
#define MULTISNAKE_H

#include "packdef.h"
#include <QColor>
#include <QPainter>

class MultiSnake
{
public:
    MultiSnake(const SnakeInfo& snakeInfo, const QColor& color);
    void updateSnake(const SnakeInfo &snakeInfo);

    void draw(QPainter& painter) const;

    int score() const;
    void setScore(int score);

    QVector<QPoint> positions() const;
    void setPositions(const QVector<QPoint> &positions);

    int playerId() const;
    void setPlayerId(int playerId);

    QColor color() const;
    void setColor(const QColor &color);

private:
    int m_playerId;
    int m_length;
    QVector<QPoint> m_positions;
    int m_score;
    QColor m_color;
};
#endif // MULTISNAKE_H
