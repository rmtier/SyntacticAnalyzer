#include "syntactic_analyzer.h"
#include <algorithm>
#include <list>
#include "exception.h"
#include "assert.h"

SyntacticAnalyzer::SyntacticAnalyzer()
{

}

bool SyntacticAnalyzer::VerifyGrammer(const ParsingTable &table, const std::vector<Token>& token)
{
    std::vector<StackElement> stack;
    const std::vector<Nonterminal> nonterminals = table.nonterminals;
    const std::vector<Token> & terminals = table.terminals;
    std::vector<Token> reversed_token_vec(token.rbegin(), token.rend());
    std::vector<std::string> all_vect;

    unsigned int y = 0;
    unsigned int x = 0;

    //first nonterm in stack
    StackElement s;
    s.type = ELEMENT_TYPE::nonterminal;
    s.nonterminal = nonterminals.front();
    stack.push_back(s);

    for (int i = 0; i < table.height_size; i++)
        all_vect.push_back( table.matrix[i][0].value );

    //read all tokens
    while (!reversed_token_vec.empty())
    {
        const Token token = reversed_token_vec.back();

        //if stack is empty and tokens not empd - toker are not in grammar
        if (stack.empty())
            return (reversed_token_vec.empty() ? true : false);

        const StackElement stack_elem = stack.back();

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
                stack.push_back( table.rules[nonterminal_index-1].second.at(r_index));
        }
        else if (value == "error")
        {
            if (!stack.empty())
                stack.pop_back();
        }
        else if (value == "pop")
        {
            if (!stack.empty())
                stack.pop_back();
            reversed_token_vec.pop_back();
        }else
            assert(false); //implement new
    }

    if (stack.empty())
        return true;

    return false;
}
