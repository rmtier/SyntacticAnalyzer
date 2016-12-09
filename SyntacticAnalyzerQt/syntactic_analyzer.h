#ifndef SYNTACTICANALYZER_H
#define SYNTACTICANALYZER_H
#include "parsing_table.h"

class SyntacticAnalyzer
{
public:
    SyntacticAnalyzer();

    /**
     * @brief VerifyGrammer verify if token is part of grammer
     * @param table
     * @param token
     * @return
     */
    bool VerifyGrammer(const ParsingTable& table, const std::vector<Token>& token);
};

#endif // SYNTACTICANALYZER_H
