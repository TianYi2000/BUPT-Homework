#ifndef SELECTPOKEMON_H
#define SELECTPOKEMON_H

#include "pokemon.h"

#include <QWidget>
#include <QLayout>
#include <QVariant>
#include <QUdpSocket>
#include <QPushButton>
#include <QJsonObject>
#include <QScrollArea>
#include <QButtonGroup>

namespace Ui {
class SelectPokemon;
}

class SelectPokemon : public QWidget
{
    Q_OBJECT

public:
    explicit SelectPokemon(QString, QString, QWidget *parent = nullptr);
    ~SelectPokemon();

    void UdpInit();
    void UIInit();

private slots:
    void receiveData();
    void showDetails1();
    void showDetails2();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

signals:
    void selectedPokemons(Pokemon*, Pokemon*);

private:
    Ui::SelectPokemon *ui;
    QUdpSocket *clientudp;
    QJsonObject recInfo[3];
    QGridLayout *glayout;
    QButtonGroup *btnGroup1, *btnGroup2;
    int port, nowrole;
    QString username, enemyname;
    bool sendOver;
    Pokemon *minePokemon, *enemyPokemon;

    void sendRequest(QString);
    bool getInfo(QJsonObject&);
    void analysisByteArray(QByteArray);
    void setButtonBackImage(QPushButton *button,QString image,int sizeW, int sizeH);
    void setInscrollArea(QScrollArea*);
    Pokemon* createPokemon(QJsonObject);
//    void setRestraint();
};

#endif // SELECTPOKEMON_H
