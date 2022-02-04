#ifndef POKEMON_H
#define POKEMON_H

#include <QString>
#include <QPixmap>
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

struct Buff      //精灵状态(buff/debuff)
{
    int upAtk;  //Normal小精灵攻击附加效果
    int onFire; //Fire小精灵攻击附加效果
    int downDef;//Water小精灵攻击附加效果
    int downAtk;//Grass小精灵攻击附加效果
    int downAgl;//Electirc小精灵攻击附加效果
};

struct BattleState    //精灵战斗时状态
{
    int maxhp;
    int hp;
    int atk;
    int def;
    int delay;
    int isCritical;
    int isDodge;
    Attribute type;
    QString skill;
    Buff buff;
};

const int LEVEL_EXP_LIMIT[15] = {0,4,13,32,65,112,178,276,393,540,745,967,1230,1591,1957};    //每一等级升级时对应的累积经验值
const int DIFFICULTY_EXP[4] = {10, 50, 100, 500};                 //挑战不同难度关卡胜利时获得的经验值
const int MAX_LEVEL = 15;   //最大等级
const int CRITICAL_PROBABILITY = 2;    //暴击概率为20%
const int DODGE_PROBABILITY = 2;       //闪避概率20%
const int SKILL_PROBABILITY = 2;


class Pokemon
{
public:
    Pokemon(QString, Category, Attribute, int, int, int, int, QString, QString);            //创建初始宝可梦（不指定经验和等级，默认等级为1）
    Pokemon(int, QString, Category, Attribute, int, int, int, int, int, int, QString, QString);  //创建任意宝可梦（指定经验和等级）
    virtual int Attack (Pokemon *) = 0;     //攻击（虚函数，由子类重写）
    QJsonObject toJson();                   //将小精灵的各项指数转换成一个Json对象，方便socket传输与读取
    QString toSql(QString);                 //将小精灵的各项指数转换成一串Sql语法，方面存入数据库
    static QString showCategory(int);       //从种类序号值向种类描述字符串的转换
    static QString showAttribute(int);      //从数据序号值向属性描述字符串的转换
    bool judgeAttack (int);                 //判断小精灵当前是否完成下一次攻击前的等待
    QString returnName();                   //返回小精灵的名字字符串
    QPixmap returnPic(int);                 //返回小精灵的图片
    void stateInitialization();             //战斗前对小精灵的初始化，包括恢复血量为满等等
    void getExp(int);                       //战斗胜利后增加小精灵的经验
    void TestLevelUp ();                    //判断小精灵当前的经验值是否可以升级
    ~Pokemon();                             //小精灵析构函数

    BattleState state;                      //小精灵的状态，在模拟战斗时实时访问与更新

protected:
    int id;                 //精灵ID
    QString name;           //中文名字
    Category kind;          //种类
    Attribute type;         //精灵元素属性

    int level;              //等级
    int exp;                //经验值
    int atk;                //攻击力
    int def;                //防御力
    int maxhp;              //最大生命值
    int agl;                //敏捷
    int delay;              //攻击间隔
    QString skill;          //技能
    QString engname;        //英文名称（用于访问小精灵的图片资源）
    static int totalNum;    //精灵总数量
    QJsonObject pokemonJson;
};

/*普通属性精灵子类*/
class Normal : public Pokemon
{
public:
    Normal(QString Name, Category Kind, int Atk, int Def, int Maxhp, int Agl, QString Skill, QString Engname)
        : Pokemon(Name, Kind, NormalT, Atk, Def, Maxhp, Agl, Skill, Engname) {};            //创建普通属性初始宝可梦（等级为1）
    Normal(int id, QString Name, Category Kind, int Level, int Exp,int Atk, int Def, int Maxhp, int Agl, QString Skill, QString Engname)
        : Pokemon(id, Name, Kind, NormalT, Level, Exp, Atk, Def, Maxhp, Agl, Skill, Engname) {};//创建普通属性任意宝可梦
    int Attack (Pokemon *); //重写攻击方法
};

/*火属性精灵子类*/
class Fire : public Pokemon
{
public:
    Fire(QString Name, Category Kind, int Atk, int Def, int Maxhp, int Agl, QString Skill, QString Engname)
        : Pokemon(Name, Kind, FireT, Atk, Def, Maxhp, Agl, Skill, Engname) {};              //创建火属性初始宝可梦（等级为1）
    Fire(int id, QString Name, Category Kind, int Level, int Exp,int Atk, int Def, int Maxhp, int Agl, QString Skill, QString Engname)
        : Pokemon(id, Name, Kind, FireT, Level, Exp, Atk, Def, Maxhp, Agl, Skill, Engname) {};  //创建火属性任意宝可梦
    int Attack (Pokemon *); //重写攻击方法
};

/*水属性精灵子类*/
class Water : public Pokemon
{
public:
    Water(QString Name, Category Kind, int Atk, int Def, int Maxhp, int Agl, QString Skill, QString Engname)
        : Pokemon(Name, Kind, WaterT, Atk, Def, Maxhp, Agl, Skill, Engname) {};              //创建水属性初始宝可梦（等级为1）
    Water(int id, QString Name, Category Kind, int Level, int Exp,int Atk, int Def, int Maxhp, int Agl, QString Skill, QString Engname)
        : Pokemon(id, Name, Kind, WaterT, Level, Exp, Atk, Def, Maxhp, Agl, Skill, Engname) {};  //创建水属性任意宝可梦
    int Attack (Pokemon *); //重写攻击方法
};

/*草属性精灵子类*/
class Grass : public Pokemon
{
public:
    Grass(QString Name, Category Kind, int Atk, int Def, int Maxhp, int Agl, QString Skill, QString Engname)
        : Pokemon(Name, Kind, GrassT, Atk, Def, Maxhp, Agl, Skill, Engname) {};             //创建草属性初始宝可梦（等级为1）
    Grass(int id, QString Name, Category Kind, int Level, int Exp,int Atk, int Def, int Maxhp, int Agl, QString Skill, QString Engname)
        : Pokemon(id, Name, Kind, GrassT, Level, Exp, Atk, Def, Maxhp, Agl, Skill, Engname) {};//创建草属性任意宝可梦
    int Attack (Pokemon *); //重写攻击方法
};

/*电属性精灵子类*/
class Electric : public Pokemon
{
public:
    Electric(QString Name, Category Kind, int Atk, int Def, int Maxhp, int Agl, QString Skill, QString Engname)
        : Pokemon(Name, Kind, ElectricT, Atk, Def, Maxhp, Agl, Skill, Engname) {};              //创建电属性初始宝可梦（等级为1）
    Electric(int id, QString Name, Category Kind, int Level, int Exp,int Atk, int Def, int Maxhp, int Agl, QString Skill, QString Engname)
        : Pokemon(id, Name, Kind, ElectricT, Level, Exp, Atk, Def, Maxhp, Agl, Skill, Engname) {};  //创建电属性任意宝可梦
    int Attack (Pokemon *); //重写攻击方法
};

#endif // POKEMON_H
