#include "battle.h"
#include "ui_battle.h"

#include <QDebug>
#include <QPoint>
#include <QThread>
#include <QPixmap>
#include <QMessageBox>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QGraphicsOpacityEffect>

int UPDATE_TIMER = 10; //定时器设置为10ms
int SPEED_TIME = 1; //默认战斗一倍速

const QString KIND_COLOR[5] = {"#A8A878", "#F08030", "#6890F0", "#78C850", "#F8D030"};

Battle::Battle(int Mode, QString Username, Pokemon* MinePokemon, Pokemon* EnemyPokemon, int Difficulty, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Battle)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setAutoFillBackground(true);
    QPalette pal = this->palette();
    pal.setBrush(backgroundRole(), QPixmap(":/back/background/back.png"));
    setPalette(pal);
    this->setFixedSize(1280,960);
    hide();

    mode = Mode;
    username = Username;
    minePokemon = MinePokemon;
    enemyPokemon = EnemyPokemon;
    difficulty = Difficulty;

    SPEED_TIME = 1;
    AnimationInit();
    BattleInit();
    UIInit();
    TimerInit();
}

Battle::~Battle()
{
    delete ui;
}

void Battle::AnimationInit()
{
    recA = ui->picA->frameGeometry();
    recB = ui->picB->frameGeometry();
    qDebug() << recA.x() << recA.y();
    qDebug() << recB.x() << recB.y();
}


void Battle::UIInit()
{
    QPixmap imgA, imgB;
    imgA = minePokemon->returnPic(1);
    imgB = enemyPokemon->returnPic(0);

    imgA.scaled(ui->picA->size(), Qt::KeepAspectRatio);
    imgB.scaled(ui->picB->size(), Qt::KeepAspectRatio);

    ui->picA->setScaledContents(true);
    ui->picB->setScaledContents(true);

    ui->picA->setPixmap(imgA);
    ui->picB->setPixmap(imgB);

    ui->hpA->setText(QString::number(minePokemon->state.hp) + "/" + QString::number(minePokemon->state.maxhp));
    ui->hpPerA->setValue(100.0);

    ui->hpB->setText(QString::number(enemyPokemon->state.hp) + "/" + QString::number(enemyPokemon->state.maxhp));
    ui->hpPerB->setValue(100.0);

    ui->skill1->hide();
    ui->skill2->hide();
    ui->back1->hide();
    ui->back2->hide();

}

void Battle::BattleInit()
{
    death = 0;
    minePokemon->stateInitialization();
    enemyPokemon->stateInitialization();
}

void Battle::TimerInit()
{
    nowTimeA = 0;
    nowTimeB = 0;
    PAUSE = 0;
    myTimer = new QTimer(this);
    myTimer->start(UPDATE_TIMER);
    QObject::connect(myTimer, SIGNAL(timeout()), this, SLOT(process()));
}


void Battle::restartTimer()
{
    PAUSE = 0;
    myTimer->start(UPDATE_TIMER / SPEED_TIME);
}

void Battle::deleteValue()
{
    delete damageLabel;
}

void Battle::showDamage1()
{
    upDateBuff();

    qDebug() << minePokemon->state.isCritical;
    if (minePokemon->state.isDodge == 1){
        damageLabel->setStyleSheet("color:grey;");
        damageLabel->setText("MISS");
    }
    else if (minePokemon->state.isCritical == 1)
        damageLabel->setStyleSheet("color:red;");
    else
        damageLabel->setStyleSheet("color:yellow;");
    damageLabel->setGeometry(recB.x(), recB.y() + recB.height() / 2.0, 100, 20);
    damageLabel->show();
    valueAnimation->setDuration(400  / SPEED_TIME);
    valueAnimation->setStartValue(QPoint(recB.x() , recB.y() + recB.height()/ 2.0));
    valueAnimation->setEndValue(QPoint(recB.x(), recB.y() - recB.height() / 2.0));
    valueAnimation->setEasingCurve(QEasingCurve::OutInQuad);
    valueAnimation->start();

    QObject::connect(valueAnimation, SIGNAL(finished()), this, SLOT(deleteValue()));


    ui->hpB->setText(QString::number(enemyPokemon->state.hp) + "/" + QString::number(enemyPokemon->state.maxhp));
    ui->hpPerB->setValue(100.0 * enemyPokemon->state.hp / enemyPokemon->state.maxhp);

    if (enemyPokemon->state.hp == 0) death = 2;
}

