#include "selectpokemon.h"
#include "ui_selectpokemon.h"

#include <QIcon>
#include <QDebug>
#include <QLabel>
#include <QPixmap>
#include <QString>
#include <QByteArray>
#include <QMessageBox>
#include <QJsonDocument>
#include <QNetworkDatagram>

const int CLIENT_PORT = 11111;
const int SERVER_PORT = 11110;


SelectPokemon::SelectPokemon(QString userName, QString enemyName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectPokemon)
{
    ui->setupUi(this);

    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setAutoFillBackground(true);
    QPalette pal = this->palette();
    pal.setBrush(backgroundRole(), QPixmap(":/back/background/back.png"));
    setPalette(pal);
    hide();

    UdpInit();

    minePokemon = NULL;
    enemyPokemon = NULL;

    username = userName;
    enemyname = enemyName;

    sendOver = false;
    nowrole = 1;
    sendRequest(username);

}

SelectPokemon::~SelectPokemon()
{
    delete ui;
}

void SelectPokemon::UdpInit()
{
    clientudp=new QUdpSocket();
    port = CLIENT_PORT;
    while (!clientudp->bind(port,QUdpSocket::ShareAddress))
        port ++;
    QObject::connect(clientudp,SIGNAL(readyRead()),this,SLOT(receiveData()));
}

void SelectPokemon::UIInit()
{

}

void SelectPokemon::sendRequest(QString userName)
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

void SelectPokemon::receiveData()
{
    while(clientudp->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = clientudp->receiveDatagram();
        analysisByteArray(datagram.data());
    }
    if (!sendOver)
    {
        sendOver = true;
        nowrole = 2;
        sendRequest(enemyname);
    }
}

