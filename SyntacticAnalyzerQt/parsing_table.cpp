#include "parsing_table.h"
#include "exception.h"
#include<iostream>
#include<fstream>
#include "lexical_analyzer.h"
#include <algorithm>
#include <ostream>

ParsingTable::ParsingTable()
{

}

void ParsingTable::FillTablefromFile(std::string name_of_file)
{
    std::ifstream file;
    file.open(name_of_file);

    if (file.is_open())
    {
         unsigned int number_of_lines = 0;

         height_size = (GetNumberOfLineFromFile(name_of_file) - 1) / 2;

         std::string line;
         LexicalAnalyzer lex_anal;

         if(!file.eof())
         {
             getline(file,line);
             lex_anal.ReplaceALLWhiteCharsWithSpace(line);
             lex_anal.SplitString(line, ' ', terminals); //get all terminals
             width_size = terminals.size() + 1;
         }
         else
             throw Exception("Bad format of parsing table!! Please check it");

         //alloc table
         matrix = new TableElem*[height_size];
         for (int i = 0; i < height_size; i++)
             matrix[i] = new TableElem[width_size];

         unsigned int index_in_row = 0;
         while (!file.eof())
         {
             std::vector<std::string> temp_vect;
             getline(file,line);

             if (line == "")    //skip empty lines
                 continue;

             lex_anal.ReplaceALLWhiteCharsWithSpace(line);
             lex_anal.SplitString(line, ' ', temp_vect);

             if (std::find(terminals.begin(), terminals.end(), temp_vect.front()) == terminals.end())
             {
                 nonterminals.push_back(temp_vect.front());
             }
             //fill matrix
             unsigned int index_in_matrix = 0;
             for (std::string str: temp_vect)
                 matrix[index_in_row][index_in_matrix++].value = str;

             index_in_row++;
         }
    }else
        throw Exception("Coudt not load parsing table file!!! Please check is is in your directory");


    file.close();
}

void ParsingTable::LoadRulesFromFile(std::string name_of_file)
{
    std::ifstream file;
    std::string line;

    file.open(name_of_file);

    if (file.is_open())
    {
         while (!file.eof())
         {
             getline(file,line);
             rules.push_back(line);
         }
    }else
        throw Exception("Coudt not load rules file!!! Please check is is in your directory");

    file.close();
}

unsigned int ParsingTable::GetNumberOfLineFromFile(std::string name_of_file)
{
    std::ifstream file;
    std::string line;

    file.open(name_of_file);
    unsigned int number_Of_lines = 0;

     while (!file.eof())
     {
         getline(file,line);
         number_Of_lines++;
     }

    file.close();

    return number_Of_lines;
}
