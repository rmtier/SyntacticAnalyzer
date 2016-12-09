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

    unsigned int y = 0;
    unsigned int x = 0;

    //first nonterm in stack
    StackElement s;
    s.type = ELEMENT_TYPE::nonterminal;
    s.nonterminal = nonterminals.front();
    stack.push_back(s);

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

        //find nonterminal index
        if (stack_elem.type == ELEMENT_TYPE::nonterminal)
        {
            std::vector<Nonterminal>::const_iterator n_it = nonterminals.begin();
            for (; n_it != nonterminals.end(); ++n_it)
            {
                if (n_it->value == stack_elem.nonterminal.value)
                    break;
            }

            if (n_it == nonterminals.end())
                throw Exception("UNKNOW nonterminal while verifying grammer");
            x = std::distance(nonterminals.begin(), n_it);
        }else
        {
            std::vector<Token>::const_iterator it = std::find_if(terminals.begin(), terminals.end(),  [&stack_elem] (const Token & t1) {
                    return (stack_elem.terminal.type == t1.type);
            });
            if (it == terminals.end())
                throw Exception("UNKNOW terminal while verifying grammer");
            x = std::distance(terminals.begin(), it) + nonterminals.size();
        }

        //view from matrix
        std::string value = table.matrix[x][y].value;
        int nonterminal_index = atoi(value.c_str());

        if (nonterminal_index) //if is number
        {
            if (!stack.empty())
                stack.pop_back();

            //push all terminals and non termainls
            for (const StackElement & se: table.rules[nonterminal_index].second)
                stack.push_back(se);
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