void SelectPokemon::setButtonBackImage(QPushButton *button,QString image,int sizeW, int sizeH)
{
    QPixmap pixmap(image);
    QPixmap fitpixmap=pixmap.scaled(100,100).scaled(sizeW, sizeH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    button->setIcon(QIcon(fitpixmap));
    button->setIconSize(QSize(sizeW,sizeH));
    button->setFlat(true);
    button->setStyleSheet("border: 0px");
}
/*
 * 布局已经大体完成
 * 但是可能要从每行4个变为每行3个（调整布局）
 */

void SelectPokemon::analysisByteArray(QByteArray recData)
{
    QJsonParseError json_error;
    QJsonDocument recDoc(QJsonDocument::fromJson(recData, &json_error));
    if(json_error.error != QJsonParseError::NoError){
        qDebug() << "json error!";
    }
    recInfo[nowrole] = recDoc.object();

    if (nowrole == 1)
        setInscrollArea(ui->scrollArea1);
    else
        setInscrollArea(ui->scrollArea2);

}

Pokemon* SelectPokemon::createPokemon(QJsonObject pokemonInfo)
{
    Pokemon* tempPokemon;

    int id = pokemonInfo["id"].toInt();
    QString name = pokemonInfo["name"].toString();
    Category kind = Category(pokemonInfo["kind"].toInt());
    Attribute type = Attribute(pokemonInfo["type"].toInt());
    int level = pokemonInfo["level"].toInt();
    int exp = pokemonInfo["exp"].toInt();
    int atk = pokemonInfo["atk"].toInt();
    int def = pokemonInfo["def"].toInt();
    int maxhp = pokemonInfo["hp"].toInt();
    int agl = pokemonInfo["agl"].toInt();
    QString skill = pokemonInfo["skill"].toString();
    QString engname = pokemonInfo["engname"].toString();
    switch (type) {
        case NormalT:
            tempPokemon = new Normal(id, name, kind, level, exp, atk, def, maxhp, agl, skill, engname);
            break;
        case FireT:
            tempPokemon = new Fire(id, name, kind, level, exp, atk, def, maxhp, agl, skill, engname);
            break;
        case WaterT:
            tempPokemon = new Water(id, name, kind, level, exp, atk, def, maxhp, agl, skill, engname);
            break;
        case GrassT:
            tempPokemon = new Grass(id, name, kind, level, exp, atk, def, maxhp, agl, skill, engname);
            break;
        case ElectricT:
            tempPokemon = new Electric(id, name, kind, level, exp, atk, def, maxhp, agl, skill, engname);
            break;
    }
    return tempPokemon;
}

void SelectPokemon::showDetails1()
{
    int id = btnGroup1->checkedId();
    qDebug() << "id=" << id;
    QJsonObject nowPokemon = recInfo[1]["pokemon" + QString::number(id)].toObject();
    qDebug() << nowPokemon["name"].toString();

    minePokemon = createPokemon(nowPokemon);

    QPixmap img;
    img = minePokemon->returnPic(1);

    img.scaled(ui->minePokemon->size(), Qt::KeepAspectRatio);

    ui->minePokemon->setScaledContents(true);

    ui->minePokemon->setPixmap(img);

//    if (enemyPokemon != NULL)
//    {
//        setRestraint();
//    }



    ui->name1->setText(nowPokemon["name"].toString());
    ui->type1->setText(Pokemon::showAttribute( nowPokemon["type"].toInt() ));
    ui->kind1->setText(Pokemon::showCategory( nowPokemon["kind"].toInt() ));
    ui->level1->setText(QString::number( nowPokemon["level"].toInt() ));
    ui->exp1->setText(QString::number( nowPokemon["exp"].toInt() ));
    ui->atk1->setText(QString::number( nowPokemon["atk"].toInt() ));
    ui->def1->setText(QString::number( nowPokemon["def"].toInt() ));
    ui->hp1->setText(QString::number( nowPokemon["hp"].toInt() ));
    ui->agl1->setText(QString::number( nowPokemon["agl"].toInt() ));
    ui->delay1->setText(QString::number( 300 - nowPokemon["agl"].toInt() ));
    ui->skill1->setText(nowPokemon["skill"].toString());
    return ;
}

void SelectPokemon::showDetails2()
{
    int id = btnGroup2->checkedId();
    qDebug() << "id=" << id;
    QJsonObject nowPokemon = recInfo[2]["pokemon" + QString::number(id)].toObject();
    qDebug() << nowPokemon["name"].toString();

    enemyPokemon = createPokemon(nowPokemon);

    QPixmap img;
    img = enemyPokemon->returnPic(0);

    img.scaled(ui->enemyPokemon->size(), Qt::KeepAspectRatio);

    ui->enemyPokemon->setScaledContents(true);

    ui->enemyPokemon->setPixmap(img);

//    if (minePokemon != NULL)
//    {
//        setRestraint();
//    }

    ui->name2->setText(nowPokemon["name"].toString());
    ui->type2->setText(Pokemon::showAttribute( nowPokemon["type"].toInt() ));
    ui->kind2->setText(Pokemon::showCategory( nowPokemon["kind"].toInt() ));
    ui->level2->setText(QString::number( nowPokemon["level"].toInt() ));
    ui->exp2->setText(QString::number( nowPokemon["exp"].toInt() ));
    ui->atk2->setText(QString::number( nowPokemon["atk"].toInt() ));
    ui->def2->setText(QString::number( nowPokemon["def"].toInt() ));
    ui->hp2->setText(QString::number( nowPokemon["hp"].toInt() ));
    ui->agl2->setText(QString::number( nowPokemon["agl"].toInt() ));
    ui->delay2->setText(QString::number( 300 - nowPokemon["agl"].toInt() ));
    ui->skill2->setText(nowPokemon["skill"].toString());
    return ;
}

//void SelectPokemon::setRestraint()
//{
//    qDebug() << minePokemon->state.type << enemyPokemon->state.type;
//    double value = attributeRestraint[int(minePokemon->state.type)][int(enemyPokemon->state.type)];
//    QImage img1(":/back/background/good.png"), img2(":/back/background/bad.png");
//    QPixmap pix1 = QPixmap::fromImage(img1), pix2 = QPixmap::fromImage(img2);
//    pix1.scaled(ui->state1->size(), Qt::KeepAspectRatio );
//    pix2.scaled(ui->state2->size(), Qt::KeepAspectRatio );
//    ui->state1->setScaledContents(true);
//    ui->state2->setScaledContents(true);
//    if (minePokemon->state.type != enemyPokemon->state.type)
//    {
//        if (value <0.99)
//        {
//            ui->state1->setPixmap(pix2);
//            ui->state2->setPixmap(pix1);

//        }
//        else if (value > 1.01)
//        {
//            ui->state1->setPixmap(pix1);
//            ui->state2->setPixmap(pix2);

//        }
//        else
//            ui->state2->clear();
//        {
//            ui->state1->clear();
//        }
//    }
//    else
//    {
//        ui->state1->clear();
//        ui->state2->clear();
//    }

//}

void SelectPokemon::setInscrollArea(QScrollArea* scrollArea)
{
    glayout = new QGridLayout();

    int totNum = recInfo[nowrole]["number"].toInt();
    qDebug() << totNum;
    if (nowrole == 1)
        btnGroup1 = new QButtonGroup(this);
    else
        btnGroup2 = new QButtonGroup(this);

    for (int i = 0; i < totNum; ++i)
    {
        QJsonObject nowPokemon = recInfo[nowrole]["pokemon" + QString::number(i)].toObject();

        QPushButton* pokemonBtn = new QPushButton(this);
        pokemonBtn->setCheckable(true);
        if (nowrole == 1)
            btnGroup1->addButton(pokemonBtn, i);
        else
            btnGroup2->addButton(pokemonBtn, i);
        setButtonBackImage(pokemonBtn, ":/icon/pokemons/" + nowPokemon["engname"].toString() + ".png", 105, 105);

        QVBoxLayout *vlayout = new QVBoxLayout();

        QLabel *nameLabel = new QLabel();
        nameLabel->setText(nowPokemon["name"].toString() + " lv:" + QString::number(nowPokemon["level"].toInt()));

        vlayout->addWidget(pokemonBtn);
        vlayout->addWidget(nameLabel);

        glayout->addLayout(vlayout, i / 4, i % 4);
        if (nowrole == 1)
            QObject::connect(pokemonBtn, SIGNAL(clicked()), this, SLOT(showDetails1()));
        else
            QObject::connect(pokemonBtn, SIGNAL(clicked()), this, SLOT(showDetails2()));
    }

    if (totNum % 4 != 4)
    {
        for (int j = totNum % 4 + 1; j <= 4; ++j)
        {
            QLabel *nameLabel = new QLabel("");
            glayout->addWidget(nameLabel, totNum / 4, j);
        }
    }

    glayout->setAlignment(Qt::AlignTop);
    scrollArea->widget()->setLayout(glayout);
}

void SelectPokemon::on_pushButton_clicked()
{
    if (minePokemon == NULL || enemyPokemon == NULL)
        QMessageBox::warning(this, "错误", "精灵不能为空");
    else
        emit selectedPokemons(minePokemon, enemyPokemon);
}

void SelectPokemon::on_pushButton_2_clicked()
{
    delete this;
}
