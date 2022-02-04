#include "userlist.h"
#include "ui_userlist.h"

#include <QWidget>
#include <QLayout>
#include <QVariant>
#include <QUdpSocket>
#include <QPushButton>
#include <QJsonObject>
#include <QScrollArea>
#include <QButtonGroup>
#include <QJsonDocument>
#include <QNetworkDatagram>

const int CLIENT_PORT = 11111;
const int SERVER_PORT = 11110;

UserList::UserList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserList)
{
    ui->setupUi(this);

    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setAutoFillBackground(true);
    QPalette pal = this->palette();
    pal.setBrush(backgroundRole(), QPixmap(":/back/background/back.png"));
    setPalette(pal);
    this->setFixedSize(1280,960);
    hide();

    clientudp=new QUdpSocket(this);
    port = CLIENT_PORT;
    while (!clientudp->bind(port,QUdpSocket::ShareAddress))
        port ++;
    connect(clientudp,SIGNAL(readyRead()),this,SLOT(receiveData()));
    sendRequest("GET_USERLIST");

    glayout = NULL;
}

UserList::~UserList()
{
    delete ui;
}

void UserList::sendRequest(QString OPTYPE, QString userName)
{
    qDebug() << "run sendRequest";
    QJsonObject sendInfo;
    sendInfo.insert("optype", OPTYPE);
    sendInfo.insert("username", userName);

    qDebug() << "发送请求:" << sendInfo;

    QJsonDocument userDoc;
    userDoc.setObject(sendInfo);
    QByteArray userData = userDoc.toJson();
    clientudp->writeDatagram(userData, QHostAddress::LocalHost, SERVER_PORT);
}

void UserList::receiveData()
{
    while(clientudp->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = clientudp->receiveDatagram();

        qDebug() << datagram.data();

        QJsonParseError json_error;
        QJsonDocument recDoc(QJsonDocument::fromJson(datagram.data(), &json_error));
        if(json_error.error != QJsonParseError::NoError){
            qDebug() << "json error!";
        }
        QJsonObject recInfo = recDoc.object();

        if (recInfo["result"].toString() == "userlist")
        {
            qDebug() << "接收用户列表";

            userInfo = recInfo;
            analysisUserData();
        }
        else
        {
            qDebug() << "接收宝可梦列表";

            pokemonInfo = recInfo;
            analysisPokemonData();
        }

    }
}

void UserList::analysisUserData()
{
    int totNum = userInfo["number"].toInt();
    qDebug() << totNum;
    userGroup = new QButtonGroup(this);
    QVBoxLayout *vlayout = new QVBoxLayout();
    for (int i = 0; i < totNum; ++i)
    {
        QJsonObject nowUser = userInfo["user" + QString::number(i)].toObject();

        qDebug() << nowUser;

        QPushButton* userBtn = new QPushButton(this);
        if (nowUser["online"].toInt() == 1)
        {
            userBtn->setText("(在线)" + nowUser["username"].toString());
            userBtn->setStyleSheet("background-color: #3faae8;");
        }
        else
        {
            userBtn->setText("(离线)" + nowUser["username"].toString());
            userBtn->setStyleSheet("background-color: grey;");
        }


        userBtn->setMaximumHeight(80);
        userBtn->setMinimumHeight(80);

//        userBtn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);

        userBtn->setCheckable(true);
        userGroup->addButton(userBtn, i);

        vlayout->addWidget(userBtn);
        QObject::connect(userBtn, SIGNAL(clicked()), this, SLOT(showPokemons()));
    }
    vlayout->addStretch();
    ui->scrollAreaU->widget()->setLayout(vlayout);

}

void UserList::analysisPokemonData()
{

    if (glayout != NULL)
    {
        QList<QPushButton*> btns = ui->scrollAreaWidgetContents_2->findChildren<QPushButton*>();
        QList<QLabel*> labels = ui->scrollAreaWidgetContents_2->findChildren<QLabel*>();
        foreach (QPushButton* btn, btns) {   delete btn;  }
        foreach (QLabel* label, labels) {   delete label;  }
        delete glayout;
    }

    glayout = new QGridLayout();

    int totNum = pokemonInfo["number"].toInt();
    qDebug() << totNum;
    pokemonGroup = new QButtonGroup(this);
    for (int i = 0; i < totNum; ++i)
    {
        QJsonObject nowPokemon = pokemonInfo["pokemon" + QString::number(i)].toObject();

        QPushButton* pokemonBtn = new QPushButton(this);
        pokemonBtn->setCheckable(true);
        pokemonGroup->addButton(pokemonBtn, i);
        setButtonBackImage(pokemonBtn, ":/icon/pokemons/" + nowPokemon["engname"].toString() + ".png", 105, 105);

        QVBoxLayout *vlayout = new QVBoxLayout();

        QLabel *nameLabel = new QLabel();
        nameLabel->setText(nowPokemon["name"].toString() + " lv:" + QString::number(nowPokemon["level"].toInt()));

        vlayout->addWidget(pokemonBtn);
        vlayout->addWidget(nameLabel);

        glayout->addLayout(vlayout, i / 4, i % 4);
        QObject::connect(pokemonBtn, SIGNAL(clicked()), this, SLOT(showDetails()));
    }

    if (totNum % 4 != 0)
    {
        for (int j = totNum % 4 + 1; j <= 4; ++j)
        {
            QLabel *nameLabel = new QLabel("");
            glayout->addWidget(nameLabel, totNum / 4, j);
        }
    }

    glayout->setAlignment(Qt::AlignTop);
    ui->scrollArea->widget()->setLayout(glayout);

    int battleNumber = pokemonInfo["battle"].toInt();
    int winNumber = pokemonInfo["win"].toInt();
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

void UserList::setButtonBackImage(QPushButton *button,QString image,int sizeW, int sizeH)
{
    QPixmap pixmap(image);
    QPixmap fitpixmap=pixmap.scaled(200,200).scaled(sizeW, sizeH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    button->setIcon(QIcon(fitpixmap));
    button->setIconSize(QSize(sizeW,sizeH));
    button->setFlat(true);
    button->setStyleSheet("border: 0px");
}

void UserList::showPokemons()
{
    int id = userGroup->checkedId();

    qDebug() << "id=" << id;
    QJsonObject nowUser = userInfo["user" + QString::number(id)].toObject();
    qDebug() << "当前选中的用户是:" << nowUser;
    sendRequest("GET_USER_POKEMON", nowUser["username"].toString());
}

void UserList::showDetails()
{
    int id = pokemonGroup->checkedId();
    qDebug() << "id=" << id;
    QJsonObject nowPokemon = pokemonInfo["pokemon" + QString::number(id)].toObject();
    qDebug() << nowPokemon["name"].toString();

    QPixmap img;
    img.load(":/icon/pokemons/" + nowPokemon["engname"].toString() + ".png");
    img.scaled(ui->pokemonPic->size(), Qt::KeepAspectRatio);
    ui->pokemonPic->setScaledContents(true);
    ui->pokemonPic->setPixmap(img);

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


void UserList::on_pushButton_clicked()
{
    delete this;
}
