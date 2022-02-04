#include "practicebattle.h"
#include "ui_practicebattle.h"

PracticeBattle::PracticeBattle(QString userName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PracticeBattle)
{
    ui->setupUi(this);

    username = userName;
}

PracticeBattle::~PracticeBattle()
{
    delete ui;
}

void PracticeBattle::on_level1Btn_clicked()
{
    selectWindow = new SelectPokemon(username, "Practice1");
    QObject::connect(selectWindow, SIGNAL(selectedPokemons(Pokemon*, Pokemon*)), this, SLOT(prepareFight(Pokemon*, Pokemon*)));
    selectWindow->show();
}

void PracticeBattle::prepareFight(Pokemon* minePokemon, Pokemon* enemyPokemon)
{
    qDebug() << "run PracticeBattle::prepareFight";
    selectWindow->hide();
    battleWindow = new Battle(0, username, minePokemon, enemyPokemon, 1);
    battleWindow->show();
}
