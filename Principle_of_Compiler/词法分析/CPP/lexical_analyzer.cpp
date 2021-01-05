#include "lexical_analyzer.h"
#include <map>
#include <iostream>
#include <algorithm>
using namespace std;
#define DEBUG

Lexical_Analyzer::Lexical_Analyzer(const string _code)
{
    code = _code;
    code += char(1);
    pointer = 0;
    tot_len = code.length();
    line = 1;
    col = 1;
    tot_error = 0;
    tot_token = 1;
    symbol_table[tot_token] = Token(1, 1);
}

void Lexical_Analyzer::Run()
{
    while (pointer < tot_len)
    {
        int token_fin = 0;
        Mealy out = dfa.Input(code[pointer]);
        TokenType tokentype = out.output;

        if (tokentype != UNKNOWN)
        {
            if (tokentype == IGNORE) {
                symbol_table[tot_token] = Token(line, col + 1);
            }
            else {
                switch (tokentype)
                {
                case LINE_BREAK:
                    line++;
                    col = 0;
                    symbol_table[tot_token] = Token(line, col + 1);
                    break;
                case IDENTIFIER:
                    if (ReservedWord.find(symbol_table[tot_token].val) == ReservedWord.end())
                    {
                        symbol_table[tot_token].type = tokentype;
                    }
                    else
                        symbol_table[tot_token].type = ReservedWord[symbol_table[tot_token].val];
                    token_fin = 1;
                    break;
                case INCOMPLETE_NUMBERIC_ERROR:
                case ILLEGAL_CHAR_ERROR:
                case UNCLOSED_BLOCK_COMMENT_ERROR:
                case UNCLOSED_CHAR_ERROR:
                case ESCAPE_CHARACTER_NOT_EXIST:
                case TOO_MUCH_CHARACTER:
                case UNFINISHED_STRING_ERROR:
                    error_symbol[++tot_error] = Token(symbol_table[tot_token].linePos,
                                                      symbol_table[tot_token].colPos,
                                                      tokentype);
                    if (code[pointer] == '\n')
                    {
                        line++;
                        col = 0;
                    }
                    symbol_table[tot_token] = Token(line, col + 1);
                    break;
                default:
                    symbol_table[tot_token].type = tokentype;
                    token_fin = 1;
                    break;
                }

            }
        }
        else
        {
            symbol_table[tot_token].val += code[pointer];
        }
#ifdef DEBUG
        std::cout << "trace_back = " << out.trace_back << std::endl;
#endif // DEBUG
        pointer++;
        col++;
        pointer -= out.trace_back;
        col -= out.trace_back;
        if (token_fin == 1)
        {
            tot_token++;
            symbol_table[tot_token] = Token(line, col);
        }
    }
    tot_token--;
    tot_len--;
}

void Lexical_Analyzer::Print_Table()
{
    cout << "统计结果："
        << "共 " << line << " 行 , "
        << "共 " << tot_error << " 个错误 , "
        << "共 " << tot_token << " 个单词" << endl
        << "统计结果如下:" << endl ;

    if (tot_error > 0)
    {
        cout << endl << "错误:" << endl;
        for (int i = 1; i <= tot_error; i++)
        {
            cout << TypeName[error_symbol[i].type] << " in "
                << "( " << error_symbol[i].linePos << " , "
                << error_symbol[i].colPos << " )"
                << endl;
        }
    }


    cout << endl << "符号表:" << endl;
    for (TokenType i = UNKNOWN; i <= ILLEGAL_CHAR_ERROR; i = TokenType(i + 1))
    {
        token_number[i].type = i;
        token_number[i].cnt = 0;
    }

    for (int i = 1; i <= tot_token; i++)
        token_number[symbol_table[i].type].cnt++;
    sort(token_number, token_number + 199);
    for (int i = 0; i < 200 && token_number[i].cnt > 0; i++)
        cout << TypeName[token_number[i].type] << " : " << token_number[i].cnt << endl;
    cout << "符号表为:" << endl;
    for (int i = 1; i <= tot_token; i++)
    {
        cout	<< "<"
            << TypeName[symbol_table[i].type] << " , "
            << '"' << symbol_table[i].val << "\" > in "
            << "( " << symbol_table[i].linePos << " , "
            << symbol_table[i].colPos << " )"
            << endl;
    }
}
