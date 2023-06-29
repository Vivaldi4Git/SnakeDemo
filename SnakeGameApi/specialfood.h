#ifndef SPECIALFOOD_H
#define SPECIALFOOD_H
#include "food.h"

class SpecialFood : public Food
{
public:
    SpecialFood();
    void draw(QPainter& painter);
};

#endif // SPECIALFOOD_H
