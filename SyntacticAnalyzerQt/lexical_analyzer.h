#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H
#include <string>
#include <vector>
#include <regex>


class LexicalAnalyzer
{
public:
    LexicalAnalyzer();

    /**
     * @brief ReplaceALLWhiteCharsWithSpace
     * @param str
     */
    void ReplaceALLWhiteCharsWithSpace(std::string &str);

    /**
     * @brief SplitString
     * @param str
     * @param delimiter
     * @param elements
     */
    void SplitString(const std::string &str, char delimiter, std::vector<std::string> &elements);

    /**
     * @brief GetAllNumbers use regex to find all numbers
     * @param input_text
     * @param fill vector structure
     */
    void GetAllNumbers(const std::vector<std::string>& input_text, std::vector<std::string>& out_vect);

    /**
     * @brief GetAllWords use regex to find all words
     * @param input_text
     * @param out vect specific structure
     */
    void GetAllWords(const std::vector<std::string>& input_text, std::vector<std::string>& out_vect);

private:

    /**
     * @brief FindMatch with regex
     * @param input for regexes
     * @param num_regex
     * @param out_vect
     */
    void FindMatch(const std::vector<std::string>& input_text, std::regex num_regex, std::vector<std::string>& out_vect);
};

#endif // LEXICALANALYZER_H
