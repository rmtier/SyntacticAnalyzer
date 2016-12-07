#ifndef CONFIG_H
#define CONFIG_H
#include <string>

enum TOKEN_TYPE{
   word, number, start, end, dot, startnq, endnq, starte, ende, underscore, minus, doubledot, version
};

struct Token
{
    Token(TOKEN_TYPE type, std::string& value){
        this->type = type;
        this->value = value;
    }

    TOKEN_TYPE type;
    std::string value;
};

#endif // CONFIG_H
