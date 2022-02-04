#include "battleresult.h"
#include "ui_battleresult.h"

#include <QFont>
#include <QDebug>
#include <QPixmap>
#include <QThread>
#include <QString>
#include <QByteArray>
#include <QScrollArea>
#include <QMessageBox>
#include <QJsonDocument>
#include <QNetworkDatagram>
#include <QGraphicsOpacityEffect>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>

const int CLIENT_PORT = 11111;
const int SERVER_PORT = 11110;

BattleResult::BattleResult(QString userName, Pokemon* MinePokemon, Pokemon* EnemyPokemon, int Mode, int Result, int Difficulty,  QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BattleResult)
{
    ui->setupUi(this);

    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setAutoFillBackground(true);
    QPalette pal = this->palette();
    pal.setBrush(backgroundRole(), QPixmap(":/back/background/back.png"));
    setPalette(pal);
    this->setFixedSize(1280,960);
    hide();

    username = userName;
    minePokemon = MinePokemon;
    enemyPokemon = EnemyPokemon;
    mode = Mode;
    result = Result;
    difficulty = Difficulty;
    selectedId = -1;
    pokemonAllOut = false;

    UIInit();
    UdpInit();

    showResult();


}

BattleResult::~BattleResult()
{
    delete ui;
}

void BattleResult::UIInit()
{
//    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
//    hide();
}

void BattleResult::UdpInit()
{
    clientudp=new QUdpSocket(this);
    port = CLIENT_PORT;
    while (!clientudp->bind(port,QUdpSocket::ShareAddress))
        port ++;
    connect(clientudp,SIGNAL(readyRead()),this,SLOT(receiveData()));
}

void BattleResult::sendRequest(QString optype)
{
    qDebug() << optype;
    if (optype == "UPDATE_POKEMON_INFO"){
        QJsonObject pokemonInfo = minePokemon->toJson();
        pokemonInfo.insert("optype", optype);

        qDebug() << pokemonInfo;

        QJsonDocument pokemonDoc;
        pokemonDoc.setObject(pokemonInfo);
        QByteArray pokemonData = pokemonDoc.toJson();
        clientudp->writeDatagram(pokemonData, QHostAddress::LocalHost, SERVER_PORT);
    }
    if (optype == "LOSE_BATTLE")
    {
        QJsonObject userInfo;
        userInfo.insert("optype", optype);
        userInfo.insert("username", username);
        QJsonDocument userDoc;
        userDoc.setObject(userInfo);
        QByteArray userData = userDoc.toJson();
        clientudp->writeDatagram(userData, QHostAddress::LocalHost, SERVER_PORT);
    }
    if (optype == "DROP_POKEMON")
    {
        QJsonObject pokemonInfo;
        pokemonInfo.insert("optype", optype);
        pokemonInfo.insert("id", selectedPokemon["id"].toInt());
        qDebug() << "丢弃"  << pokemonInfo;
        QJsonDocument pokemonDoc;
        pokemonDoc.setObject(pokemonInfo);
        QByteArray pokemonData = pokemonDoc.toJson();
        clientudp->writeDatagram(pokemonData, QHostAddress::LocalHost, SERVER_PORT);
    }
    if (optype == "WIN_POKEMON")
    {
        QJsonObject pokemonInfo = enemyPokemon->toJson();
        pokemonInfo.insert("username", username);
        pokemonInfo.insert("optype", optype);
        QJsonDocument pokemonDoc;
        pokemonDoc.setObject(pokemonInfo);
        QByteArray pokemonData = pokemonDoc.toJson();
        clientudp->writeDatagram(pokemonData, QHostAddress::LocalHost, SERVER_PORT);
    }
    if (optype == "ALL_OUT" || optype == "WIN" || optype == "LOSE")
    {
        QJsonObject userInfo;
        userInfo.insert("optype", optype);
        userInfo.insert("username", username);
        QJsonDocument userDoc;
        userDoc.setObject(userInfo);
        QByteArray userData = userDoc.toJson();
        clientudp->writeDatagram(userData, QHostAddress::LocalHost, SERVER_PORT);
    }
}

