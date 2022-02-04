#ifndef MODEL_H
#define MODEL_H

#include "selectpokemon.h"
#include "pokemon.h"
#include "battle.h"

#include <QWidget>


namespace Ui {
class Model;
}

class Model : public QWidget
{
    Q_OBJECT

public:
    explicit Model(QString, QWidget *parent = nullptr);
    ~Model();

private slots:
    void prepareFight(Pokemon*, Pokemon*);

    void on_confirmBtn_clicked();

    void on_difficulty1_clicked();

    void on_difficulty2_clicked();



    void on_difficulty3_clicked();

    void on_difficulty4_clicked();

    void on_mode0_clicked();

    void on_mode1_clicked();

    void on_returnBtn_clicked();

private:
    Ui::Model *ui;
    QString username;
    int difficulty, mode;
    SelectPokemon* selectWindow;
    Battle* battleWindow;
};

#endif // MODEL_H
