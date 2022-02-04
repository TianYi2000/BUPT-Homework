#ifndef PRACTICEBATTLE_H
#define PRACTICEBATTLE_H

#include "selectpokemon.h"
#include "pokemon.h"
#include "battle.h"

#include <QWidget>

namespace Ui {
class PracticeBattle;
}

class PracticeBattle : public QWidget
{
    Q_OBJECT

public:
    explicit PracticeBattle(QString, QWidget *parent = nullptr);
    ~PracticeBattle();

private slots:
    void on_level1Btn_clicked();
    void prepareFight(Pokemon*, Pokemon*);

private:
    Ui::PracticeBattle *ui;
    QString username;

    SelectPokemon* selectWindow;
    Battle* battleWindow;
};

#endif // PRACTICEBATTLE_H
