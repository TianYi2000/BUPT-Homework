#ifndef POKEMONBAG_H
#define POKEMONBAG_H

#include "battle.h"

#include <QWidget>
#include <QLayout>
#include <QVariant>
#include <QUdpSocket>
#include <QPushButton>
#include <QJsonObject>
#include <QButtonGroup>

namespace Ui {
class PokemonBag;
}

class PokemonBag : public QWidget
{
    Q_OBJECT

public:
    explicit PokemonBag(QString, QWidget *parent = nullptr);
    ~PokemonBag();

private slots:
    void receiveData();
    void showDetails();

    void on_pushButton_clicked();

private:
    Ui::PokemonBag *ui;

    QUdpSocket *clientudp;
    QJsonObject recInfo;
    QGridLayout *glayout;
    QButtonGroup *btnGroup;
    int port, highPokemonNumber, pokemonNumber;

    bool getInfo(QJsonObject&);
    void analysisByteArray(QByteArray);
    void sendRequest(QString);
    void setButtonBackImage(QPushButton *button,QString image,int sizeW, int sizeH);
};

#endif // POKEMONBAG_H
