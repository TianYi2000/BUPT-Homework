#ifndef MENU_H
#define MENU_H

#include "model.h"
#include "pokemon.h"
#include "userlist.h"
#include "pokemonbag.h"

#include <QWidget>
#include <QLayout>
#include <QVariant>
#include <QUdpSocket>
#include <QPushButton>
#include <QJsonObject>
#include <QMainWindow>
#include <QJsonObject>


namespace Ui {
class Menu;
}

class Menu : public QWidget
{
    Q_OBJECT

public:
    explicit Menu(QString, QWidget *parent = nullptr);
    ~Menu();
signals:
    void logOutSignal();

private slots:
    void on_openBagBtn_clicked();
    void on_logOutBtn_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

private:
    Ui::Menu *ui;
    PokemonBag* bagWindow;
    Battle* battleWindow;
    Model* modelWindow;
    UserList* userListWindow;
    QString username;
    QUdpSocket *clientudp;
    int port;

    void sendLogOut();

};

#endif // MENU_H
