#include "lexical_analyzer.h"
#include <sstream>

LexicalAnalyzer::LexicalAnalyzer()
{

}

void LexicalAnalyzer::ReplaceALLWhiteCharsWithSpace(std::string &str)
{
    std::replace( str.begin(), str.end(), '\t', ' ');
    std::replace( str.begin(), str.end(), '\n', ' ');
    std::replace( str.begin(), str.end(), '\r', ' ');
}

void LexicalAnalyzer::SplitString(const std::string &str, char delimiter, std::vector<std::string> &elements)
{
    std::stringstream ss;
    ss.str(str);
    std::string item;

    while (std::getline(ss, item, delimiter))
    {
        elements.push_back(item);
    }
}

void LexicalAnalyzer::GetAllNumbers(const std::vector<std::string>& input_text, std::vector<std::string>& out_vect)
{
    std::regex num_regex("[0-9]+");

    FindMatch(input_text, num_regex, out_vect);
}



void  LexicalAnalyzer::GetAllWords(const std::vector<std::string>& input_text, std::vector<std::string>& out_vect)
{
    std::regex num_regex("[a-z,0-9,A-Z]+");

    FindMatch(input_text, num_regex, out_vect);
}

void LexicalAnalyzer::FindMatch(const std::vector<std::string>& input_text, std::regex num_regex, std::vector<std::string>& out_vect)
{
    std::smatch pieces_match;

    for (auto str: input_text)
    {
        if (std::regex_match(str, pieces_match, num_regex))
        {
            for (size_t i = 0; i < pieces_match.size(); ++i)
            {
                std::ssub_match sub_match = pieces_match[i];
                out_vect.push_back(sub_match.str());
            }
        }
    }
}
