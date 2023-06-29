#ifndef NOTIFICATIONWIDGET_H
#define NOTIFICATIONWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QPaintEvent>

class NotificationWidget : public QWidget {
    Q_OBJECT

public:
    explicit NotificationWidget(QWidget *parent = nullptr);

    void showNotification(const QString &message, int duration = 3000);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString m_notificationMessage;
    QTimer *m_timer;

    void drawNotification(QPainter &painter);
};

#endif // NOTIFICATIONWIDGET_H
