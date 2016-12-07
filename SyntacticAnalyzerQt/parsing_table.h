#ifndef PARSINGTABLE_H
#define PARSINGTABLE_H
#include <string>
#include <vector>

struct TableElem
{
    std::string value;
};

class ParsingTable
{
public:
    ParsingTable();

    /**
     * @brief FillTablefromFile fill table from file
     * @param name_of_file
     */
    void FillTablefromFile(std::string name_of_file);

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
    std::vector<std::string> terminals;
    std::vector<std::string> nonterminals;
    std::vector<std::string> rules;
};

#endif // PARSINGTABLE_H
