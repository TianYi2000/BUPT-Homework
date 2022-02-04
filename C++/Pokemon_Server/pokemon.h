#ifndef POKEMON_H
#define POKEMON_H

#include <QString>
#include <QJsonObject>

class MainWindow;

enum Category   //精灵种类
{
    HighATK,    //力量型：高攻击
    HighHP,     //肉盾型：高生命值
    HighDEF,    //防御型：高防御
    LowDelay    //敏捷型：低攻击间隔
};

enum Attribute  //元素属性
{
    NormalT,     //一般属性
    FireT,       //火属性
    WaterT,      //水属性
    GrassT,      //草属性
    ElectricT    //电属性
};

enum Buff      //精灵状态(buff/debuff)
{
    //待添加
};

struct BattleState    //精灵战斗时状态
{
    //待添加
};

struct Skill    //精灵技能
{
    Attribute type;     //技能属性
    Buff state;        //技能附带状态
    double times;       //技能威力倍率
};

//int LEVEL_EXP_LIMIT[15] = {0,4,13,32,65,112,178,276,393,540,745,967,1230,1591,1957};    //每一等级升级时对应的累积经验值
const int MAX_LEVEL = 15;   //最大等级
const double CRITICAL_PROBABILITY = 0.2;    //暴击概率
const double DODGE_PROBABILITY = 0.2;       //闪避概率


class Pokemon
{
public:
    Pokemon(QString, Category, Attribute, int, int, int, int, QString, QString);
    virtual int Attack (Pokemon *) = 0;
    bool TestLevelUp ();
    QJsonObject toJson();
    QString toSql(QString);
    virtual ~Pokemon();
protected:
    int id;         //精灵ID
    QString name;   //名字
    Category kind;  //种类
    Attribute type; //精灵元素属性
    BattleState state;    //精灵状态
    int level;      //等级
    int exp;        //经验值
    int atk;        //攻击力
    int def;        //防御力
    int maxhp;      //最大生命值
    int agl;        //敏捷
    int delay;      //攻击间隔
    QString skill;  //技能
    QString engname;//英文名称
    static int totalNum;    //精灵总数量
};

class Normal : public Pokemon
{
public:
    Normal(QString Name, Category Kind, int Atk, int Def, int Maxhp, int Agl, QString Skill, QString Engname)
        : Pokemon(Name, Kind, NormalT, Atk, Def, Maxhp, Agl, Skill, Engname) {};
    int Attack (Pokemon *);
};

class Fire : public Pokemon
{
public:
    Fire(QString Name, Category Kind, int Atk, int Def, int Maxhp, int Agl, QString Skill, QString Engname)
        : Pokemon(Name, Kind, FireT, Atk, Def, Maxhp, Agl, Skill, Engname) {};
    int Attack (Pokemon *);
};

class Water : public Pokemon
{
public:
    Water(QString Name, Category Kind, int Atk, int Def, int Maxhp, int Agl, QString Skill, QString Engname)
        : Pokemon(Name, Kind, WaterT, Atk, Def, Maxhp, Agl, Skill, Engname) {};
    int Attack (Pokemon *);
};

class Grass : public Pokemon
{
public:
    Grass(QString Name, Category Kind, int Atk, int Def, int Maxhp, int Agl, QString Skill, QString Engname)
        : Pokemon(Name, Kind, GrassT, Atk, Def, Maxhp, Agl, Skill, Engname) {};
    int Attack (Pokemon *);
};

class Electric : public Pokemon
{
public:
    Electric(QString Name, Category Kind, int Atk, int Def, int Maxhp, int Agl, QString Skill, QString Engname)
        : Pokemon(Name, Kind, ElectricT, Atk, Def, Maxhp, Agl, Skill, Engname) {};
    int Attack (Pokemon *);
};

#endif // POKEMON_H
