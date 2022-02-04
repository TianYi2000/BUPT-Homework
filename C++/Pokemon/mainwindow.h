#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "menu.h"
#include "login.h"
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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void LoginSuccess();                //在登录窗口loginWindow中登录成功时返回

private slots:

    void receiveUserName(QString);
    void receiveLogOut();

private:
    Ui::MainWindow *ui;
    Login* loginWindow;
    Menu* menuWindow;
    QString username;

    void enterLogin();
    void enterMenu();
};
#endif // MAINWINDOW_H
