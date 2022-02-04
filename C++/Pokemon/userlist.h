#ifndef USERLIST_H
#define USERLIST_H

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
class UserList;
}

class UserList : public QWidget
{
    Q_OBJECT

public:
    explicit UserList(QWidget *parent = nullptr);
    ~UserList();

private slots:
    void receiveData();
    void showPokemons();
    void showDetails();

    void on_pushButton_clicked();

private:
    Ui::UserList *ui;
    QUdpSocket *clientudp;
    QJsonObject userInfo, pokemonInfo;
    QGridLayout *glayout;
    QButtonGroup *userGroup, *pokemonGroup;
    int port;

    void sendRequest(QString OPTYPE, QString userName = "");
    void analysisUserData();
    void analysisPokemonData();
    void setButtonBackImage(QPushButton *button,QString image,int sizeW, int sizeH);
    void deleteLayout(QLayout*);
};

#endif // USERLIST_H
