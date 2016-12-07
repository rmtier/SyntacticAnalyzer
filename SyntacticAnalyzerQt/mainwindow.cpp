#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lexical_analyzer.h"
#include "exception.h"
#include <QMessageBox>
#include <parsing_table.h>

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

        for (int i = 0; i < pars_table.rules.size(); i++)
        {
             ui->debug_output_plain_text->appendPlainText(QString::fromStdString(pars_table.rules.at(i)));
        }
    }
    catch(Exception e)
    {
        QMessageBox msgBox;
        msgBox.setText(QString::fromStdString(e.exeption_string));
        msgBox.exec();
    }
}
