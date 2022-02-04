#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QTime>
#include <QDebug>
#include <QDebug>
#include <QImage>
#include <thread>
#include <QtGlobal>
#include <QSqlQuery>
#include <QSqlError>
#include <QByteArray>
#include <QSqlDatabase>
#include <QJsonDocument>
#include <QNetworkDatagram>
#include <QCryptographicHash>

const int SERVER_PORT = 11110;
const int TOTAL_POKEMON = 89;

int MainWindow::userNum = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    udpInit();
    dataBaseInit();
}

void MainWindow::udpInit()
{
    serverudp=new QUdpSocket(this);
    serverudp->bind(QHostAddress::LocalHost, SERVER_PORT);
    connect(serverudp,SIGNAL(readyRead()),this,SLOT(receiveData()));
}

void MainWindow::dataBaseInit()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("userData.db");
    if (!db.open())
    {
        qDebug() << "数据库打开失败";
    }
    QSqlQuery query;

    /*
    query.exec("CREATE TABLE userAccount ("
                       "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                       "username TEXT NOT NULL, "
                       "password TEXT NOT NULL)");
    */

    query.exec("UPDATE userAccount SET online = 0"); //初始状态所有用户离线
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::receiveData()
{
    while(serverudp->hasPendingDatagrams())
    {
        QByteArray data;
        data.resize(serverudp->pendingDatagramSize());
        QHostAddress addr;
        quint16 port;
        serverudp->readDatagram(data.data(),data.size(),&addr,&port);

        std::thread nowThread = std::thread(std::mem_fn(&MainWindow::analysisByteArray),this, data, addr, port);
        nowThread.detach();
    }
}

