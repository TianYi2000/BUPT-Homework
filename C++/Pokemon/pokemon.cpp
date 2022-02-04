#include "pokemon.h"

#include <QTime>
#include <QDebug>
#include <QImage>
#include <QtGlobal>
#include <QJsonValue>
#include <QByteArray>

#define gmax(_a, _b) ((_a) > (_b) ? (_a) : (_b))
#define gmin(_a, _b) ((_a) < (_b) ? (_a) : (_b))

const double attributeRestraint[5][5] =
{
    {1, 1, 1, 1, 1},
    {1, 0.5, 0.5, 2, 1},
    {1, 2, 0.5, 0.5, 1},
    {1, 0.5, 2, 0.5, 1},
    {1, 1, 2, 0.5, 0.5}
};                          //元素属性克制表，表示伤害倍率

const int AglInf = 300;     //敏捷值上限常量,用于计算攻击间隔,攻击间隔 = AglInf - 敏捷值
int Pokemon::totalNum = 0;

/*
 * 创建初始宝可梦（不指定经验和等级，默认等级为1）
 * 输入：除经验和等级外的各项精灵参数
 * 输出：无
*/
Pokemon::Pokemon(QString Name, Category Kind, Attribute Type, int Atk, int Def, int Maxhp, int Agl, QString Skill, QString Engname)
{
    id = Pokemon::totalNum++;
    name = Name;
    kind = Kind;
    type = Type;
    level = 1;      //初始等级为1级
    exp = 0;        //初始经验值为0
    atk = Atk;
    def = Def;
    maxhp = Maxhp;
    agl = Agl;
    delay = AglInf - agl;   //攻击间隔 = AGlINF - 敏捷值
    skill = Skill;
    engname = Engname;

}

/*
 * 创建任意宝可梦（指定经验和等级）
 * 输入：各项精灵参数
 * 输出：无
*/
Pokemon::Pokemon(int Id, QString Name, Category Kind, Attribute Type, int Level, int Exp, int Atk, int Def, int Maxhp, int Agl, QString Skill, QString Engname)
{
    id = Id;
    name = Name;
    kind = Kind;
    type = Type;
    level = Level;
    exp = Exp;
    atk = Atk;
    def = Def;
    maxhp = Maxhp;
    agl = Agl;
    delay = AglInf - agl;   //攻击间隔 = 200 - 敏捷值
    skill = Skill;
    engname = Engname;


    qDebug() << atk << def << maxhp << agl;
}

/*
 * 战斗胜利后增加小精灵的经验
 * 输入：玩家挑战的难度等级
 * 输出：无
 */
void Pokemon::getExp(int difficulty)
{
    exp += DIFFICULTY_EXP[difficulty];      //将难度值转换为可获得的经验值
    TestLevelUp();                          //小精灵获得经验后，应当判断能否升级
}

/*
 * 判断小精灵当前的经验值是否可以升级，如果是，则增加等级并根据小精灵的种类提高属性值
 * 输入：无
 * 输出：无
 */
void Pokemon::TestLevelUp()
{
    if (exp >= LEVEL_EXP_LIMIT[level] && level < 15)    //如果当前经验到达了当前等级经验上限，且未满级，则可以进行升级
    {
        level ++;
        /*
         * 属性提升
         * 不同种类精灵主属性大量提升，副属性少量提升
         */
        if (kind == HighATK){       //高攻击种类，主属性为攻击
            atk += 10;
            def += 5;
            maxhp += 5;
            agl += 5;
            delay = AglInf - agl;
        }

        if (kind == HighDEF){       //高防御种类，主属性为防御
            atk += 5;
            def += 10;
            maxhp += 5;
            agl += 5;
            delay = AglInf - agl;
        }

        if (kind == HighHP){        //高血量种类，主属性为最大血量
            atk += 5;
            def += 5;
            maxhp += 10;
            agl += 5;
            delay = AglInf - agl;
        }

        if (kind == LowDelay){      //低攻击间隔种类，主属性为敏捷
            atk += 5;
            def += 5;
            maxhp += 5;
            agl += 10;
            delay = AglInf - agl;
        }
    }
}

/*
 * 将小精灵的各项指数转换成一个Json对象，方便socket传输与读取
 * 输入：无
 * 输出：小精灵Json对象
 */
QJsonObject Pokemon::toJson()
{
    QJsonObject pokemonInfo;
    pokemonInfo["id"] = id;
    pokemonInfo["name"] = name;
    pokemonInfo["kind"] = kind;
    pokemonInfo["type"] = type;
    pokemonInfo["level"] = level;
    pokemonInfo["exp"] = exp;
    pokemonInfo["atk"] = atk;
    pokemonInfo["def"] = def;
    pokemonInfo["maxhp"] = maxhp;
    pokemonInfo["agl"] = agl;
    pokemonInfo["delay"] = delay;
    return pokemonInfo;
}

