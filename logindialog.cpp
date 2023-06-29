#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    // 切换到登录页面
    ui->stackedWidget->setCurrentIndex(0);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::closeEvent(QCloseEvent *event)
{
    event->accept(); //对窗口进行关闭
    Q_EMIT SIG_close();
}

void LoginDialog::on_pb_toRegister_clicked()
{
    // 切换到注册页面
    ui->stackedWidget->setCurrentIndex(1);
}

void LoginDialog::on_pb_toLogin_clicked()
{
    // 切换到登录页面
    ui->stackedWidget->setCurrentIndex(0);
}

void LoginDialog::on_pb_login_clicked()
{
    //1. 获取控件中的数据
    QString tel = ui->le_tel_login->text();
    QString password = ui->le_passwd_login->text();

    //2. 判断是否合法
//    tel.isEmpty() || password.isEmpty() ||
    QString tempTel = tel;
    QString tempPassword = password;
    if(tempTel.remove(" ").isEmpty() || tempPassword.remove(" ").isEmpty())  //如果去掉空格后，为空
    {
           QMessageBox::about(this, "warning", "手机号或密码不能为空");
           return;
    }
    if(tel.length() > 11 || password.length() > 10)  //手机号不能超过15位， 密码不超过10位
    {
        QMessageBox::about(this, "warning", "手机号不能超过11位， 密码不超过10位");
        return;
    }

    //3. 将数据以信号形式发给kernel
    Q_EMIT SIG_loginCommit(tel, password);
}

void LoginDialog::on_pb_register_clicked()
{
    //1. 获取控件中的数据
    QString tel = ui->le_tel_register->text();
    QString password = ui->le_passwd_register->text();
    QString nickName = ui->lb_nickname_register->text();

    //2. 判断是否合法
    QString tempTel = tel;
    QString tempPassword = password;
    QString tempnickName = nickName;
    if(tempTel.remove(" ").isEmpty() || tempPassword.remove(" ").isEmpty() || tempnickName.remove(" ").isEmpty())  //如果去掉空格后，为空
    {
           QMessageBox::about(this, "warning", "手机号或密码不能为空");
           return;
    }
    if(tel.length() > 11 || password.length() > 10 || nickName.length() > 20)  //手机号不能超过11位， 密码不超过10位, 昵称不超过20位
    {
        QMessageBox::about(this, "warning", "手机号不能超过11位， 密码不超过10位, 昵称不超过20位");
        return;
    }

    //3. 将数据以信号形式发给kernel
    Q_EMIT SIG_registerCommit(tel, password, nickName);
}
