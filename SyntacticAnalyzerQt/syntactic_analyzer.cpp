#include "syntactic_analyzer.h"
#include <algorithm>
#include <list>
#include "exception.h"
#include "assert.h"

SyntacticAnalyzer::SyntacticAnalyzer()
{

}

bool SyntacticAnalyzer::VerifyGrammer(const ParsingTable &table, const std::vector<Token>& token, std::vector<Token>::iterator& token_it, std::vector<StackElement>& stack)
{
    std::vector<std::string> all_vect;

    if (token_it == token.begin() && stack.empty())
    {
        InsertFirstElementInStack(table, stack);
    }

    FillAllVector(table, all_vect);

    unsigned int x = 0;
    unsigned int y = 0;

    //read all tokens
    while (token_it != token.end())
    {
        //if stack is empty and tokens not empd - toker are not in grammar
        if (stack.empty())
            return (token_it == token.end() ? true : false);

        MakeStepInGrammer(table, token_it, stack, all_vect, x, y);
    }

    if (stack.empty())
        return true;

    return false;
}

void SyntacticAnalyzer::InsertFirstElementInStack(const ParsingTable &table,  std::vector<StackElement>& stack)
{
    //first nonterm in stack
    StackElement s;
    s.type = ELEMENT_TYPE::nonterminal;
    s.nonterminal = table.nonterminals.front();
    stack.push_back(s);
}

void SyntacticAnalyzer::FillAllVector(const ParsingTable &table, std::vector<std::string>& all_vect)
{
    for (int i = 0; i < table.height_size; i++)
        all_vect.push_back( table.matrix[i][0].value );
}

void SyntacticAnalyzer::GetMatrixIndex(const ParsingTable &table, const Token &token, const StackElement& stack_elem, std::vector<std::string> &all_vect, unsigned int &x, unsigned int &y)
{
    const std::vector<Token> & terminals = table.terminals;

    //if is epsion return 0,0
    if (stack_elem.type == ELEMENT_TYPE::terminal
            && stack_elem.terminal.type == TOKEN_TYPE::epsilon)
    {
        x = 0; y = 0;
        return;
    }

    //find terminal index
    std::vector<Token>::const_iterator it = std::find_if(terminals.begin(), terminals.end(),  [&token] (const Token & t1) {
            return (token.type == t1.type);
    });
    if (it == terminals.end())
        throw Exception("UNKNOW terminal while verifying grammer");
    y = std::distance(terminals.begin(), it) + 1;

    //find x index
    std::vector<std::string>::iterator s_it = all_vect.begin();
    if (stack_elem.type == ELEMENT_TYPE::terminal)
    {
       s_it = std::find(all_vect.begin(), all_vect.end(), stack_elem.terminal.value);
    }
    else
    {
       s_it = std::find(all_vect.begin(), all_vect.end(), stack_elem.nonterminal.value);
    }

    if (s_it == all_vect.end())
        throw Exception("UNKNOW nonterminal while verifying grammer");
    x = std::distance(all_vect.begin(), s_it);
}

bool SyntacticAnalyzer::MakeStepInGrammer(const ParsingTable &table, std::vector<Token>::const_iterator &token_it,
                                          std::vector<StackElement> &stack, std::vector<std::string>& all_vect, unsigned int& x, unsigned int& y)
{
    const Token& token = *token_it;
    const StackElement& stack_elem = stack.back();

    GetMatrixIndex(table, token, stack_elem, all_vect, x, y);

    //view from matrix
    std::string value = table.matrix[x][y].value;
    int nonterminal_index = atoi(value.c_str());

    if (nonterminal_index) //if is number
    {
        if (!stack.empty())
            stack.pop_back();

        //push all terminals and non termainls
        int r_index = table.rules[nonterminal_index-1].second.size() - 1;
        for (; r_index >= 0; r_index--)
        {
            const StackElement& se = table.rules[nonterminal_index-1].second.at(r_index);

            if (se.type == ELEMENT_TYPE::terminal && se.terminal.type == TOKEN_TYPE::epsilon)
                continue;

            stack.push_back( se );
        }
    }
    else if (value == "error")
    {
        if (token_it->type == TOKEN_TYPE::space)
            token_it++;
        else if (!stack.empty())
            stack.pop_back();
    }
    else if (value == "pop")
    {
        if (!stack.empty())
            stack.pop_back();
        token_it = std::next(token_it);
    }else
        assert(false); //implement new

    return true;
}