/*
 * 将小精灵的各项指数转换成一个Sql语句，方便存入数据库
 * 输入：玩家用户名（将其与小精灵信息共同存入数据库，方便判断小精灵归属）
 * 输出：Sql语句
 */
QString Pokemon::toSql(QString username)
{
    QString Sql;
    Sql = " VALUES (";
    Sql += "NULL,";
    Sql += "'" + username + "',";
    Sql += "'" + name + "',";
    Sql += QString::number(atk) + ",";
    Sql += QString::number(maxhp) + ",";
    Sql += QString::number(def) + ",";
    Sql += QString::number(agl) + ",";
    Sql += QString::number(type) + ",";
    Sql += QString::number(kind) + ",";
    Sql += "'" + skill + "',";
    Sql += QString::number(exp) + ",";
    Sql += QString::number(level) + ",";
    Sql += "'" + engname + "')";
    qDebug() << "Pokemon to Sql is" << Sql;
    return Sql;
}

/*
 * 从种类序号值向种类描述字符串的转换，从类内部存储信息转换为交互界面上打印的信息
 * 输入：种类序号
 * 输出：种类描述
 */
QString Pokemon::showCategory(int kind)
{
    QString category;
    switch (kind){
    case 0:
        category = "力量型";
        break;
    case 1:
        category = "肉盾型";
        break;
    case 2:
        category = "防御型";
        break;
    case 3:
        category = "敏捷型";
        break;
    }

    return category;

}

/*
 * 从属性序号值向种类描述字符串的转换，从类内部存储信息转换为交互界面上打印的信息
 * 输入：属性序号
 * 输出：属性描述
 */
QString Pokemon::showAttribute(int type)
{
    QString attribute;
    switch (type){
    case 0:
        attribute = "一般属性";
        break;
    case 1:
        attribute = "火属性";
        break;
    case 2:
        attribute = "水属性";
        break;
    case 3:
        attribute = "草属性";
        break;
    case 4:
        attribute = "电属性";
        break;
    }
    return attribute;
}

/*
 * 判断小精灵当前是否完成下一次攻击前的等待
 * 输入：小精灵距离上次攻击已经等待的时间
 * 输出：小精灵在当前时刻是否可以发动攻击
 */
bool Pokemon::judgeAttack(int nowTime)
{
//    qDebug() << nowTime << delay;
    if (nowTime >= state.delay + state.buff.downAgl > 0 ? 10 : 0)           //小精灵距离上次攻击已经等待的时间超过攻击间隔时，可以攻击
        return true;
    else return false;
}

/*
 * 返回小精灵的名字
 */
QString Pokemon::returnName()
{
    return name;
}

/*
 * 返回小精灵的图片
 * 通过小精灵的英文名生成对应图片资源的路径并访问
 * 输入：是否进行镜像（为了让对战时左侧与右侧的宝可梦能够相向而视）
 * 输出：小精灵图片QPixmap
 */
QPixmap Pokemon::returnPic(int mirror)
{
    QImage img;
    img.load(":/icon/pokemons/" + engname + ".png");
    if (mirror)
        img = img.mirrored(true, false);

    return QPixmap::fromImage(img);
}

/*
 * 战斗前对小精灵的初始化
 */
void Pokemon::stateInitialization()
{
    state.hp = maxhp;           //战斗前小精灵的血量恢复为满
    state.maxhp = maxhp;
    state.atk = atk;
    state.def = def;
    state.type = type;
    state.delay = delay;
    state.buff.upAtk = 0;
    state.buff.onFire = 0;
    state.buff.downAgl = 0;
    state.buff.downAtk = 0;
    state.buff.downDef = 0;
}


/*
 * 普通属性小精灵的攻击函数
 * 输入：敌方小精灵的引用
 * 输出：无
 */
