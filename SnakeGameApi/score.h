#ifndef SCORE_H
#define SCORE_H

#include <QPainter>

class Score {
public:
    Score();

    int getValue() const;
    void increase(int amount = 1);
    void reset();

    void draw(QPainter& painter) const;

private:
    int m_value;
};

#endif // SCORE_H
