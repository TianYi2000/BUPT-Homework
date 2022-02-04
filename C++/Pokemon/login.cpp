#include "login.h"
#include "ui_login.h"

#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QJsonDocument>
#include <QNetworkDatagram>
#include <QCryptographicHash>

const int CLIENT_PORT = 11111;
const int SERVER_PORT = 11110;

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setAutoFillBackground(true);
    QPalette pal = this->palette();
    pal.setBrush(backgroundRole(), QPixmap(":/back/background/back.png"));
    setPalette(pal);
    this->setFixedSize(1280,960);
    hide();

    QFont font = this->font();
    font.setFamily("Microsoft YaHei");  //字体名称
    font.setPixelSize(16);  //字体点大小
    font.setPointSize(18);  //字体像素大小
    this->setFont(font);

    clientudp=new QUdpSocket(this);
    port = CLIENT_PORT;
    while (!clientudp->bind(port,QUdpSocket::ShareAddress))
        port ++;
    connect(clientudp,SIGNAL(readyRead()),this,SLOT(receiveData()));
}


Login::~Login()
{
    delete ui;
    clientudp->close();
}

bool Login::getInfo(QJsonObject& userInfo)
{
    if (ui->username->text() == "" || ui->password->text() == "")
    {
        QMessageBox::warning(this, "错误", "账号或密码不能为空");
        return false;
    }
    if (ui->password->text().length() < 6)
    {
        QMessageBox::warning(this, "错误", "密码长度不能小于6位");
        return false;
    }

    userInfo.insert("username", ui->username->text());

    //对密码进行Md5加密后再传输
    QByteArray pwdByte = ui->password->text().toUtf8();
    QString PwdMd5 = QCryptographicHash::hash(pwdByte, QCryptographicHash::Md5).toHex();

    userInfo.insert("password", PwdMd5);
}

void Login::on_signUpBtn_clicked()
{
    QJsonObject userInfo;
    userInfo.insert("optype", "SIGN_UP");
    if(getInfo(userInfo))
    {
        QJsonDocument userDoc;
        userDoc.setObject(userInfo);
        QByteArray userData = userDoc.toJson();

        clientudp->writeDatagram(userData, QHostAddress::LocalHost, SERVER_PORT);
    }
}


void Login::on_logInBtn_clicked()
{
    QJsonObject userInfo;
    userInfo.insert("optype", "LOG_IN");
    if(getInfo(userInfo))
    {
        QJsonDocument userDoc;
        userDoc.setObject(userInfo);
        QByteArray userData = userDoc.toJson();

        clientudp->writeDatagram(userData, QHostAddress::LocalHost, SERVER_PORT);
    }
}

void Login::receiveData()
{
    while(clientudp->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = clientudp->receiveDatagram();
        analysisByteArray(datagram.data());
    }
}

void Login::analysisByteArray(QByteArray logData)
{
    QJsonParseError json_error;
    QJsonDocument logDoc(QJsonDocument::fromJson(logData, &json_error));
    if(json_error.error != QJsonParseError::NoError){
        qDebug() << "json error!";
    }
    QJsonObject logInfo = logDoc.object();


    QString result = logInfo["result"].toString();
    QString info = logInfo["info"].toString();

    if (result == "LOGIN_FAIL")
    {
        if (info == "user not existed")
        {
            QMessageBox::warning(this, "错误", "用户不存在");
            ui->username->clear();
            ui->password->clear();
        }

        if (info == "wrong password")
        {
            QMessageBox::warning(this, "错误", "密码错误");
            ui->password->clear();
        }
    }

    if (result == "LOGIN_SUCCESS")
    {
        QMessageBox::information(this, "成功", "登录成功");
        emit sendUserName(ui->username->text());
        qDebug() << ui->username->text();
        this->hide();
    }

    if (result == "SIGN_UP_FAIL")
    {
        if (info == "user already existed")
        {
            QMessageBox::warning(this, "错误", "用户已存在");
            ui->username->clear();
            ui->password->clear();
        }
    }

    if (result == "SIGN_UP_SUCCESS")
    {
        QMessageBox::warning(this, "成功", "注册成功");
        emit sendUserName(ui->username->text());
        this->hide();
    }

}