int Normal::Attack(Pokemon *rhs)
{
    int damage = 0.25 * (atk + (state.buff.upAtk > 0 ? 10 : 0) ) * (atk + (state.buff.upAtk > 0 ? 10 : 0)) / rhs->state.def * attributeRestraint[int(type)][int(rhs->state.type)] + 5;
    //伤害计算公式为0.2*力量*（攻击者的攻击力/防守者的防御力）*属性克制 + 1
    if (damage <= 0) damage = 1;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));      //用当前初始化随机函数内核

    int pskill = qrand()%10;
    if (pskill < qrand() % 10)
    {
        state.skill = skill;
        damage = damage * 1.2;
        state.buff.upAtk = 3;
    }
    else
    {
        state.skill = "";
        state.buff.upAtk = gmax(state.buff.upAtk - 1, 0);
    }


    int dodge = qrand()%10;                 //随机一个0-9的整数

    if (dodge < DODGE_PROBABILITY)          //随机结果小于闪避额定值，则此次攻击被闪避
    {
        state.isDodge = 1;                  //在战斗状态中标记此次闪避结果
        damage = 0;                         //因为被闪避，因此伤害为0
    }
    else
    {
        if (damage <= 0) damage = 1;        //如果没有被闪避，则伤害值最小为1
        state.isDodge = 0;                  //在战斗状态中标记此次未被闪避

        int critical = qrand()%10;          //随机一个0-9的整数
        if (critical < CRITICAL_PROBABILITY)//如果随机结果小于暴击额定值，则此次攻击暴击
        {
            state.isCritical = 1;           //在战斗状态中标记此次暴击
            damage *= 1.2;                    //最终伤害翻倍
        }
        else{
            state.isCritical = 0;
        }
    }

    rhs->state.hp = gmax(rhs->state.hp - damage, 0);    //如果敌方宝可梦收到伤害后血量为负，则将其置为0，此次战斗结束
    return damage;
}

/*
 * 火属性小精灵的攻击函数
 * 输入：敌方小精灵的引用
 * 输出：无
 */
int Fire::Attack(Pokemon *rhs)
{
    int damage = 0.25 * atk * atk / rhs->state.def * attributeRestraint[int(type)][int(rhs->state.type)] + 5
            + rhs->state.buff.onFire > 0 ? 10 : 0;
    //伤害计算公式为0.2*力量*（攻击者的攻击力/防守者的防御力）*属性克制 + 1
    if (damage <= 0) damage = 1;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));      //用当前初始化随机函数内核

    int pskill = qrand()%10;
    if (pskill < qrand() % 10)
    {
        state.skill = skill;
        damage = damage * 1.2;
        rhs->state.buff.onFire = 3;
    }
    else
    {
        state.skill = "";
        rhs->state.buff.onFire = gmax (0, rhs->state.buff.onFire - 1);
    }
    int dodge = qrand()%10;                 //随机一个0-9的整数

    if (dodge < DODGE_PROBABILITY)          //随机结果小于闪避额定值，则此次攻击被闪避
    {
        state.isDodge = 1;                  //在战斗状态中标记此次闪避结果
        damage = 0;                         //因为被闪避，因此伤害为0
    }
    else
    {
        if (damage <= 0) damage = 1;        //如果没有被闪避，则伤害值最小为1
        state.isDodge = 0;                  //在战斗状态中标记此次未被闪避

        int critical = qrand()%10;          //随机一个0-9的整数
        if (critical < CRITICAL_PROBABILITY)//如果随机结果小于暴击额定值，则此次攻击暴击
        {
            state.isCritical = 1;           //在战斗状态中标记此次暴击
            damage *= 1.2;                    //最终伤害翻倍
        }
        else{
            state.isCritical = 0;
        }
    }
    rhs->state.hp = gmax(rhs->state.hp - damage, 0);    //如果敌方宝可梦收到伤害后血量为负，则将其置为0，此次战斗结束
    return damage;
}

/*
 * 水属性小精灵的攻击函数
 * 输入：敌方小精灵的引用
 * 输出：无
 */
int Water::Attack(Pokemon *rhs)
{
    int damage = 0.25 * atk * atk / (rhs->state.def - (rhs->state.buff.downDef > 0 ? 10 : 0))
            * attributeRestraint[int(type)][int(rhs->state.type)] + 5;
    qDebug() << "damage = " << damage;
    //伤害计算公式为0.2*力量*（攻击者的攻击力/防守者的防御力）*属性克制 + 1
    if (damage <= 0) damage = 1;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));      //用当前初始化随机函数内核

    int pskill = qrand()%10;
    if (pskill < qrand() % 10)
    {
        state.skill = skill;
        damage = damage * 1.2;
        rhs->state.buff.downDef = 3;
    }
    else
    {
        state.skill = "";
        rhs->state.buff.downDef = gmax (0, rhs->state.buff.downDef - 1);
    }


    int dodge = qrand()%10;                 //随机一个0-9的整数

    if (dodge < DODGE_PROBABILITY)          //随机结果小于闪避额定值，则此次攻击被闪避
    {
        state.isDodge = 1;                  //在战斗状态中标记此次闪避结果
        damage = 0;                         //因为被闪避，因此伤害为0
    }
    else
    {
        if (damage <= 0) damage = 1;        //如果没有被闪避，则伤害值最小为1
        state.isDodge = 0;                  //在战斗状态中标记此次未被闪避

        int critical = qrand()%10;          //随机一个0-9的整数
        if (critical < CRITICAL_PROBABILITY)//如果随机结果小于暴击额定值，则此次攻击暴击
        {
            state.isCritical = 1;           //在战斗状态中标记此次暴击
            damage *= 1.2;                    //最终伤害翻倍
        }
        else{
            state.isCritical = 0;
        }
    }

    rhs->state.hp = gmax(rhs->state.hp - damage, 0);    //如果敌方宝可梦收到伤害后血量为负，则将其置为0，此次战斗结束
    return damage;
}