void MainWindow::analysisByteArray(QByteArray userData, QHostAddress addr, int port)
{
    QString connectionName ;
    {
        connectionName = "Database" + QString::number(qrand() % 10000);
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName("userData.db");
        if (!db.open())
        {
            qDebug() << "数据库打开失败";
        }
        QSqlQuery query(db);
        QJsonParseError json_error;
        QJsonDocument userDoc(QJsonDocument::fromJson(userData, &json_error));
        if(json_error.error != QJsonParseError::NoError){
            qDebug() << "json error!";
        }
        QJsonObject userInfo = userDoc.object(), retInfo;
        if (userInfo["optype"] == "LOG_IN")
        {
            query.exec("SELECT * FROM userAccount WHERE username = \"" + userInfo["username"].toString() + "\" ;");
            if (!query.next())
            {
                qDebug() << query.lastError();
                qDebug() << "user not existed";
                retInfo["result"] = "LOGIN_FAIL";
                retInfo["info"] = "user not existed";
            }
            else
            {
                QString password = query.value(2).toString();
                if (password == userInfo["password"].toString())
                {

                    query.exec("UPDATE userAccount SET online = 1 WHERE username = \"" + userInfo["username"].toString() + "\" ;");
                    retInfo["result"] = "LOGIN_SUCCESS";
                    retInfo["info"] = "";
                }
                else
                {
                    retInfo["result"] = "LOGIN_FAIL";
                    retInfo["info"] = "wrong password";
                }

            }
        }

        if (userInfo["optype"] == "SIGN_UP")
        {
            query.exec("SELECT * FROM userAccount WHERE username = \"" + userInfo["username"].toString() + "\" ;");
            if (query.next())
            {
                retInfo["result"] = "SIGN_UP_FAIL";
                retInfo["info"] = "user already existed";
            }
            else
            //注册成功
            {
                qDebug() << "添加新用户";
                qDebug() << "账户:" << userInfo["username"].toString();
                qDebug() << "密码:" << userInfo["password"].toString();
                QString Sql = "INSERT INTO userAccount VALUES (";
                Sql += "NULL,";
                Sql += "'" + userInfo["username"].toString() + "',";
                Sql += "'" + userInfo["password"].toString() + "',";
                Sql += "1, 0, 0, 0)";
                query.exec(Sql);

                retInfo["result"] = "SIGN_UP_SUCCESS";
                retInfo["info"] = "";

                //获取三个初始精灵
                for (int i = 1; i <= 3; ++i)
                {

                    Pokemon* NewPokemon = CreatePokemon(db);
                    if (NewPokemon != NULL)
                    {
                        qDebug() << i;
                        query.exec("INSERT INTO userPokemon" + NewPokemon->toSql(userInfo["username"].toString()));
                        delete NewPokemon;
                    }

                }
            }
        }

        if (userInfo["optype"] == "GET_USER_POKEMON")
        {
            qDebug() << "请求用户宝可梦信息";
            retInfo = userPokemontoJson(userInfo["username"].toString(), 9999, db);
            retInfo.insert("result", "pokemonlist");

            query.exec("SELECT * FROM userAccount WHERE username = \"" + userInfo["username"].toString() + "\" ;");
            if(query.next())
            {
                retInfo.insert("battle", query.value(5).toInt());
                retInfo.insert("win", query.value(6).toInt());
            }
            qDebug() << query.lastError();
            qDebug() << retInfo;
        }

        if (userInfo["optype"] == "LOG_OUT")
        {
            qDebug() << "用户注销";

            query.exec("SELECT * FROM userAccount WHERE username = \"" + userInfo["username"].toString() + "\" ;");
            if (query.next())
            {
                query.exec("UPDATE userAccount SET online = 0 WHERE username = \"" + userInfo["username"].toString() + "\" ;");
                retInfo["result"] = "LOG_OUT_SUCCESS";
                retInfo["info"] = "";
            }
            else
            {
                qDebug() << "注销失败；原因：数据库中未找到该用户的账户信息";
                retInfo["result"] = "LOG_OUT_FAIL";
                retInfo["info"] = "cannot find user";
            }

        }

        if (userInfo["optype"] == "GET_USERLIST")
        {
            qDebug() << "返回用户列表";
            query.exec("SELECT * FROM userAccount;");
            retInfo.insert("result", "userlist");

            int cnt = 0;
            while (query.next())
                if (query.value(4).toInt() == 0)    //非系统账户
                {
                    QJsonObject nowUser;
                    nowUser.insert("username", query.value(1).toString());
                    nowUser.insert("online", query.value(3).toInt());

                    retInfo.insert("user" + QString::number(cnt++), nowUser);
                }
            retInfo.insert("number", cnt);
            qDebug() << "用户数量为" << cnt;
        }

        if (userInfo["optype"] == "UPDATE_POKEMON_INFO")
        {
            qDebug() << "更新宝可梦信息";
            qDebug() << "需要更新的宝可梦id为" << userInfo["id"].toInt();
            query.exec("SELECT * FROM userPokemon WHERE id = " + QString::number(userInfo["id"].toInt()));
            if (query.next())
            {
                query.exec("UPDATE userPokemon SET atk = " + QString::number( userInfo["atk"].toInt()) + " WHERE id = " + QString::number( userInfo["id"].toInt()));
                query.exec("UPDATE userPokemon SET def = " + QString::number( userInfo["def"].toInt()) + " WHERE id = " + QString::number( userInfo["id"].toInt()));
                query.exec("UPDATE userPokemon SET hp = " + QString::number( userInfo["maxhp"].toInt()) + " WHERE id = " + QString::number( userInfo["id"].toInt()));
                query.exec("UPDATE userPokemon SET agl = " + QString::number( userInfo["agl"].toInt()) + " WHERE id = " + QString::number( userInfo["id"].toInt()));
                query.exec("UPDATE userPokemon SET exp = " + QString::number( userInfo["exp"].toInt()) + " WHERE id = " + QString::number( userInfo["id"].toInt()));
                query.exec("UPDATE userPokemon SET level = " + QString::number( userInfo["level"].toInt()) + " WHERE id = " + QString::number( userInfo["id"].toInt()));
                qDebug() << query.lastError();
            }
            else{
                qDebug() << "错误" << "未找到指定id的宝可梦";
            }
        }

        if (userInfo["optype"] == "LOSE_BATTLE")
        {
            qDebug() << "选择要丢弃的宝可梦";
            retInfo = userPokemontoJson(userInfo["username"].toString(), 3, db);
            retInfo.insert("result", "pokemonlist");
            qDebug() << retInfo;
        }

        if (userInfo["optype"] == "DROP_POKEMON")
        {
            qDebug() << "确认丢弃的宝可梦";
            query.exec("DELETE FROM userPokemon WHERE id = " + QString::number(userInfo["id"].toInt()));
            qDebug() << query.lastError();
        }

        if (userInfo["optype"] == "WIN_POKEMON")
        {
            qDebug() << "用户赢得宝可梦";
            qDebug() << userInfo["username"].toString() << "获得" << userInfo["name"].toString();
            query.exec("SELECT * FROM userPokemon WHERE id = " + QString::number(userInfo["id"].toInt()));
            if (query.next())
            {
                QString username = userInfo["username"].toString();
                QString name = query.value(2).toString();
                int atk = query.value(3).toInt();
                int hp = query.value(4).toInt();
                int def = query.value(5).toInt();
                int agl = query.value(6).toInt();
                int type = query.value(7).toUInt();
                int kind = query.value(8).toInt();
                QString skill = query.value(9).toString();
                int exp = query.value(10).toInt();
                int level = query.value(11).toInt();
                QString engname = query.value(12).toString();

                QString Sql = "INSERT INTO userPokemon VALUES(";
                Sql +=  "NULL,";
                Sql += "'" + username + "',";
                Sql += "'" + name + "',";
                Sql += QString::number(atk) + ",";
                Sql += QString::number(hp) + ",";
                Sql += QString::number(def) + ",";
                Sql += QString::number(agl) + ",";
                Sql += QString::number(type) + ",";
                Sql += QString::number(kind) + ",";
                Sql += "'" + skill + "',";
                Sql += QString::number(exp) + ",";
                Sql += QString::number(level) + ",";
                Sql += "'" + engname + "')";
                query.exec(Sql);
            }
            qDebug() << query.lastError();
        }
        if (userInfo["optype"] == "ALL_OUT")
        {
            qDebug() << "用户失去所有宝可梦，系统赠送其一个";
            Pokemon* NewPokemon = CreatePokemon(db);
            if (NewPokemon != NULL)
            {
                query.exec("INSERT INTO userPokemon" + NewPokemon->toSql(userInfo["username"].toString()));
                delete NewPokemon;
            }

            qDebug() << query.lastError();
        }
        if (userInfo["optype"] == "WIN")
        {
            qDebug() << "用户胜利";
            query.exec("SELECT * FROM userAccount WHERE username = \"" + userInfo["username"].toString() + "\" ;");

            if (query.next())
            {
                int battle = query.value(5).toInt();
                int win = query.value(6).toUInt();
                query.exec("UPDATE userAccount SET battle =" + QString::number(battle + 1) +", win ="
                           + QString::number(win + 1) + " WHERE username = \"" + userInfo["username"].toString() + "\" ;");
            }
            else
            {
                qDebug() << "注销失败；原因：数据库中未找到该用户的账户信息";
            }
            qDebug() << query.lastError();
        }

        if (userInfo["optype"] == "LOSE")
        {
            qDebug() << "用户失败";
            query.exec("SELECT * FROM userAccount WHERE username = \"" + userInfo["username"].toString() + "\" ;");

            if (query.next())
            {
                int battle = query.value(5).toInt();
                query.exec("UPDATE userAccount SET battle =" + QString::number(battle + 1) +" WHERE username = \"" + userInfo["username"].toString() + "\" ;");
            }
            else
            {
                qDebug() << "注销失败；原因：数据库中未找到该用户的账户信息";
            }
            qDebug() << query.lastError();
        }

        udpSend(retInfo, addr, port);
        db.close();
    }

    QSqlDatabase::removeDatabase(connectionName);
    return ;
}

