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
    bool VerifyGrammer(const ParsingTable& table, const std::vector<Token>& token, std::vector<Token>::iterator& token_it, std::vector<StackElement>& stack);

    void InsertFirstElementInStack(const ParsingTable &table, std::vector<StackElement>& stack );

    void FillAllVector(const ParsingTable &table, std::vector<std::string>& all_vect);

    void GetMatrixIndex(const ParsingTable& table, const Token &token, const StackElement& stack, std::vector<std::string>& all_vect,
                        unsigned int& x, unsigned int& y);

    /**
     * @brief MakeStepInGrammer
     * @param table
     * @param token
     * @param stack
     * @return
     */
    bool MakeStepInGrammer(const ParsingTable& table, std::vector<Token>::const_iterator& reversed_tokens,
                           std::vector<StackElement>& stack, std::vector<std::string>& all_vect,
                           unsigned int& x, unsigned int& y);
};

#endif // SYNTACTICANALYZER_H
