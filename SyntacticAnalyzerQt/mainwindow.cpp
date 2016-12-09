#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lexical_analyzer.h"
#include "exception.h"
#include <QMessageBox>
#include <parsing_table.h>
#include "syntactic_analyzer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->XML_input_plain_text->toPlainText().clear();

    ///////////////////////////////////////////
    //// this is like main loop for processing
    /// ////////////////////////////////////////

    try
    {
        std::string XML_input = ui->XML_input_plain_text->toPlainText().toStdString();

        LexicalAnalyzer lex_analyzer;

        std::vector<Token> token_vect;

        lex_analyzer.ReplaceALLWhiteCharsWithSpace(XML_input);

        lex_analyzer.ParseConfigToTokens(XML_input, token_vect);

        //debug numbers
        {
           for (Token t: token_vect)
                ui->debug_output_plain_text->appendPlainText(QString::fromStdString(t.value));
        }

        //fill matrix from file
        ParsingTable pars_table;

        pars_table.FillTablefromFile("table.txt");

        //debug
        for (int i = 0; i < pars_table.height_size; i++)
        {
            std::string s = "";

            for (int j = 0; j < pars_table.width_size; j++)
            {
                s += pars_table.matrix[i][j].value;
                s += " ";
            }
            ui->debug_output_plain_text->appendPlainText(QString::fromStdString(s));
        }

        pars_table.LoadRulesFromFile("grammar.txt");


        SyntacticAnalyzer syn_anal;

        QMessageBox msgBox;

        if (syn_anal.VerifyGrammer(pars_table, token_vect))
        {
            msgBox.setText("Congratulation!!!! Grammer is OK :)");
        }else
        {
            msgBox.setText("Bad lack!! you have some misstake :)");
        }

        msgBox.exec();
    }
    catch(Exception e)
    {
        QMessageBox msgBox;
        msgBox.setText(QString::fromStdString(e.exeption_string));
        msgBox.exec();
    }
}