void MainWindow::udpSend(QJsonObject logInfo, QHostAddress addr, int port)
{
    QJsonDocument logDoc;
    logDoc.setObject(logInfo);
    QByteArray logData = logDoc.toJson();

    serverudp->writeDatagram(logData, addr, port);
}

void MainWindow::dataBaseInsert(QJsonObject userInfo, QSqlDatabase& db)
{
    QSqlQuery query(db);
    qDebug() << "账户:" << userInfo["username"].toString();
    qDebug() << "密码:" << userInfo["password"].toString();
    QString Sql = "INSERT INTO userAccount VALUES (";
    Sql += "NULL,";
    Sql += "'" + userInfo["username"].toString() + "',";
    Sql += "'" + userInfo["password"].toString() + "',";
    Sql += "1, 0, 0, 0)";
    query.exec(Sql);
}

Pokemon* MainWindow::CreatePokemon(QSqlDatabase& db)
{
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    int num = qrand() % (TOTAL_POKEMON + 1);
    QSqlQuery query(db);
    query.exec("SELECT * FROM initialPokemon WHERE num = " + QString::number(num) + " ;");
//    qDebug() << query.lastError();
    Pokemon* newPokemon = NULL;
    if (query.next())
    {
        QString name = query.value(1).toString();
        qDebug() << name;
        int atk = query.value(2).toInt();
        int hp = query.value(3).toInt();
        int def = query.value(4).toInt();
        int agl = query.value(5).toInt();
        QString typeString = query.value(6).toString();
        Category kind = Category(query.value(7).toInt());
        QString skill = query.value(8).toString();
        QString engname = query.value(9).toString();
        qDebug() << skill;
//        qDebug() << query.lastError();
        if (typeString == "一般")
            newPokemon = new Normal(name, kind, atk, def, hp, agl, skill, engname);
        if (typeString == "火")
            newPokemon = new Fire(name, kind, atk, def, hp, agl, skill, engname);
        if (typeString == "水")
            newPokemon = new Water(name, kind, atk, def, hp, agl, skill, engname);
        if (typeString == "草")
            newPokemon = new Grass(name, kind, atk, def, hp, agl, skill, engname);
        if (typeString == "电")
            newPokemon = new Electric(name, kind, atk, def, hp, agl, skill, engname);
    }
    else qDebug() << "初始化宝可梦失败";
    return newPokemon;
}

