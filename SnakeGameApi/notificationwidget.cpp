#include "notificationwidget.h"
#include <QPainter>

NotificationWidget::NotificationWidget(QWidget *parent)
    : QWidget(parent)
{
    // 设置窗口属性
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);

    // 创建定时器，用于自动关闭通知
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &NotificationWidget::close);
}

void NotificationWidget::showNotification(const QString &message, int duration)
{
    m_notificationMessage = message;
    adjustSize();

    // 计算通知栏的位置
    QPoint pos = parentWidget()->mapToGlobal(parentWidget()->rect().topRight());
    pos.setX(pos.x() - width());
    setGeometry(pos.x(), pos.y(), width(), height());

    // 显示通知栏并启动定时器
    show();
    m_timer->start(duration);
}

void NotificationWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制通知栏内容
    drawNotification(painter);
}

void NotificationWidget::drawNotification(QPainter &painter)
{
    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 5, 5);

    // 绘制文本标签
    painter.setPen(Qt::black);
    painter.drawText(rect(), Qt::AlignCenter, m_notificationMessage);
}
