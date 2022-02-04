#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QMessageBox>
#include <QJsonDocument>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setAutoFillBackground(true);
    QPalette pal = this->palette();
    pal.setBrush(backgroundRole(), QPixmap(":/back/background/back.png"));
    setPalette(pal);
    this->setFixedSize(1280,960);

    username = "";
    enterLogin();

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::enterMenu()
{
    menuWindow = new Menu (username, this);
    menuWindow->show();
    connect(menuWindow, SIGNAL(logOutSignal()), this, SLOT(receiveLogOut()));
}

void MainWindow::receiveLogOut()
{
    username = "";
    enterLogin();
}

void MainWindow::enterLogin()
{
    loginWindow = new Login(this);
    loginWindow->show();
    connect(loginWindow, SIGNAL(sendUserName(QString)), this, SLOT(receiveUserName(QString)));

}

void MainWindow::receiveUserName(QString userName)
{
    qDebug() << "run receiveUserName";
    qDebug() << username;
    username = userName;

    enterMenu();
}
