#include "roomwidget.h"
#include "ui_roomwidget.h"
#include "QDebug"
RoomWidget::RoomWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoomWidget),m_snakeGameWnd(nullptr)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::ClickFocus);

}

RoomWidget::~RoomWidget()
{
    delete ui;
}

void RoomWidget::addPlayer(int playerid, QString &playerName)
{
    QListWidgetItem* item = new QListWidgetItem();
    item->setText(playerName);
    item->setData(Qt::UserRole, playerid);  // 将playerid绑定到QListWidgetItem的UserRole属性中
    ui->wdg_list_player->addItem(item);
}

void RoomWidget::on_pb_leaving_clicked()
{
    Q_EMIT SIG_playerLeave();
}

void RoomWidget::on_pb_startGame_clicked()
{
    QVector<int> playerVector;
    QList<QListWidgetItem*> items = ui->wdg_list_player->findItems(QString("*"), Qt::MatchWildcard);
    foreach (QListWidgetItem* item, items) {
        int playerid = item->data(Qt::UserRole).toInt();
        playerVector.append(playerid);
    }
    for (int i = 0; i < playerVector.size(); i++) {
        int playerid = playerVector[i];
        qDebug() << "Player ID: " << playerid;
    }

    Q_EMIT SIG_gameStart(playerVector);

}

void RoomWidget::slot_directionChanged(Direction d)
{
    Q_EMIT SIG_directionChanged(d);
}
void RoomWidget::keyPressEvent(QKeyEvent *event)
{
//    // 将键盘事件转发给 MultiPlayerSnakeWidget
//    m_snakeGameWnd->keyPressEvent(event);
    switch (event->key()) {
        case Qt::Key_W:
            Q_EMIT SIG_directionChanged(Direction::Up);
            break;
        case Qt::Key_S:
            Q_EMIT SIG_directionChanged(Direction::Down);
            break;
        case Qt::Key_A:
            Q_EMIT SIG_directionChanged(Direction::Left);
            break;
        case Qt::Key_D:
            Q_EMIT SIG_directionChanged(Direction::Right);
            break;
        default:
            QWidget::keyPressEvent(event);
            return;
    }
}

void RoomWidget::startGame(const GameRoomInfo &gameInfo, int playerid)
{
    // 删除原来的 MultiPlayerSnakeWidget 实例
    if (m_snakeGameWnd) {
        m_snakeGameWnd->deleteLater();
        m_snakeGameWnd = nullptr;
    }
    // 根据GameInfo创建 MultiPlayerSnakeWidget 实例
    m_snakeGameWnd = new MultiPlayerSnakeWidget(gameInfo);
    connect(m_snakeGameWnd, SIGNAL(SIG_directionChanged(Direction)), this, SLOT(slot_directionChanged(Direction)));
    m_snakeGameWnd->setCurrentPlayerId(playerid);

    // 设置 MultiPlayerSnakeWidget 的大小和位置
    m_snakeGameWnd->setGeometry(ui->wdg_game->geometry());

    // 将 MultiPlayerSnakeWidget 添加到 wdg_game 中
    m_snakeGameWnd->setParent(ui->wdg_game);

    m_snakeGameWnd->show();
}

void RoomWidget::updateGame(const GameRoomInfo &gameInfo)
{
    m_snakeGameWnd->updateGame(gameInfo);
}

void RoomWidget::setGameOver()
{
    m_snakeGameWnd->setGameOver();
}