void Battle::showDamage2()
{
    upDateBuff();

    if (enemyPokemon->state.isDodge == 1){
        damageLabel->setStyleSheet("color:grey;");
        damageLabel->setText("MISS");
    }
    else if (enemyPokemon->state.isCritical == 1)
        damageLabel->setStyleSheet("color:red;");
    else
        damageLabel->setStyleSheet("color:yellow;");

    damageLabel->setGeometry(recA.x(), recA.y() + recA.height() / 2.0, 100, 100);
    damageLabel->show();
    valueAnimation->setDuration(400  / SPEED_TIME);
    valueAnimation->setStartValue(QPoint(recA.x() , recA.y() + recA.height()/ 2.0));
    valueAnimation->setEndValue(QPoint(recA.x(), recA.y() - recA.height() / 2.0));
    valueAnimation->start();

    QObject::connect(valueAnimation, SIGNAL(finished()), this, SLOT(deleteValue()));

    ui->hpA->setText(QString::number(minePokemon->state.hp) + "/" + QString::number(minePokemon->state.maxhp));
    ui->hpPerA->setValue(100.0 * minePokemon->state.hp / minePokemon->state.maxhp);

    if (minePokemon->state.hp == 0) death = 1;
}

void Battle::upDateBuff()
{
    ui->minebuff1->clear();
    ui->minebuff1N->clear();
    ui->minebuff2->clear();
    ui->minebuff2N->clear();

    ui->enemybuff1->clear();
    ui->enemybuff1N->clear();
    ui->enemybuff2->clear();
    ui->enemybuff2N->clear();

    if (minePokemon->state.buff.upAtk > 0){
        ui->minebuff1->setText("Atk↑");
        ui->minebuff1N->setText(QString::number(minePokemon->state.buff.upAtk));
    }

    if (minePokemon->state.buff.downAtk > 0){
       ui->minebuff2->setText("Atk↓");
       ui->minebuff2N->setText(QString::number(minePokemon->state.buff.downAtk));
    }

    if (minePokemon->state.buff.downDef > 0){
       ui->minebuff2->setText("Def↓");
       ui->minebuff2N->setText(QString::number(minePokemon->state.buff.downDef));
    }

    if (minePokemon->state.buff.downAgl > 0){
       ui->minebuff2->setText("Agl↓");
       ui->minebuff2N->setText(QString::number(minePokemon->state.buff.downAgl));
    }

    if (minePokemon->state.buff.onFire > 0){
       ui->minebuff2->setText("Fire");
       ui->minebuff2N->setText(QString::number(minePokemon->state.buff.onFire));
    }

    if (enemyPokemon->state.buff.upAtk > 0){
        ui->enemybuff1->setText("Atk↑");
        ui->enemybuff1N->setText(QString::number(enemyPokemon->state.buff.upAtk));
    }

    if (enemyPokemon->state.buff.downAtk > 0){
       ui->enemybuff2->setText("Atk↓");
       ui->enemybuff2N->setText(QString::number(enemyPokemon->state.buff.downAtk));
    }

    if (enemyPokemon->state.buff.downDef > 0){
       ui->enemybuff2->setText("Def↓");
       ui->enemybuff2N->setText(QString::number(enemyPokemon->state.buff.downDef));
    }

    if (enemyPokemon->state.buff.downAgl > 0){
       ui->enemybuff2->setText("Agl↓");
       ui->enemybuff2N->setText(QString::number(enemyPokemon->state.buff.downAgl));
    }

    if (enemyPokemon->state.buff.onFire > 0){
       ui->enemybuff2->setText("Fire");
       ui->enemybuff2N->setText(QString::number(enemyPokemon->state.buff.onFire));
    }
}

