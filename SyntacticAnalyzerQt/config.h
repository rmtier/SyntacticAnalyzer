#ifndef CONFIG_H
#define CONFIG_H
#include <string>

enum TOKEN_TYPE{
   word, number, start, end, dot, startnq, endnq, starte,
   ende, underscore, minus, doubledot, version,
   slash, forward_slash, comma, dig, let, space, end_of_line
};

struct Token
{
    TOKEN_TYPE type;
    std::string value;
};

enum ELEMENT_TYPE
{
    terminal, nonterminal
};

struct Nonterminal
{
    std::string value;
};

struct StackElement
{
    ELEMENT_TYPE type;

    Token terminal;
    Nonterminal nonterminal;
};

#endif // CONFIG_H
