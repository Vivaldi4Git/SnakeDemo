
QT += widgets

#SOURCES += \
#    snakewidget.cpp \
#    snake.cpp \
#    food.cpp \
#    specialfood.cpp \
#    score.cpp \
#    multiplayersnakewidget.cpp

#HEADERS += \
#    snakewidget.h \
#    snake.h \
#    food.h \
#    specialfood.h \
#    score.h \
#    multiplayersnakewidget.h

#FORMS += \
#    snakewidget.ui

FORMS += \
    $$PWD/snakewidget.ui


HEADERS += \
    $$PWD/food.h \
    $$PWD/game.h \
    $$PWD/multiplayersnakewidget.h \
    $$PWD/score.h \
    $$PWD/snakegame.h \
    $$PWD/specialfood.h \
    $$PWD/multisnake.h \
    $$PWD/notificationwidget.h

SOURCES += \
    $$PWD/food.cpp \
    $$PWD/game.cpp \
    $$PWD/multiplayersnakewidget.cpp \
    $$PWD/score.cpp \
    $$PWD/snakegame.cpp \
    $$PWD/specialfood.cpp \
    $$PWD/multisnake.cpp \
    $$PWD/notificationwidget.cpp
