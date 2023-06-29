#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QCloseEvent>
namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
    //重写的关闭窗口事件
    void closeEvent(QCloseEvent* event);
private slots:
    void on_pb_toRegister_clicked();

    void on_pb_toLogin_clicked();

    void on_pb_login_clicked();

    void on_pb_register_clicked();

private:
    Ui::LoginDialog *ui;
signals:
    SIG_loginCommit(QString tel, QString password);
    SIG_registerCommit(QString tel, QString password, QString nickName);
    SIG_close();
};

#endif // LOGINDIALOG_H
