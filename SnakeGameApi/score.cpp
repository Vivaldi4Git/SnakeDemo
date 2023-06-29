#include "score.h"

Score::Score() : m_value(0) {}

int Score::getValue() const {
    return m_value;
}

void Score::increase(int amount) {
    m_value += amount;
}

void Score::reset() {
    m_value = 0;
}

void Score::draw(QPainter& painter) const {
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 12));
    painter.drawText(10, 20, QString("Score: %1").arg(m_value));
}
