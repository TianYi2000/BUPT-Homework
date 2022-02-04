#ifndef BATTLERESULT_H
#define BATTLERESULT_H

#include "pokemon.h"

#include <QLabel>
#include <QWidget>
#include <QLayout>
#include <QVariant>
#include <QUdpSocket>
#include <QPushButton>
#include <QJsonObject>
#include <QButtonGroup>
#include <QPropertyAnimation>

namespace Ui {
class BattleResult;
}

class BattleResult : public QWidget
{
    Q_OBJECT

public:
    explicit BattleResult(QString, Pokemon*, Pokemon*, int, int, int, QWidget *parent = nullptr);
    ~BattleResult();

    void UIInit();
    void UdpInit();

signals:
    void resultWindowOver();

private slots:
    void showResultOver();
    void endResult();
    void receiveData();
    void showDetails();
    void dropPokemon();

private:
    Ui::BattleResult *ui;
    QUdpSocket *clientudp;
    int port;
    Pokemon* minePokemon, *enemyPokemon;
    QString username;
    QLabel* resultLabel;
    QJsonObject recInfo;
    QButtonGroup* btnGroup;
    QJsonObject selectedPokemon;
    bool pokemonAllOut;

//    QPropertyAnimation *pResultAnimation1, *pResultAnimation2;
    int mode, result, difficulty, selectedId;


    void showResult();      //Stage 1
    void showExp();         //Stage 2
    void showChoose();     //Stage 3

    void sendRequest(QString);
    void analysisByteArray(QByteArray recData);
    void setButtonBackImage(QPushButton *button,QString image,int sizeW, int sizeH);
};

#endif // BATTLERESULT_H