/*
 * 草属性小精灵的攻击函数
 * 输入：敌方小精灵的引用
 * 输出：无
 */
int Grass::Attack(Pokemon *rhs)
{
    int damage = 0.25 * (atk - state.buff.downAtk > 0 ? 10 : 0) * (atk - state.buff.downAtk > 0 ? 10 : 0)
            / rhs->state.def * attributeRestraint[int(type)][int(rhs->state.type)] + 5;
    //伤害计算公式为0.2*力量*（攻击者的攻击力/防守者的防御力）*属性克制 + 1
    if (damage <= 0) damage = 1;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));      //用当前初始化随机函数内核

    int pskill = qrand()%10;
    if (pskill < qrand() % 10)
    {
        state.skill = skill;
        damage = damage * 1.2;
        rhs->state.buff.downAtk = 3;
    }
    else
    {
        state.skill = "";
        rhs->state.buff.downAtk = gmax (0, rhs->state.buff.downAtk - 1);
    }


    int dodge = qrand()%10;                 //随机一个0-9的整数

    if (dodge < DODGE_PROBABILITY)          //随机结果小于闪避额定值，则此次攻击被闪避
    {
        state.isDodge = 1;                  //在战斗状态中标记此次闪避结果
        damage = 0;                         //因为被闪避，因此伤害为0
    }
    else
    {
        if (damage <= 0) damage = 1;        //如果没有被闪避，则伤害值最小为1
        state.isDodge = 0;                  //在战斗状态中标记此次未被闪避

        int critical = qrand()%10;          //随机一个0-9的整数
        if (critical < CRITICAL_PROBABILITY)//如果随机结果小于暴击额定值，则此次攻击暴击
        {
            state.isCritical = 1;           //在战斗状态中标记此次暴击
            damage *= 1.2;                    //最终伤害翻倍
        }
        else{
            state.isCritical = 0;
        }
    }

    rhs->state.hp = gmax(rhs->state.hp - damage, 0);    //如果敌方宝可梦收到伤害后血量为负，则将其置为0，此次战斗结束
    return damage;
}

/*
 * 电属性小精灵的攻击函数
 * 输入：敌方小精灵的引用
 * 输出：无
 */
int Electric::Attack(Pokemon *rhs)
{
    int damage = 0.25 * atk * atk / rhs->state.def * attributeRestraint[int(type)][int(rhs->state.type)] + 5;
    //伤害计算公式为0.2*力量*（攻击者的攻击力/防守者的防御力）*属性克制 + 1
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));      //用当前初始化随机函数内核

    int pskill = qrand()%10;
    if (pskill < qrand() % 10)
    {
        state.skill = skill;
        damage = damage * 1.2;
        rhs->state.buff.downAgl = 3;
    }
    else
    {
        state.skill = "";
        rhs->state.buff.downAgl = gmax(0, rhs->state.buff.downAgl - 1);
    }

    int dodge = qrand()%10;                 //随机一个0-9的整数

    if (dodge < DODGE_PROBABILITY)          //随机结果小于闪避额定值，则此次攻击被闪避
    {
        state.isDodge = 1;                  //在战斗状态中标记此次闪避结果
        damage = 0;                         //因为被闪避，因此伤害为0
    }
    else
    {
        if (damage <= 0) damage = 1;        //如果没有被闪避，则伤害值最小为1
        state.isDodge = 0;                  //在战斗状态中标记此次未被闪避

        int critical = qrand()%10;          //随机一个0-9的整数
        if (critical < CRITICAL_PROBABILITY)//如果随机结果小于暴击额定值，则此次攻击暴击
        {
            state.isCritical = 1;           //在战斗状态中标记此次暴击
            damage *= 1.2;                    //最终伤害翻倍
        }
        else{
            state.isCritical = 0;
        }
    }

    rhs->state.hp = gmax(rhs->state.hp - damage, 0);    //如果敌方宝可梦收到伤害后血量为负，则将其置为0，此次战斗结束
    return damage;
}

Pokemon::~Pokemon(){};