void Battle::attackAnimation(int kind)
{
    myTimer->stop();
    PAUSE = 1;

    damageLabel = new QLabel(this);
    damageLabel->setFont(QFont("Arial", 15, QFont::Bold));

    damageLabel->setText("-" + QString::number(damage));

    valueAnimation = new QPropertyAnimation(damageLabel, "pos");

    QGraphicsOpacityEffect *pLabelOpacity = new QGraphicsOpacityEffect(this);
    pLabelOpacity->setOpacity(1);


    QSequentialAnimationGroup *pPosGroup = new QSequentialAnimationGroup(this);

    if (kind == 0)  //己方宝可梦攻击敌方宝可梦
    {
        /*技能展示*/
        if (minePokemon->state.skill != "")
        {
            ui->skill1->setText(minePokemon->state.skill);
            showSkillAnimation = new QPropertyAnimation(pLabelOpacity, "opacity");

            ui->skill1->show();
            ui->skill1->setStyleSheet("color:" + KIND_COLOR[minePokemon->state.type]);
            ui->back1->show();

            ui->skill1->setGraphicsEffect(pLabelOpacity);
            ui->back1->setGraphicsEffect(pLabelOpacity);

            showSkillAnimation->setDuration(800 / SPEED_TIME);
            showSkillAnimation->setStartValue(0);
            showSkillAnimation->setEndValue(1);

            pPosGroup->addAnimation(showSkillAnimation);

            connect(showSkillAnimation, SIGNAL(finished()), this, SLOT(skillOver()) );
        }


        /*阶段1*/
        pPosAnimation1 = new QPropertyAnimation(ui->picA , "pos");
        pPosAnimation1->setDuration(500  / SPEED_TIME);
        pPosAnimation1->setStartValue(QPoint(recA.x(), recA.y()));
        pPosAnimation1->setEndValue(QPoint(recB.x() - recB.width() / 4.0, recB.y()));
        pPosAnimation1->setEasingCurve(QEasingCurve::InOutQuad);

        /*阶段2*/
        pPosAnimation2 = new QPropertyAnimation(ui->picA , "pos");
        pPosAnimation2->setDuration(500  / SPEED_TIME);
        pPosAnimation2->setStartValue(QPoint(recB.x() - recB.width() / 4.0, recB.y()));
        pPosAnimation2->setEndValue(QPoint(recA.x(), recA.y()));
        pPosAnimation2->setEasingCurve(QEasingCurve::InOutQuad);

        QObject::connect(pPosAnimation1, SIGNAL(finished()), this, SLOT(showDamage1()));
    }

    if (kind == 1)  //敌方宝可梦攻击己方宝可梦
    {

        /*技能展示*/
        if (enemyPokemon->state.skill != "")
        {
            ui->skill2->setText(enemyPokemon->state.skill);
            showSkillAnimation = new QPropertyAnimation(pLabelOpacity, "opacity");

            ui->skill2->show();
            ui->skill2->setStyleSheet("color:" + KIND_COLOR[enemyPokemon->state.type]);
            ui->back2->show();

            ui->skill2->setGraphicsEffect(pLabelOpacity);
            ui->back2->setGraphicsEffect(pLabelOpacity);

            showSkillAnimation->setDuration(800 / SPEED_TIME);
            showSkillAnimation->setStartValue(0);
            showSkillAnimation->setEndValue(1);

            pPosGroup->addAnimation(showSkillAnimation);

            connect(showSkillAnimation, SIGNAL(finished()), this, SLOT(skillOver()) );
        }

        /*阶段1*/
        pPosAnimation1 = new QPropertyAnimation(ui->picB , "pos");
        pPosAnimation1->setDuration(500  / SPEED_TIME);
        pPosAnimation1->setStartValue(QPoint(recB.x(), recB.y()));
        pPosAnimation1->setEndValue(QPoint(recA.x() + recA.width() / 4.0, recA.y()));
        pPosAnimation1->setEasingCurve(QEasingCurve::InOutQuad);

        valueAnimation->setDuration(300 / SPEED_TIME);
        valueAnimation->setStartValue(QPoint(recB.x() , recB.y() + recB.height()/ 2.0));
        valueAnimation->setEndValue(QPoint(recB.x(), recB.y() - recB.height() / 2.0));
        valueAnimation->setEasingCurve(QEasingCurve::InOutQuad);

        /*阶段2*/
        pPosAnimation2 = new QPropertyAnimation(ui->picB , "pos");
        pPosAnimation2->setDuration(500 / SPEED_TIME);
        pPosAnimation2->setStartValue(QPoint(recA.x() + recA.width() / 4.0, recA.y()));
        pPosAnimation2->setEndValue(QPoint(recB.x(), recB.y()));
        pPosAnimation2->setEasingCurve(QEasingCurve::InOutQuad);

        QObject::connect(pPosAnimation1, SIGNAL(finished()), this, SLOT(showDamage2()));
    }
    pPosGroup->addAnimation(pPosAnimation1);
    pPosGroup->addAnimation(pPosAnimation2);

    pPosGroup->start();


    QObject::connect(pPosGroup, SIGNAL(finished()), this, SLOT(restartTimer()));
}

void Battle::skillOver()
{
    ui->skill1->hide();
    ui->skill2->hide();
    ui->back1->hide();
    ui->back2->hide();
}

void Battle::process()
{
//    qDebug() << nowTimeA << nowTimeB;
    if (PAUSE == 0)
    {
        nowTimeA += 1;


        if (minePokemon->judgeAttack(nowTimeA)){
            damage = minePokemon->Attack(enemyPokemon);
            nowTimeA = 0;

            attackAnimation(0);
        }
        if(death == 2)  //赢
        {
            myTimer->stop();
            resultWindow = new BattleResult(username, minePokemon, enemyPokemon, mode, 1, difficulty, this);
            connect(resultWindow, SIGNAL(resultWindowOver()), this, SLOT(receiveOver()));
            qDebug() << "准备运行resultWindow";
            resultWindow->show();

        }

        if (PAUSE == 1) return;

        nowTimeB += 1;
        if (enemyPokemon->judgeAttack(nowTimeB)){
            damage = enemyPokemon->Attack(minePokemon);
            nowTimeB = 0;

            attackAnimation(1);
        }
        if(death == 1)  //  输
        {
            myTimer->stop();
            resultWindow = new BattleResult(username, minePokemon, enemyPokemon, mode, 0, difficulty, this);
            connect(resultWindow, SIGNAL(resultWindowOver()), this, SLOT(receiveOver()));
            qDebug() << "准备运行resultWindow";
            resultWindow->show();

        }
    }
}

void Battle::receiveOver()
{
    //结束 返回
    this->hide();
}

void Battle::on_oneSpeed_clicked()
{
    SPEED_TIME = 1;
}

void Battle::on_twoSpeed_clicked()
{
    SPEED_TIME = 2;
}
