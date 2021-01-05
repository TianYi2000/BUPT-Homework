#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tokenlayout = NULL;
    errorlayout = NULL;
    ui->lineNum->hide();
    ui->tokenNum->hide();
    ui->errorNum->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_analyzeBtn_clicked()
{
    QString str = ui->codeText->toPlainText();
    qDebug() << str;
    lexer = new Lexical_Analyzer(str.toStdString());
    lexer->Run();
    ui->lineNum->show();
    ui->tokenNum->show();
    ui->errorNum->show();
    ui->lineNum->setText(QString::number(lexer->line));
    ui->tokenNum->setText(QString::number(lexer->tot_token));
    ui->errorNum->setText(QString::number(lexer->tot_error));
    setSymbol();
    setError();
}

void MainWindow::setSymbol()
{
    if (tokenlayout != NULL)
    {
        QList<QPushButton*> btns = ui->symbolScroll->findChildren<QPushButton*>();
        foreach (QPushButton* btn, btns) {   delete btn;  }
        delete tokenlayout;
    }

    tokenlayout = new QVBoxLayout();

    int totNum = lexer->tot_token;
    qDebug() << totNum;
    tokenGroup = new QButtonGroup(this);
    for (int i = 1; i <= totNum; ++i)
    {

        QPushButton* tokenBtn = new QPushButton(this);
        tokenBtn->setText("<"
                          + QString::fromStdString(TypeName[lexer->symbol_table[i].type]) + " , "
                          + "\"" + QString::fromStdString(lexer->symbol_table[i].val) + "\"" + " in "
                          + "( " + QString::number(lexer->symbol_table[i].linePos) + " , "
                          + QString::number(lexer->symbol_table[i].colPos) + " ) >");

        tokenBtn->setMaximumHeight(30);
        tokenBtn->setMinimumHeight(30);
        tokenBtn->setStyleSheet("font-size:12");
        tokenBtn->setStyleSheet("text-align : left");

        tokenBtn->setCheckable(true);
        tokenGroup->addButton(tokenBtn, i);

        tokenlayout->addWidget(tokenBtn);
        QObject::connect(tokenBtn, SIGNAL(clicked()), this, SLOT(clickToken()));
    }
    tokenlayout->addStretch();
    ui->symbolScroll->widget()->setLayout(tokenlayout);
}

void MainWindow::setError()
{
    if (errorlayout != NULL)
    {
        QList<QPushButton*> btns = ui->errorScroll->findChildren<QPushButton*>();
        foreach (QPushButton* btn, btns) {   delete btn;  }
        delete errorlayout;
    }

    errorlayout = new QVBoxLayout();

    int totNum = lexer->tot_error;
    qDebug() << totNum;
    errorGroup = new QButtonGroup(this);
    for (int i = 1; i <= totNum; ++i)
    {

        QPushButton* errorBtn = new QPushButton(this);
        errorBtn->setText(QString::fromStdString(TypeName[lexer->error_symbol[i].type]) +
                          + " in ( " + QString::number(lexer->error_symbol[i].linePos) + " , "
                          + QString::number(lexer->error_symbol[i].colPos) + " )");

        errorBtn->setMaximumHeight(30);
        errorBtn->setMinimumHeight(30);
        errorBtn->setStyleSheet("font-size:12");
        errorBtn->setStyleSheet("text-align : left");

        errorBtn->setCheckable(true);
        errorGroup->addButton(errorBtn, i);

        errorlayout->addWidget(errorBtn);
        QObject::connect(errorBtn, SIGNAL(clicked()), this, SLOT(clickError()));
    }
    errorlayout->addStretch();
    ui->errorScroll->widget()->setLayout(errorlayout);
}

void MainWindow::clickToken()
{
    int id = tokenGroup->checkedId();
}

void MainWindow::clickError()
{
    int id = tokenGroup->checkedId();
    QTextCursor tmpCursor = ui->codeText->textCursor();
    tmpCursor.setPosition(lexer->error_symbol[id].linePos);
    ui->codeText->setTextCursor(tmpCursor);
}

void MainWindow::on_openBtn_clicked()
{
    QString file_path = QFileDialog::getOpenFileName(this,"选择一个代码文件", ".", "Code(*.txt *.cpp *.mycpp *.h)");
    ui->lineEdit->setText(file_path);

    ui->codeText->clear();
    QFile file(file_path);
    if(!file.open(QFile::ReadOnly | QFile::Text))
       return;
    QTextStream in(&file);
    while(!in.atEnd())
    {
        QString line = in.readLine();
        ui->codeText->append(line);
    }
}
