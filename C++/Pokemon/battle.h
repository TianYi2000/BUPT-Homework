#ifndef BATTLE_H
#define BATTLE_H

#include "pokemon.h"
#include "battleresult.h"

#include <QRect>
#include <QTimer>
#include <QLabel>
#include <QWidget>
#include <QPropertyAnimation>

namespace Ui {
class Battle;
}

class Battle : public QWidget
{
    Q_OBJECT

public:
    explicit Battle(int, QString, Pokemon* , Pokemon* ,int,  QWidget *parent = nullptr);
    ~Battle();
    void UIInit();		//UI初始化
    void BattleInit();//战斗状态初始化
    void TimerInit();//计时器初始化
    void AnimationInit();//动画初始


private slots:
    void process();			//计时器槽函数
    void restartTimer();	//动画结束后重新启动定时器
    void showDamage1();		//展示我方宝可梦攻击造成的伤害
    void showDamage2();		//展示敌方宝可梦攻击造成的伤害
    void deleteValue();		//删除伤害标签
    void receiveOver();		//战斗结果演示结束
    void skillOver();		//技能展示函数
    void on_oneSpeed_clicked();	//按下一倍速按钮
    void on_twoSpeed_clicked();	//按下二倍速按钮

private:
    Ui::Battle *ui;
    QString username;
    Pokemon *minePokemon, *enemyPokemon;
    QTimer* myTimer;
    QRect recA, recB;
    QLabel* damageLabel;
    int nowTimeA, nowTimeB, PAUSE, damage, death, mode, difficulty;
    QPropertyAnimation *showSkillAnimation, *pPosAnimation1, *pPosAnimation2, *valueAnimation;
    BattleResult* resultWindow;

    void attackAnimation(int);	//攻击动画展示
    void upDateBuff();			//更新Buff显示

};

#endif // BATTLE_H
