#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pokemon.h"

#include <QList>
#include <QMainWindow>
#include <QUdpSocket>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void analysisByteArray(QByteArray, QHostAddress, int); //分析客户端发送的信息
    Pokemon* CreatePokemon(QSqlDatabase&);                              //创建一个初始精灵
    QList<QJsonObject> GetUserPokemon(QString, QSqlDatabase&);             //获得一个用户的所有精灵
    QJsonObject userPokemontoJson(QString, int, QSqlDatabase&);            //将一个用户的所有精灵转换成一个json对象

private slots:
    void receiveData();

private:
    Ui::MainWindow *ui;
    QUdpSocket *serverudp;
//    QSqlDatabase db;
    static int userNum;
    void udpInit();
    void udpSend(QJsonObject, QHostAddress, int);
    void dataBaseInit();
    void dataBaseInsert(QJsonObject, QSqlDatabase&);


};
#endif // MAINWINDOW_H
