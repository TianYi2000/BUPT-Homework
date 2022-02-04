#ifndef UDPCONNECT_H
#define UDPCONNECT_H

#include <QWidget>
#include <QLayout>
#include <QVariant>
#include <QUdpSocket>
#include <QPushButton>
#include <QJsonObject>
#include <QButtonGroup>

namespace Ui {
class Udpconnect;
}

class Udpconnect : public QWidget
{
    Q_OBJECT

public:
    explicit Udpconnect(QWidget *parent = nullptr);
    ~Udpconnect();

    void sendRequest();

private:
    Ui::Udpconnect *ui;

    QUdpSocket *clientudp;
    int port;
};

#endif // UDPCONNECT_H
