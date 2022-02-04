#include "pokemon.h"

#include <QDebug>
#include <QByteArray>


const int AglInf = 200;     //最大敏捷值,用于计算攻击间隔,攻击间隔 = 200 - 敏捷值
int Pokemon::totalNum = 0;

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
    delay = AglInf - agl;   //攻击间隔 = 200 - 敏捷值
    skill = Skill;
    engname = Engname;
}

bool Pokemon::TestLevelUp()
{
//    if (exp >= LEVEL_EXP_LIMIT[level] && level < 15)
    {
        level ++;
        /*
         * 属性提升
        */
        return true;
    }
    return false;
}

QJsonObject Pokemon::toJson()
{
    QJsonObject pokemonJson;
    pokemonJson.insert("name", name);
    pokemonJson.insert("kind", kind);
    pokemonJson.insert("level", level);
    pokemonJson.insert("exp", exp);
    pokemonJson.insert("atk", atk);
    pokemonJson.insert("maxhp", maxhp);
    pokemonJson.insert("agl", agl);
    pokemonJson.insert("delay", delay);
    return pokemonJson;
}

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

int Normal::Attack(Pokemon *rhs)
{

}

int Fire::Attack(Pokemon *rhs)
{

}

int Water::Attack(Pokemon *rhs)
{

}

int Grass::Attack(Pokemon *rhs)
{

}

int Electric::Attack(Pokemon *rhs)
{

}

Pokemon::~Pokemon(){};

