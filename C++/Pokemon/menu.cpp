#include "menu.h"
#include "ui_menu.h"

#include <QMessageBox>
#include <QJsonDocument>

const int CLIENT_PORT = 11111;
const int SERVER_PORT = 11110;

Menu::Menu(QString userName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);

    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setAutoFillBackground(true);
    QPalette pal = this->palette();
    pal.setBrush(backgroundRole(), QPixmap(":/back/background/back.png"));
    setPalette(pal);
    this->setFixedSize(1280,960);
    hide();

    username = userName;
     ui->userName->setText(username);
}

Menu::~Menu()
{
    /*用户关闭窗口时，需要向服务器传达注销讯息*/
    sendLogOut();

    delete ui;
}

void Menu::sendLogOut()
{
    qDebug() << "User Log Out";

    clientudp=new QUdpSocket(this);
    port = CLIENT_PORT;
    while (!clientudp->bind(port,QUdpSocket::ShareAddress))
        port ++;

    QJsonObject userInfo;
    userInfo.insert("optype", "LOG_OUT");
    userInfo.insert("username", username);

    QJsonDocument userDoc;
    userDoc.setObject(userInfo);
    QByteArray userData = userDoc.toJson();

    clientudp->writeDatagram(userData, QHostAddress::LocalHost, SERVER_PORT);

}

void Menu::on_openBagBtn_clicked()
{
    bagWindow = new PokemonBag(username, this);
    bagWindow->show();
}


void Menu::on_logOutBtn_clicked()
{
    sendLogOut();

    QMessageBox::warning(this, "", "注销成功");
    emit(logOutSignal());
    this->hide();
}

void Menu::on_pushButton_2_clicked()
{
    modelWindow = new Model(username, this);
    modelWindow->show();
}

void Menu::on_pushButton_3_clicked()
{
    userListWindow = new UserList(this);
    userListWindow->show();
}
