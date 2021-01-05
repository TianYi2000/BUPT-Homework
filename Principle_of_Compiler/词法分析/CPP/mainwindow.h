#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "lexical_analyzer.h"
#include "token.h"

#include <QMainWindow>
#include <QWidget>
#include <QLayout>
#include <QVariant>
#include <QPushButton>
#include <QJsonObject>
#include <QScrollArea>
#include <QButtonGroup>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_analyzeBtn_clicked();
    void clickToken();
    void clickError();
    void on_openBtn_clicked();

private:
    Ui::MainWindow *ui;
    Lexical_Analyzer *lexer;
    QButtonGroup *tokenGroup, *errorGroup;
    QVBoxLayout *tokenlayout, *errorlayout;

    void setSymbol();
    void setError();
};
#endif // MAINWINDOW_H
