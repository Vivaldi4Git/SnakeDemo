#include "specialfood.h"

SpecialFood::SpecialFood()
{

}

void SpecialFood::draw(QPainter &painter)
{
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(position.x() * 10, position.y() * 10, 10, 10, QColor("#ec4141")); // 使用红色
}