void BattleResult::receiveData()
{
    while(clientudp->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = clientudp->receiveDatagram();
        analysisByteArray(datagram.data());
    }
}

void BattleResult::setButtonBackImage(QPushButton *button,QString image,int sizeW, int sizeH)
{

    QPixmap pixmap(image);
    QPixmap fitpixmap=pixmap.scaled(200,200).scaled(sizeW, sizeH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    button->setIcon(QIcon(fitpixmap));
    button->setIconSize(QSize(sizeW,sizeH));
    button->setFlat(true);
    button->setStyleSheet("border: 0px");
}

void BattleResult::analysisByteArray(QByteArray recData)
{
    QJsonParseError json_error;
    QJsonDocument recDoc(QJsonDocument::fromJson(recData, &json_error));
    if(json_error.error != QJsonParseError::NoError){
        qDebug() << "json error!";
    }
    recInfo = recDoc.object();

    if (recInfo["result"] != "pokemonlist")
    {
        return;
    }

    QHBoxLayout *hlayout = new QHBoxLayout();

    int totNum = recInfo["number"].toInt();
    qDebug() << totNum;
    btnGroup = new QButtonGroup(this);
    if (totNum == 1)
    {
        pokemonAllOut = true;
    }
    for (int i = 0; i < totNum; ++i)
    {
        QJsonObject nowPokemon = recInfo["pokemon" + QString::number(i)].toObject();

        QPushButton* pokemonBtn = new QPushButton(this);
        pokemonBtn->setCheckable(true);
        btnGroup->addButton(pokemonBtn, i);
        setButtonBackImage(pokemonBtn, ":/icon/pokemons/" + nowPokemon["engname"].toString() + ".png", 200, 200);

        QVBoxLayout *vlayout = new QVBoxLayout();

        QLabel *nameLabel = new QLabel();
        nameLabel->setText(nowPokemon["name"].toString() + " lv:" + QString::number(nowPokemon["level"].toInt()));

        vlayout->addWidget(pokemonBtn);
        vlayout->addWidget(nameLabel);

        hlayout->addLayout(vlayout, i);
        QObject::connect(pokemonBtn, SIGNAL(clicked()), this, SLOT(showDetails()));
    }

    for (int i = totNum + 1; i <= 3; ++i)
    {
        QLabel* labelFilled = new QLabel();
         hlayout->addWidget(labelFilled, i);
    }

    QWidget* widget = new QWidget(this);
    widget->setGeometry(237,291,996-237,532-291);
    widget->setLayout(hlayout);
    widget->show();

    QPushButton* confirmBtn = new QPushButton(this);
    confirmBtn->setText("确认");
    QFont font1("Microsoft YaHei", 20, 75);
    confirmBtn->setFont(font1);
    confirmBtn->setGeometry(485, 712, 705-485, 804-712);
    confirmBtn->show();

    QObject::connect(confirmBtn, SIGNAL(clicked()), this, SLOT(dropPokemon()));

}

void BattleResult::dropPokemon()
{
    if (selectedId < 0)
       QMessageBox::critical(NULL, "错误", "请选择要丢弃的宝可梦", QMessageBox::Yes, QMessageBox::Yes);
    else
    {
        QMessageBox:: StandardButton result=
                QMessageBox::warning(NULL, "警告", "确认要丢弃" + selectedPokemon["name"].toString() + "吗？",
                QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        switch (result)
        {
        case QMessageBox::Yes:
            qDebug()<<"Yes";
            sendRequest("DROP_POKEMON");
            if (pokemonAllOut == true)
            {
                QMessageBox::information(NULL, "提示", "您已经失去所有宝可梦，因此系统将随机赠送您一个初级精灵", QMessageBox::Yes, QMessageBox::Yes);
                sendRequest("ALL_OUT");
            }
            endResult();

            break;
        case QMessageBox::No:
            qDebug()<<"NO";
            break;
        }


    }
}

void BattleResult::showDetails()
{

    selectedId = btnGroup->checkedId();
    qDebug() << "selectedId=" << selectedId;
    selectedPokemon = recInfo["pokemon" + QString::number(selectedId)].toObject();
    return ;
}

void BattleResult::showResult()
{
    resultLabel = new QLabel(this);
    QFont font1("Microsoft YaHei", 50, 75);
    resultLabel->setFont(font1);
    if (result == 1)
    {
        resultLabel->setText("胜利！");
        resultLabel->setStyleSheet("color:yellow;");
        sendRequest("WIN");
    }
    else
    {
        resultLabel->setText("失败！");
        resultLabel->setStyleSheet("color:grey;");
        sendRequest("LOSE");
    }
    resultLabel->setGeometry(480, 420, 800-460, 510 - 420);

    QGraphicsOpacityEffect *pLabelOpacity = new QGraphicsOpacityEffect(this);
    pLabelOpacity->setOpacity(1);
    resultLabel->setGraphicsEffect(pLabelOpacity);

    QPropertyAnimation *pResultAnimation1 = new QPropertyAnimation(pLabelOpacity, "opacity");
    pResultAnimation1->setDuration(400);
    pResultAnimation1->setStartValue(0);
    pResultAnimation1->setEndValue(1);

    QPropertyAnimation *pResultAnimation2 = new QPropertyAnimation(pLabelOpacity, "opacity");
    pResultAnimation2->setDuration(1200);
    pResultAnimation2->setStartValue(1);
    pResultAnimation2->setEndValue(0);

    QSequentialAnimationGroup *pPosGroup = new QSequentialAnimationGroup(this);
    pPosGroup->addAnimation(pResultAnimation1);
    pPosGroup->addAnimation(pResultAnimation2);

    resultLabel->show();
    pPosGroup->start();

    QObject::connect(pPosGroup, SIGNAL(finished()), this, SLOT(showResultOver()));

}

void BattleResult::showExp()
{
    QLabel* infoLabel = new QLabel(this);
    infoLabel->setText("获得经验");
    QFont font1("Microsoft YaHei", 20, 75);
    infoLabel->setGeometry(136,45,200,50);
    infoLabel->setFont(font1);
    infoLabel->show();

    qDebug() << "run showExp";
    QJsonObject oldPokemon = minePokemon->toJson();
    minePokemon->getExp(difficulty);
    QJsonObject newPokemon = minePokemon->toJson();

    QLabel* pokemonPic = new QLabel(this);
    pokemonPic->setGeometry(145, 222, 399-145, 442-222);
    QPixmap img;
    img = minePokemon->returnPic(0);
    img.scaled(pokemonPic->size(), Qt::KeepAspectRatio);
    pokemonPic->setScaledContents(true);
    pokemonPic->setPixmap(img);
    pokemonPic->show();

    QGridLayout *glayout = new QGridLayout();
    glayout->setGeometry(QRect(0, 0, 400-144, 821-485));
    int change;

    QLabel *nameInfo = new QLabel(this);
    QLabel *nameValue = new QLabel(this);
    nameInfo->setText("名字");
    nameValue->setText(newPokemon["name"].toString());
    glayout->addWidget(nameInfo, 0, 0);
    glayout->addWidget(nameValue, 0, 1);

    QLabel *levelInfo = new QLabel(this);
    QLabel *levelValue = new QLabel(this);
    QLabel *levelChange = new QLabel(this);
    levelInfo->setText("等级");
    levelValue->setText(QString::number( newPokemon["level"].toInt()));
    change = newPokemon["level"].toInt() - oldPokemon["level"].toInt();
    if (change > 0)
        levelChange->setText("+" + QString::number(change));
    else
        levelChange->setText("");
    glayout->addWidget(levelInfo, 1, 0);
    glayout->addWidget(levelValue, 1, 1);
    glayout->addWidget(levelChange, 1, 2);

    QLabel *expInfo = new QLabel(this);
    QLabel *expValue = new QLabel(this);
    QLabel *expChange = new QLabel(this);
    expInfo->setText("经验值");
    expValue->setText(QString::number( newPokemon["exp"].toInt()));
    change = newPokemon["exp"].toInt() - oldPokemon["exp"].toInt();
    if (change > 0)
        expChange->setText("+" + QString::number(change));
    else
        expChange->setText("");
    glayout->addWidget(expInfo, 2, 0);
    glayout->addWidget(expValue, 2, 1);
    glayout->addWidget(expChange, 2, 2);

    QLabel *atkInfo = new QLabel(this);
    QLabel *atkValue = new QLabel(this);
    QLabel *atkChange = new QLabel(this);
    atkInfo->setText("攻击");
    atkValue->setText(QString::number( newPokemon["atk"].toInt()));
    change = newPokemon["atk"].toInt() - oldPokemon["atk"].toInt();
    if (change > 0)
        atkChange->setText("+" + QString::number(change));
    else
        atkChange->setText("");
    glayout->addWidget(atkInfo, 3, 0);
    glayout->addWidget(atkValue, 3, 1);
    glayout->addWidget(atkChange, 3, 2);

    QLabel *defInfo = new QLabel(this);
    QLabel *defValue = new QLabel(this);
    QLabel *defChange = new QLabel(this);
    defInfo->setText("防御");
    defValue->setText(QString::number( newPokemon["def"].toInt()));
    change = newPokemon["def"].toInt() - oldPokemon["def"].toInt();
    if (change > 0)
        defChange->setText("+" + QString::number(change));
    else
        defChange->setText("");
    glayout->addWidget(defInfo, 4, 0);
    glayout->addWidget(defValue, 4, 1);
    glayout->addWidget(defChange, 4, 2);

    QLabel *hpInfo = new QLabel(this);
    QLabel *hpValue = new QLabel(this);
    QLabel *hpChange = new QLabel(this);
    hpInfo->setText("血量");
    hpValue->setText(QString::number( newPokemon["maxhp"].toInt()));
    change = newPokemon["hp"].toInt() - oldPokemon["maxhp"].toInt();
    if (change > 0)
        hpChange->setText("+" + QString::number(change));
    else
        hpChange->setText("");
    glayout->addWidget(hpInfo, 5, 0);
    glayout->addWidget(hpValue, 5, 1);
    glayout->addWidget(hpChange, 5, 2);

    QLabel *aglInfo = new QLabel(this);
    QLabel *aglValue = new QLabel(this);
    QLabel *aglChange = new QLabel(this);
    aglInfo->setText("敏捷");
    aglValue->setText(QString::number( newPokemon["agl"].toInt()));
    change = newPokemon["agl"].toInt() - oldPokemon["agl"].toInt();
    if (change > 0)
        aglChange->setText("+" + QString::number(change));
    else
        aglChange->setText("");
    glayout->addWidget(aglInfo, 6, 0);
    glayout->addWidget(aglValue, 6, 1);
    glayout->addWidget(aglChange, 6, 2);

    QLabel *delayInfo = new QLabel(this);
    QLabel *delayValue = new QLabel(this);
    QLabel *delayChange = new QLabel(this);
    delayInfo->setText("攻击间隔");
    delayValue->setText(QString::number(300 - newPokemon["agl"].toInt()));
    change = newPokemon["agl"].toInt() - oldPokemon["agl"].toInt();
    if (change > 0)
        delayChange->setText("-" + QString::number(change));
    else
        delayChange->setText("");
    glayout->addWidget(delayInfo, 7, 0);
    glayout->addWidget(delayValue, 7, 1);
    glayout->addWidget(delayChange, 7, 2);

    QWidget* widget = new QWidget(this);
    widget->setGeometry(144, 485, 400-144, 821-485);
    widget->setLayout(glayout);
    widget->show();

    if (mode == 1)
    {
        QLabel* infoLabel = new QLabel(this);
        infoLabel->setText("获得新的宝可梦");
        QFont font1("Microsoft YaHei", 20, 75);
        infoLabel->setGeometry(837,45,250,50);
        infoLabel->setFont(font1);
        infoLabel->show();

        qDebug() << "run showGet";
        QJsonObject getPokemon = enemyPokemon->toJson();

        QLabel* pokemonPic = new QLabel(this);
        pokemonPic->setGeometry(843, 222, 399-145, 442-222);
        QPixmap img;
        img = enemyPokemon->returnPic(0);
        img.scaled(pokemonPic->size(), Qt::KeepAspectRatio);
        pokemonPic->setScaledContents(true);
        pokemonPic->setPixmap(img);
        pokemonPic->show();

        QGridLayout *glayout = new QGridLayout();
        glayout->setGeometry(QRect(0, 0, 400-144, 821-485));

        QLabel *nameInfo = new QLabel(this);
        QLabel *nameValue = new QLabel(this);
        nameInfo->setText("名字");
        nameValue->setText(getPokemon["name"].toString());
        glayout->addWidget(nameInfo, 0, 0);
        glayout->addWidget(nameValue, 0, 1);

        QLabel *levelInfo = new QLabel(this);
        QLabel *levelValue = new QLabel(this);
        levelInfo->setText("等级");
        levelValue->setText(QString::number( getPokemon["level"].toInt()));
        glayout->addWidget(levelInfo, 1, 0);
        glayout->addWidget(levelValue, 1, 1);

        QLabel *expInfo = new QLabel(this);
        QLabel *expValue = new QLabel(this);
        expInfo->setText("经验值");
        expValue->setText(QString::number( getPokemon["exp"].toInt()));
        glayout->addWidget(expInfo, 2, 0);
        glayout->addWidget(expValue, 2, 1);

        QLabel *atkInfo = new QLabel(this);
        QLabel *atkValue = new QLabel(this);
        atkInfo->setText("攻击");
        atkValue->setText(QString::number( getPokemon["atk"].toInt()));
        glayout->addWidget(atkInfo, 3, 0);
        glayout->addWidget(atkValue, 3, 1);

        QLabel *defInfo = new QLabel(this);
        QLabel *defValue = new QLabel(this);
        defInfo->setText("防御");
        defValue->setText(QString::number( getPokemon["def"].toInt()));
        glayout->addWidget(defInfo, 4, 0);
        glayout->addWidget(defValue, 4, 1);

        QLabel *hpInfo = new QLabel(this);
        QLabel *hpValue = new QLabel(this);
        hpInfo->setText("血量");
        hpValue->setText(QString::number( getPokemon["maxhp"].toInt()));
        glayout->addWidget(hpInfo, 5, 0);
        glayout->addWidget(hpValue, 5, 1);

        QLabel *aglInfo = new QLabel(this);
        QLabel *aglValue = new QLabel(this);
        aglInfo->setText("敏捷");
        aglValue->setText(QString::number( getPokemon["agl"].toInt()));
        glayout->addWidget(aglInfo, 6, 0);
        glayout->addWidget(aglValue, 6, 1);

        QLabel *delayInfo = new QLabel(this);
        QLabel *delayValue = new QLabel(this);
        delayInfo->setText("攻击间隔");
        delayValue->setText(QString::number(300 - getPokemon["agl"].toInt()));
        glayout->addWidget(delayInfo, 7, 0);
        glayout->addWidget(delayValue, 7, 1);

        QWidget* widget = new QWidget(this);
        widget->setGeometry(843, 485, 400-144, 821-485);
        widget->setLayout(glayout);
        widget->show();

        sendRequest("WIN_POKEMON");
    }


    QPushButton* confirmBtn = new QPushButton(this);
    confirmBtn->setText("确认");
    confirmBtn->setGeometry(540, 666, 698-540, 728-666);
    confirmBtn->setFont(font1);
    confirmBtn->show();



    QObject::connect(confirmBtn, SIGNAL(clicked()), this, SLOT(endResult()));


}


void BattleResult::showChoose()
{
    QLabel* infoLabel = new QLabel (this);
    infoLabel->setText("请选择一个丢弃的精灵");
    infoLabel->setGeometry(410, 86, 772-410, 195-86);
    QFont font1("Microsoft YaHei", 20, 75);
    infoLabel->setFont(font1);
    infoLabel->show();
    sendRequest("LOSE_BATTLE");
}


void BattleResult::showResultOver()
{
    if (result == 1)
        showExp();
    else
        if (mode == 0)
            endResult();
        else
            showChoose();
}


void BattleResult::endResult()
{
    if (mode == 1 && result == 1)
        sendRequest("UPDATE_POKEMON_INFO");
    emit resultWindowOver();
    this->hide();
}
