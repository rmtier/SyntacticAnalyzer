#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "parsing_table.h"
#include "lexical_analyzer.h"
#include "exception.h"
#include "syntactic_analyzer.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_step_button_clicked();

    void on_revert_button_clicked();

public:
    void DrawMatrix();
    void PrintTokens();
    void PrintStack();
    void FillData();
    void CleanData();

private:
    Ui::MainWindow *ui;
    ParsingTable pars_table; //has information about trminals, nonterminals, and rules and table
    LexicalAnalyzer lex_analyzer; //tokenize input
    std::vector<Token> token_vect; //tokens
    std::vector<Token>::iterator actual_token_it; //actual token prcessing
    std::vector<StackElement> stack; //stack
    bool data_filled;
};

#endif // MAINWINDOW_H
