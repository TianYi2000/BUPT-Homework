#include "pokemonbag.h"
#include "ui_pokemonbag.h"
#include "pokemon.h"

#include <QIcon>
#include <QDebug>
#include <QLabel>
#include <QPixmap>
#include <QString>
#include <QByteArray>
#include <QMessageBox>

#include <QJsonDocument>
#include <QNetworkDatagram>
#include <QCryptographicHash>

const int CLIENT_PORT = 11111;
const int SERVER_PORT = 11110;

PokemonBag::PokemonBag(QString userName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PokemonBag)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setAutoFillBackground(true);
    QPalette pal = this->palette();
    pal.setBrush(backgroundRole(), QPixmap(":/back/background/back.png"));
    setPalette(pal);
    this->setFixedSize(1280,960);
    hide();

    pokemonNumber = 0;
    highPokemonNumber = 0;

    clientudp=new QUdpSocket(this);
    port = CLIENT_PORT;
    while (!clientudp->bind(port,QUdpSocket::ShareAddress))
        port ++;
    connect(clientudp,SIGNAL(readyRead()),this,SLOT(receiveData()));
    sendRequest(userName);
}

PokemonBag::~PokemonBag()
{
    delete ui;
}

void PokemonBag::sendRequest(QString userName)
{
    qDebug() << "run sendRequest";
    QJsonObject userInfo;
    userInfo.insert("optype", "GET_USER_POKEMON");
    userInfo.insert("username", userName);

    QJsonDocument userDoc;
    userDoc.setObject(userInfo);
    QByteArray userData = userDoc.toJson();

    clientudp->writeDatagram(userData, QHostAddress::LocalHost, SERVER_PORT);
}

void PokemonBag::receiveData()
{
    while(clientudp->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = clientudp->receiveDatagram();
        analysisByteArray(datagram.data());
    }
}

