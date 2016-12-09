#ifndef PARSINGTABLE_H
#define PARSINGTABLE_H
#include <string>
#include <vector>
#include "config.h"

struct TableElem
{
    std::string value;
};

class ParsingTable
{
    typedef std::vector<std::pair<Nonterminal, std::vector<StackElement>>> RULE_VECT; //nonterminal has vector terminals or nonterminals

public:
    ParsingTable();

    /**
     * @brief FillTablefromFile fill table from file
     * @param name_of_file
     */
    void FillTablefromFile(std::string name_of_file);

    void FillTerminals(std::string& str);

    /**
     * @brief LoadRulesFromFile
     * @param name_of_file
     */
    void LoadRulesFromFile(std::string name_of_file);

private:
    /**
     * @brief GetNumberOfLineFromFile
     * @param name
     */
    unsigned int GetNumberOfLineFromFile(std::string name);
public:
    TableElem** matrix; //matrix
    unsigned int width_size;
    unsigned int height_size;
    std::vector<Token> terminals;
    std::vector<Nonterminal> nonterminals;
    RULE_VECT rules;
};

#endif // PARSINGTABLE_H
