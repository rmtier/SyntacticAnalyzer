#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <parsing_table.h>
#include <QTableWidgetItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    data_filled = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->XML_input_plain_text->toPlainText().clear();

    try
    {
        FillData();

        SyntacticAnalyzer syn_anal;

        QMessageBox msgBox;

        if (syn_anal.VerifyGrammer(pars_table, token_vect, actual_token_it, stack))
        {
            msgBox.setText("Congratulation!!!! Grammer is OK :)");
        }else
        {
            msgBox.setText("Bad lack!! you have some misstake :)");
        }

        ui->step_button->setEnabled(false);
        ui->pushButton->setEnabled(false);

        msgBox.exec();
    }
    catch(Exception e)
    {
        QMessageBox msgBox;
        msgBox.setText(QString::fromStdString(e.exeption_string));
        msgBox.exec();
    }
}

void MainWindow::DrawMatrix()
{
    ui->table_widget->clear();

    ui->table_widget->setRowCount(pars_table.height_size );
    ui->table_widget->setColumnCount(pars_table.width_size - 1);

    //first set horizonals header(terminals)
    for (int i = 0; i < pars_table.terminals.size(); i++)
        ui->table_widget->setHorizontalHeaderItem(i, new QTableWidgetItem(QString::fromLocal8Bit((pars_table.terminals.at(i).value.c_str()))) );

    //first set vertical header(terminals)
    for (int i = 0; i < pars_table.height_size; i++)
        ui->table_widget->setVerticalHeaderItem(i, new QTableWidgetItem(QString::fromLocal8Bit((pars_table.matrix[i][0].value.c_str()))) );

    ui->table_widget->resizeColumnsToContents();

     //new QTableWidgetItem(QString::fromLocal8Bit((pars_table.terminals.at(i).value.c_str()))) );
    //debug
    for (int i = 0; i < pars_table.height_size; i++)
    {
        for (int j = 1; j < pars_table.width_size; j++)
        {
            ui->table_widget->setItem(i, j-1, new QTableWidgetItem(QString::fromLocal8Bit((pars_table.matrix[i][j].value.c_str()))) );
        }
    }

}

void MainWindow::PrintTokens()
{
    ui->token_list_widget->clear();

    //debug tokens
    for (Token& t: token_vect)
         new QListWidgetItem("T:  " + QString::fromLocal8Bit(t.value.c_str()), ui->token_list_widget);
}

void MainWindow::PrintStack()
{
    ui->stack_widget->clear();

    if (!stack.empty())
    {
        std::vector<StackElement>::reverse_iterator s_it = stack.rbegin();
        for (; s_it != stack.rend(); s_it++)
        {
            if (s_it->type == ELEMENT_TYPE::terminal)
            {
                new QListWidgetItem("T:  " + QString::fromLocal8Bit(s_it->terminal.value.c_str()), ui->stack_widget);
            }
            else
            {
                new QListWidgetItem("N:  " + QString::fromLocal8Bit(s_it->nonterminal.value.c_str()), ui->stack_widget);
            }
        }

        ui->stack_widget->item(0)->setSelected(true);
    }
}

void MainWindow::FillData()
{
    CleanData();

    std::string XML_input = ui->XML_input_plain_text->toPlainText().toStdString();

    lex_analyzer.ReplaceALLWhiteCharsWithSpace(XML_input);

    lex_analyzer.ParseConfigToTokens(XML_input, token_vect);

    if (token_vect.size() > 1)
    {
        std::vector<Token>::iterator it = std::next(token_vect.begin());
        while ( it != token_vect.end() )
        {
            if (it->type == TOKEN_TYPE::space &&
                    (std::prev(it)->type != TOKEN_TYPE::let ))
                it = token_vect.erase(it);
            else
                ++it;
        }
    }
    actual_token_it = token_vect.begin();

    PrintTokens();

    //fill matrix from file
    pars_table.FillTablefromFile("table.txt");

    DrawMatrix();

    pars_table.LoadRulesFromFile("new_grammar.txt");


}

void MainWindow::CleanData()
{
    ParsingTable p;

    data_filled = false;
    stack.clear();
    pars_table = p;
    token_vect.clear();
}

void MainWindow::on_step_button_clicked()
{
    try
    {
        SyntacticAnalyzer syn_anal;

        if (!data_filled)
        {
            FillData();
            data_filled = true;
        }

        if  (actual_token_it == token_vect.begin() && stack.empty())
        {
            syn_anal.InsertFirstElementInStack(pars_table, stack);
        }

        std::vector<std::string> all_vect;
        syn_anal.FillAllVector(pars_table, all_vect);

        unsigned int x = 0;
        unsigned int y = 0;

        int number_of_jumps = ui->number_of_jumps_edit_box->text().toInt();

        if (number_of_jumps == 0)
            throw Exception("Select other jump number");

        for (int i = 0; i < number_of_jumps; i++)
        {
            if (stack.empty() || actual_token_it == token_vect.end())
                break;
            syn_anal.MakeStepInGrammer(pars_table, actual_token_it, stack, all_vect, x, y);
        }

        ui->table_widget->clearSelection();

        if (!stack.empty())
        {
            syn_anal.GetMatrixIndex(pars_table, *actual_token_it, stack.back(), all_vect, x, y);
            QModelIndex index = ui->table_widget->model()->index(x, y-1);
            ui->table_widget->selectionModel()->select(index, QItemSelectionModel::Select);
            ui->table_widget->scrollTo(index, QAbstractItemView::PositionAtCenter);
        }

        if (stack.empty() && actual_token_it != token_vect.end())
            throw Exception("Bad INPUT");
        else if (stack.empty() && actual_token_it == token_vect.end())
            throw Exception("Good grammer");

        PrintStack();

        QListWidgetItem * item = ui->token_list_widget->item(std::distance(token_vect.begin(), actual_token_it));

        item->setSelected(true);

        QModelIndex index = ui->token_list_widget->model()->index(std::distance(token_vect.begin(), actual_token_it), 0);
        ui->token_list_widget->scrollTo(index, QAbstractItemView::EnsureVisible);

    }
    catch(Exception e)
    {
        QMessageBox msgBox;
        msgBox.setText(QString::fromStdString(e.exeption_string));
        msgBox.exec();
        ui->step_button->setEnabled(false);
        ui->pushButton->setEnabled(false);
    }
}

void MainWindow::on_revert_button_clicked()
{
    CleanData();

   ui->token_list_widget->clear();
   ui->table_widget->clear();
   ui->stack_widget->clear();

   ui->step_button->setEnabled(true);
   ui->pushButton->setEnabled(true);
}