void PokemonBag::setButtonBackImage(QPushButton *button,QString image,int sizeW, int sizeH)
{
    QPixmap pixmap(image);
    QPixmap fitpixmap=pixmap.scaled(200,200).scaled(sizeW, sizeH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    button->setIcon(QIcon(fitpixmap));
    button->setIconSize(QSize(sizeW,sizeH));
    button->setFlat(true);
    button->setStyleSheet("border: 0px");
}
/*
 * 布局已经大体完成
 * 但是可能要从每行4个变为每行3个（调整布局）
 */

void PokemonBag::analysisByteArray(QByteArray recData)
{
    QJsonParseError json_error;
    QJsonDocument recDoc(QJsonDocument::fromJson(recData, &json_error));
    if(json_error.error != QJsonParseError::NoError){
        qDebug() << "json error!";
    }
    recInfo = recDoc.object();

    glayout = new QGridLayout();

    int totNum = recInfo["number"].toInt();
    qDebug() << totNum;
    btnGroup = new QButtonGroup(this);
    for (int i = 0; i < totNum; ++i)
    {
        QJsonObject nowPokemon = recInfo["pokemon" + QString::number(i)].toObject();

        if (nowPokemon["level"].toInt() == 15) highPokemonNumber++;
        pokemonNumber ++;

        QPushButton* pokemonBtn = new QPushButton(this);
        pokemonBtn->setCheckable(true);
        btnGroup->addButton(pokemonBtn, i);
        setButtonBackImage(pokemonBtn, ":/icon/pokemons/" + nowPokemon["engname"].toString() + ".png", 105, 105);

        QVBoxLayout *vlayout = new QVBoxLayout();

        QLabel *nameLabel = new QLabel();
        nameLabel->setText(nowPokemon["name"].toString() + " lv:" + QString::number(nowPokemon["level"].toInt()));
        nameLabel->setAlignment(Qt::AlignHCenter);

        vlayout->addWidget(pokemonBtn);
        vlayout->addWidget(nameLabel);

        glayout->addLayout(vlayout, i / 4, i % 4);
        QObject::connect(pokemonBtn, SIGNAL(clicked()), this, SLOT(showDetails()));
    }

    if (totNum % 4 != 0)
    {
        for (int j = totNum % 4 + 1; j <= 4; ++j)
        {

            QPushButton* pokemonBtn = new QPushButton(this);
            setButtonBackImage(pokemonBtn, "", 105, 105);

            QVBoxLayout *vlayout = new QVBoxLayout();

            QLabel *nameLabel = new QLabel();
            nameLabel->setText("");
            nameLabel->setAlignment(Qt::AlignHCenter);

            vlayout->addWidget(pokemonBtn);
            vlayout->addWidget(nameLabel);

            glayout->addLayout(vlayout, totNum / 4, j);
        }
    }

    glayout->setAlignment(Qt::AlignTop);
    ui->scrollArea->widget()->setLayout(glayout);


    if (pokemonNumber >= 5 && pokemonNumber < 10)
    {
        QPixmap img(":/back/background/Bronze.png");
        img.scaled(ui->badgeNumber->size(), Qt::KeepAspectRatio);
        ui->badgeNumber->setScaledContents(true);
        ui->badgeNumber->setPixmap(img);

        ui->badgeNumberInfo->setText("(用户拥有" + QString::number(pokemonNumber) + "个精灵)");
    }

    if (pokemonNumber >= 10 && pokemonNumber < 15)
    {
        QPixmap img(":/back/background/Silver.png");
        img.scaled(ui->badgeNumber->size(), Qt::KeepAspectRatio);
        ui->badgeNumber->setScaledContents(true);
        ui->badgeNumber->setPixmap(img);

        ui->badgeNumberInfo->setText("(用户拥有" + QString::number(pokemonNumber) + "个精灵)");
    }

    if (pokemonNumber >= 15)
    {
        QPixmap img(":/back/background/Gold.png");
        img.scaled(ui->badgeNumber->size(), Qt::KeepAspectRatio);
        ui->badgeNumber->setScaledContents(true);
        ui->badgeNumber->setPixmap(img);

        ui->badgeNumberInfo->setText("(用户拥有" + QString::number(pokemonNumber) + "个精灵)");
    }

    if (highPokemonNumber >= 5 && highPokemonNumber < 10)
    {
        QPixmap img(":/back/background/Bronze.png");
        img.scaled(ui->badgeNumber->size(), Qt::KeepAspectRatio);
        ui->badgeHigh->setScaledContents(true);
        ui->badgeHigh->setPixmap(img);

        ui->badgeHighInfo->setText("(用户拥有" + QString::number(highPokemonNumber) + "个高级精灵)");
    }

    if (highPokemonNumber >= 10 && highPokemonNumber < 15)
    {
        QPixmap img(":/back/background/Silver.png");
        img.scaled(ui->badgeHigh->size(), Qt::KeepAspectRatio);
        ui->badgeHigh->setScaledContents(true);
        ui->badgeHigh->setPixmap(img);

        ui->badgeHighInfo->setText("(用户拥有" + QString::number(highPokemonNumber) + "个高级精灵)");
    }

    if (highPokemonNumber >= 15)
    {
        QPixmap img(":/back/background/Gold.png");
        img.scaled(ui->badgeHigh->size(), Qt::KeepAspectRatio);
        ui->badgeHigh->setScaledContents(true);
        ui->badgeHigh->setPixmap(img);

        ui->badgeHighInfo->setText("(用户拥有" + QString::number(highPokemonNumber) + "个高级精灵)");
    }

    int battleNumber = recInfo["battle"].toInt();
    int winNumber = recInfo["win"].toInt();
    ui->battleInfo->setText("共战斗" + QString::number(battleNumber) + "场,胜利"
            + QString::number(winNumber) + "场");
    if (winNumber == 0)
    {
        ui->winInfo->setText("胜率为0%");
    }
    else
    {
        ui->winInfo->setText("胜率为" + QString::number(100.0 * winNumber / battleNumber, 'f', 2) + "%");
    }
}

void PokemonBag::showDetails()
{
    int id = btnGroup->checkedId();
    qDebug() << "id=" << id;
    QJsonObject nowPokemon = recInfo["pokemon" + QString::number(id)].toObject();
    qDebug() << nowPokemon["name"].toString();

    ui->name->setText(nowPokemon["name"].toString());
    ui->type->setText(Pokemon::showAttribute( nowPokemon["type"].toInt() ));
    ui->kind->setText(Pokemon::showCategory( nowPokemon["kind"].toInt() ));
    ui->level->setText(QString::number( nowPokemon["level"].toInt() ));
    ui->exp->setText(QString::number( nowPokemon["exp"].toInt() ));
    ui->atk->setText(QString::number( nowPokemon["atk"].toInt() ));
    ui->def->setText(QString::number( nowPokemon["def"].toInt() ));
    ui->hp->setText(QString::number( nowPokemon["hp"].toInt() ));
    ui->agl->setText(QString::number( nowPokemon["agl"].toInt() ));
    ui->delay->setText(QString::number( 300 - nowPokemon["agl"].toInt() ));
    ui->skill->setText(nowPokemon["skill"].toString());
    return ;
}




void PokemonBag::on_pushButton_clicked()
{
    delete this;
}
