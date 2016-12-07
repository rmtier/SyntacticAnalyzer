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
    ///////////////////////////////////////////
    //// this is like main loop for processing
    /// ////////////////////////////////////////

    try
    {
        std::string XML_input = ui->XML_input_plain_text->toPlainText().toStdString();
        std::vector<std::string> numbers;
        std::vector<std::string> words;
        std::vector<std::string> XML_divided_input;

        //start procesing with lexical analyzation
        LexicalAnalyzer lex;
        lex.ReplaceALLWhiteCharsWithSpace(XML_input); //change input
        lex.SplitString(XML_input, ' ', XML_divided_input);

        lex.GetAllNumbers(XML_divided_input, numbers);
        lex.GetAllWords(XML_divided_input, words);

        //debug numbers
        {
            ui->debug_output_plain_text->appendPlainText("Number tokens:");

            for (std::string elem: numbers)
                ui->debug_output_plain_text->appendPlainText(QString::fromStdString(elem));

            ui->debug_output_plain_text->appendPlainText("\nWord tokens:");

            for (std::string elem: words)
                ui->debug_output_plain_text->appendPlainText(QString::fromStdString(elem));
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
    }
    catch(Exception e)
    {
        QMessageBox msgBox;
        msgBox.setText(QString::fromStdString(e.exeption_string));
        msgBox.exec();
    }
}