QList<QJsonObject> MainWindow::GetUserPokemon(QString username, QSqlDatabase& db)
{
    QSqlQuery query(db);
    QList<QJsonObject> pokemonList;
    query.exec("SELECT * FROM userPokemon WHERE username = '" + username + "' ;");

    while (query.next())
    {
        qDebug() << "查询到宝可梦";
        int id = query.value(0).toInt();
        QString name = query.value(2).toString();
        int atk = query.value(3).toInt();
        int hp = query.value(4).toInt();
        int def = query.value(5).toInt();
        int agl = query.value(6).toInt();
        int type = query.value(7).toInt();
        int kind = query.value(8).toInt();
        QString skill = query.value(9).toString();
        int exp = query.value(10).toInt();
        int level = query.value(11).toInt();
        QString engname = query.value(12).toString();

        QJsonObject nowPokemon;
        nowPokemon.insert("id", id);
        nowPokemon.insert("name", name);
        nowPokemon.insert("atk", atk);
        nowPokemon.insert("hp", hp);
        nowPokemon.insert("def", def);
        nowPokemon.insert("agl", agl);
        nowPokemon.insert("type", type);
        nowPokemon.insert("kind", kind);
        nowPokemon.insert("skill", skill);
        nowPokemon.insert("exp", exp);
        nowPokemon.insert("level", level);
        nowPokemon.insert("engname", engname);

        pokemonList.append(nowPokemon);

    }
    return pokemonList;
}

QJsonObject MainWindow::userPokemontoJson(QString username, int limit, QSqlDatabase& db)
{
    QJsonObject pokemonInfo;
    QList<QJsonObject> pokemonList = GetUserPokemon(username, db);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    while(pokemonList.size() > limit)
    {
        int drop = rand() % pokemonList.size();
        pokemonList.removeAt(drop);
    }
    pokemonInfo.insert("number", pokemonList.size());
    for (int i = 0; i < pokemonList.size(); ++i)
    {
        pokemonInfo.insert("pokemon" + QString::number(i), pokemonList[i]);
    }
    return pokemonInfo;
}







