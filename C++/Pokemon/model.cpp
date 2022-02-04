#include "model.h"
#include "ui_model.h"

#include <QMessageBox>

Model::Model(QString userName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Model)
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
    difficulty = -1;
    mode = -1;
}

Model::~Model()
{
    delete ui;
}

void Model::prepareFight(Pokemon* minePokemon, Pokemon* enemyPokemon)
{
    qDebug() << "run prepareFight";
    selectWindow->hide();
    battleWindow = new Battle(mode, username, minePokemon, enemyPokemon, difficulty, this);
    battleWindow->show();
}

void Model::on_difficulty1_clicked()
{
    difficulty = 1;
    ui->difficultyLabel->setText("入门");
}

void Model::on_difficulty2_clicked()
{
    difficulty = 2;
    ui->difficultyLabel->setText("初级");
}

void Model::on_difficulty3_clicked()
{
    difficulty = 3;
    ui->difficultyLabel->setText("中级");
}

void Model::on_difficulty4_clicked()
{
    difficulty = 4;
    ui->difficultyLabel->setText("高级");
}

void Model::on_mode0_clicked()
{
    mode = 0;
    ui->modeLabel->setText("升级");
}

void Model::on_mode1_clicked()
{
    mode = 1;
    ui->modeLabel->setText("决斗");
}

void Model::on_confirmBtn_clicked()
{
    if (difficulty < 0 || mode < 0)
    {
        QMessageBox::warning(this, "错误", "难度或模式不能为空，请选择！");
        return ;
    }

    switch (difficulty) {
        case 1:
            selectWindow = new SelectPokemon(username, "Difficulty1", this);
            break;
        case 2:
            selectWindow = new SelectPokemon(username, "Difficulty2", this);
            break;
        case 3:
            selectWindow = new SelectPokemon(username, "Difficulty3", this);
            break;
        case 4:
            selectWindow = new SelectPokemon(username, "Difficulty4", this);
            break;
    }
    connect(selectWindow, SIGNAL(selectedPokemons(Pokemon*, Pokemon*)), this, SLOT(prepareFight(Pokemon*, Pokemon*)));
    selectWindow->show();
}

void Model::on_returnBtn_clicked()
{
    delete this;
}
