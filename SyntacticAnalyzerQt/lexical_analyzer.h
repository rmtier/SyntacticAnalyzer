#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H
#include <string>
#include <vector>
#include <regex>
#include "config.h"


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

    /**
     * @brief IsWord
     * @param str
     * @return
     */
    bool IsWord(const std::string& str);

    /**
     * @brief IsNumber
     * @param str
     * @return
     */

    bool IsNumber(const std::string& str);

    bool IsVersion(const std::string& str);

    /**
     * @brief ParseConfigToTokens
     * @param input_text
     * @param out_vect
     */
    void ParseConfigToTokens(const std::string& input_text, std::vector<Token>& out_vect);

private:

    /**
     * @brief FindMatch with regex
     * @param input for regexes
     * @param num_regex
     * @param out_vect
     */
    void FindMatch(const std::vector<std::string>& input_text, std::regex& num_regex, std::vector<std::string>& out_vect);

    /**
     * @brief FindMatch
     * @param input_text
     * @param num_regex
     * @param out_vect
     */
    void FindMatch(const std::string& input_text, std::regex& num_regex, std::vector<std::string>& out_vect);

    std::string number_regex;
    std::string word_regex;
};

#endif // LEXICALANALYZER_H
