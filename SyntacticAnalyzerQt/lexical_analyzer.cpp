#include "lexical_analyzer.h"
#include <sstream>

LexicalAnalyzer::LexicalAnalyzer()
{
    number_regex = "[0-9]+";
    word_regex = "[a-z,0-9,A-Z]+";
}

void LexicalAnalyzer::ReplaceALLWhiteCharsWithSpace(std::string &str)
{
    std::replace( str.begin(), str.end(), '\t', ' ');
    std::replace( str.begin(), str.end(), '\n', ' ');
    std::replace( str.begin(), str.end(), '\r', ' ');
    std::replace( str.begin(), str.end(), '"', ' ');
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
    std::regex num_regex(number_regex);

    FindMatch(input_text, num_regex, out_vect);
}

void  LexicalAnalyzer::GetAllWords(const std::vector<std::string>& input_text, std::vector<std::string>& out_vect)
{
    std::regex num_regex(word_regex);

    FindMatch(input_text, num_regex, out_vect);
}

bool LexicalAnalyzer::IsWord(const std::string &str)
{
    std::regex num_regex(word_regex);

    std::vector<std::string> vect;

    FindMatch(str, num_regex, vect);

    if (vect.empty())
        return false;
    return true;
}

bool LexicalAnalyzer::IsNumber(const std::string &str)
{
    std::regex num_regex(number_regex);

    std::vector<std::string> vect;

    FindMatch(str, num_regex, vect);

    if (vect.empty())
        return false;
    return true;
}

bool LexicalAnalyzer::IsVersion(const std::string &str)
{
    std::regex num_regex("version=");

    std::vector<std::string> vect;

    FindMatch(str, num_regex, vect);

    if (vect.empty())
        return false;
    return true;
}

void LexicalAnalyzer::ParseConfigToTokens(const std::string &input_text, std::vector<Token> &out_vect)
{
    std::string stack_for_strings;

    auto CreteToken = [] (TOKEN_TYPE type, std::string & str) -> Token
    {
        Token t;
        t.type = type;
        t.value = str;
        return t;
    };

    auto CreateString = [&CreteToken] (std::string& str, std::vector<Token> &out_vect, LexicalAnalyzer* lex_a) {
        if (lex_a->IsVersion(str))
        {
            out_vect.push_back(CreteToken(TOKEN_TYPE::version, str));
        }else
            for (char c: str)
            {
                if (lex_a->IsNumber(std::string(&c)))
                    out_vect.push_back(CreteToken(TOKEN_TYPE::dig, std::string(&c)));
                else if (lex_a->IsWord(std::string(&c)))
                    out_vect.push_back(CreteToken(TOKEN_TYPE::let, std::string(&c)));
            }
        str.clear();
    };

    unsigned int index = 0;
    for (; index < input_text.size(); index++ )
    {
        if ( ::isspace(input_text[index]) )
        {
            CreateString(stack_for_strings, out_vect, this);
            continue;
        }
        switch (input_text[index]) {
        case '<':
            CreateString(stack_for_strings, out_vect, this);
            if (index + 1 < input_text.size())
            {
                if (input_text[index+1] == '/')
                {
                    out_vect.push_back(CreteToken(TOKEN_TYPE::starte, std::string("</")));
                    index++;
                }
                else if (input_text[index+1] == '?')
                {
                    out_vect.push_back(CreteToken(TOKEN_TYPE::start, std::string("<?")));
                    index++;
                }else
                    out_vect.push_back(CreteToken(TOKEN_TYPE::startnq, std::string("<")));

            }else
                 out_vect.push_back(CreteToken(TOKEN_TYPE::startnq, std::string("<")));
            break;
        case '>':
            CreateString(stack_for_strings, out_vect, this);

            out_vect.push_back(CreteToken(TOKEN_TYPE::startnq, std::string(">")));
            break;
        case '/':
            CreateString(stack_for_strings, out_vect, this);

            if (index + 1 < input_text.size())
            {
                if (input_text[index+1] == '>')
                {
                    out_vect.push_back(CreteToken(TOKEN_TYPE::ende, std::string("/>")));
                    index++;
                }
                else if (input_text[index+1] == '?')
                {
                    out_vect.push_back(CreteToken(TOKEN_TYPE::start, std::string("?>")));
                    index++;
                }else
                    out_vect.push_back(CreteToken(TOKEN_TYPE::slash, std::string("/")));
            }
            break;
        case '?':
            CreateString(stack_for_strings, out_vect, this);

            if (index + 1 < input_text.size())
            {
                if (input_text[index+1] == '>')
                {
                    out_vect.push_back(CreteToken(TOKEN_TYPE::end, std::string("?>")));
                    index++;
                }
            }
            break;
        case '.':
            CreateString(stack_for_strings, out_vect, this);
            out_vect.push_back(CreteToken(TOKEN_TYPE::dot, std::string(".")));
            break;
        case ':':
            CreateString(stack_for_strings, out_vect, this);
            out_vect.push_back(CreteToken(TOKEN_TYPE::doubledot, std::string(":")));
            break;
        case '-':
            CreateString(stack_for_strings, out_vect, this);
            out_vect.push_back(CreteToken(TOKEN_TYPE::minus, std::string("-")));
            break;
        case '_':
            CreateString(stack_for_strings, out_vect, this);
            out_vect.push_back(CreteToken(TOKEN_TYPE::underscore, std::string("_")));
            break;
        case '\\':
            CreateString(stack_for_strings, out_vect, this);
            out_vect.push_back(CreteToken(TOKEN_TYPE::forward_slash, std::string("\\")));
            break;
        case ',':
            CreateString(stack_for_strings, out_vect, this);
            out_vect.push_back(CreteToken(TOKEN_TYPE::comma, std::string(",")));
            break;
        default:
            stack_for_strings += input_text[index];
            break;
        }
    }
}

void LexicalAnalyzer::FindMatch(const std::string& input_text, std::regex& num_regex, std::vector<std::string>& out_vect)
{
    std::smatch pieces_match;

    if (std::regex_match(input_text, pieces_match, num_regex))
    {
        for (size_t i = 0; i < pieces_match.size(); ++i)
        {
            std::ssub_match sub_match = pieces_match[i];
            out_vect.push_back(sub_match.str());
        }
    }
}

void LexicalAnalyzer::FindMatch(const std::vector<std::string>& input_text, std::regex& num_regex, std::vector<std::string>& out_vect)
{
    for (auto str: input_text)
    {
        FindMatch(str, num_regex, out_vect);
    }
}
