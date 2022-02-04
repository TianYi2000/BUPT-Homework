#include "udpconnect.h"
#include "ui_udpconnect.h"

#include <QString>
#include <QByteArray>
#include <QJsonDocument>
#include <QNetworkDatagram>

const int CLIENT_PORT = 11111;
const int SERVER_PORT = 11110;


Udpconnect::Udpconnect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Udpconnect)
{
    ui->setupUi(this);

    clientudp=new QUdpSocket();
    port = CLIENT_PORT;
    while (!clientudp->bind(port,QUdpSocket::ShareAddress))
        port ++;
    QObject::connect(clientudp,SIGNAL(readyRead()),this,SLOT(receiveData()));
}

Udpconnect::~Udpconnect()
{
    delete ui;
}
