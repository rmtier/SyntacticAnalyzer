#include "parsing_table.h"
#include "exception.h"
#include <iostream>
#include <fstream>
#include "lexical_analyzer.h"
#include <algorithm>
#include <ostream>
#include "assert.h"
#include "config.h"

ParsingTable::ParsingTable()
{
    matrix = nullptr;
}

ParsingTable::~ParsingTable()
{
    if (matrix != nullptr)
    {
        for (int i = 0; i < height_size; i++)
            delete [] matrix[i];

        delete [] matrix;
    }
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
             FillTerminals(line);
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

             //fill matrix
             unsigned int index_in_matrix = 0;
             for (unsigned int index = 0; index < temp_vect.size(); index++ )
             {
                 std::string& str = temp_vect[index];
                 if (str == "")
                     continue;
                 matrix[index_in_row][index_in_matrix++].value = str;
             }
             index_in_row++;
         }
    }else
        throw Exception("Coudt not load parsing table file!!! Please check is is in your directory");


    file.close();
}

void ParsingTable::FillTerminals(std::string& str)
{
    std::vector<std::string> str_vect;
    LexicalAnalyzer lex_anal;
    lex_anal.ReplaceALLWhiteCharsWithSpace(str);
    lex_anal.SplitString(str, ' ', str_vect);

    auto CreteToken = [] (TOKEN_TYPE type, std::string & str) -> Token
    {
        Token t;
        t.type = type;
        t.value = str;
        return t;
    };

    for (std::string& cur_str: str_vect)
    {
        if(cur_str == "<")
            terminals.push_back(CreteToken(TOKEN_TYPE::startnq, cur_str));
        else if(cur_str == ">")
            terminals.push_back(CreteToken(TOKEN_TYPE::endnq, cur_str));
        else if(cur_str == "</")
            terminals.push_back(CreteToken(TOKEN_TYPE::starte, cur_str));
        else if(cur_str == "/>")
            terminals.push_back(CreteToken(TOKEN_TYPE::ende, cur_str));
        else if(cur_str == "<?")
            terminals.push_back(CreteToken(TOKEN_TYPE::start, cur_str));
        else if(cur_str == "?>")
            terminals.push_back(CreteToken(TOKEN_TYPE::end, cur_str));
        else if(cur_str == "/")
            terminals.push_back(CreteToken(TOKEN_TYPE::slash, cur_str));
        else if(cur_str == "\\")
            terminals.push_back(CreteToken(TOKEN_TYPE::forward_slash, cur_str));
        else if(cur_str == ",")
            terminals.push_back(CreteToken(TOKEN_TYPE::comma, cur_str));
        else if(cur_str == ".")
            terminals.push_back(CreteToken(TOKEN_TYPE::dot, cur_str));
        else if(cur_str == ":")
            terminals.push_back(CreteToken(TOKEN_TYPE::doubledot, cur_str));
        else if(cur_str == "version=")
            terminals.push_back(CreteToken(TOKEN_TYPE::version, cur_str));
        else if(cur_str == "-")
            terminals.push_back(CreteToken(TOKEN_TYPE::minus, cur_str));
        else if(cur_str == "_")
            terminals.push_back(CreteToken(TOKEN_TYPE::underscore, cur_str));
        else if(cur_str == "let")
            terminals.push_back(CreteToken(TOKEN_TYPE::let, cur_str));
        else if(cur_str == "dig")
            terminals.push_back(CreteToken(TOKEN_TYPE::dig, cur_str));
        else if(cur_str == "space")
            terminals.push_back(CreteToken(TOKEN_TYPE::space, cur_str));
        else if(cur_str == "$")
            terminals.push_back(CreteToken(TOKEN_TYPE::end_of_line, cur_str));

    }
}

void ParsingTable::LoadRulesFromFile(std::string name_of_file)
{
    std::ifstream file;
    std::string line;
    LexicalAnalyzer lex_anal;

    file.open(name_of_file);

    if (file.is_open())
    {
         while (!file.eof())
         {
             getline(file,line);

             if (line == "")
                 continue;

             std::vector<std::string> vect;

             lex_anal.SplitString(line, ' ', vect);

             Nonterminal n;
             n.value = vect.front();

             //filter nonterminals
             bool is_in_Nonterminals = false;
             std::vector<Nonterminal>::iterator n_it = nonterminals.begin();
             for (; n_it != nonterminals.end(); ++n_it)
                 if (n_it->value == n.value)
                     is_in_Nonterminals = true;

             if (!is_in_Nonterminals)
                nonterminals.push_back(n);

             //move iterator to first non tern, or term
             auto it = vect.begin();
             it = std::next(it);
             it = std::next(it);

             std::vector<StackElement> rules;
             while (it != vect.end() )
             {
                 if (it->at(0) == '\'') //if is terminal
                 {
                     std::string & str = *it;

                     //erase "'"
                     str.erase(std::remove_if(str.begin(), str.end(), [] (char c) {
                         return (c == '\'');
                     }), str.end());

                     //create token
                     std::vector<Token> token_vect;

                     StackElement s;
                     s.type = ELEMENT_TYPE::terminal;

                     if (str != "space")
                     {
                         Token t;
                         t.type = TOKEN_TYPE::space;
                         t.value = str;
                         s.terminal = t;
                     }else if (str != "dig")
                     {
                         Token t;
                         t.type = TOKEN_TYPE::dig;
                         t.value = str;
                         s.terminal = t;
                     }
                     else if (str != "let")
                     {
                         Token t;
                         t.type = TOKEN_TYPE::let;
                         t.value = str;
                         s.terminal = t;
                     }else
                     {
                         lex_anal.ParseConfigToTokens(str, token_vect);
                         assert(token_vect.size() == 1);
                         s.terminal = token_vect.front();
                     }

                     rules.push_back( s );
                 }
                 else
                 {
                     StackElement s;
                     Nonterminal n;
                     n.value = *it;
                     s.type = ELEMENT_TYPE::nonterminal;
                     s.nonterminal = n;
                     rules.push_back( s );
                 }
                 ++it;
             }

             this->rules.push_back(std::make_pair(n, rules));
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
