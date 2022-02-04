#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QUdpSocket>
#include <QVariant>


namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void receiveData();             //接收从服务器返回的信息
    void on_signUpBtn_clicked();    //点击注册按钮的槽函数
    void on_logInBtn_clicked();     //点击登录按钮的槽函数

signals:
    void sendUserName(QString);     //登录/注册成功后向主窗口发送用户信息的信号

private:
    Ui::Login *ui;
    QUdpSocket *clientudp;          //udp连接
    int port;                       //udp端口

    bool getInfo(QJsonObject&);     //获取用户输入的账号密码信息
    void analysisByteArray(QByteArray); //分析从服务器返回的信息
};

#endif // LOGIN_H
