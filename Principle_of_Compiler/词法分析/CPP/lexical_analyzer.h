#pragma once
#include "dfa.h"
#include "token.h"
#include <string>
using namespace std;
const int MAX_TOKEN = 10000;

class Lexical_Analyzer
{
public:
    struct TokenCount
    {
        TokenType type;
        int cnt;
        bool operator< (const TokenCount& rhs)
        {
            return cnt > rhs.cnt;
        }
    };
    Lexical_Analyzer(const string);
    void Run();
    void Print_Table();
    string code;
    int pointer, tot_len, line, col, tot_token, tot_error;
    DFA dfa;
    Token symbol_table[MAX_TOKEN];
    Token error_symbol[MAX_TOKEN];
    TokenCount token_number[200];
};

