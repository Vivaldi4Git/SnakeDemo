#-------------------------------------------------
#
# Project created by QtCreator 2023-06-17T13:33:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MultiSnake
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mediator/inetmediator.cpp \
    mediator/TcpClientMediator.cpp \
    net/TcpClientNet.cpp \
    ckernel.cpp \
    logindialog.cpp \
    roomlistwidget.cpp \
    roomitem.cpp \
    roomwidget.cpp

HEADERS  += mainwindow.h \
    mediator/INetMediator.h \
    mediator/TcpClientMediator.h \
    net/INet.h \
    net/TcpClientNet.h \
    shared/packdef.h \
    ckernel.h \
    logindialog.h \
    roomlistwidget.h \
    roomitem.h \
    roomwidget.h

FORMS    += mainwindow.ui \
    logindialog.ui \
    roomlistwidget.ui \
    roomitem.ui \
    roomwidget.ui
INCLUDEPATH += ./net
INCLUDEPATH += ./mediator
INCLUDEPATH += ./shared
LIBS += -lws2_32

include(./SnakeGameApi/SnakeGameApi.pri)
INCLUDEPATH += ./SnakeGameApi

