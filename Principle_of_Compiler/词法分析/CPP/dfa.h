#pragma once
#include "token.h"
#include <string>
using namespace std;
const int MAXMINE_CHAR = 128;
const int MAXMINE_STATE = 100;
const int STATE_S = 0;
const int STATE_T = 99;
struct Mealy
{
    int next;
    TokenType output;
    int trace_back;
    void operator=(const int _next) { next = _next; output = UNKNOWN; trace_back = 0; };
    Mealy() : next(0), output(UNKNOWN) {};
    Mealy(const int _next, const TokenType _output = UNKNOWN, const int _trace_back = 0) :
        next(_next), output(_output), trace_back(_trace_back) {};
};
class DFA
{
public:
    DFA();
    ~DFA();
    Mealy Input(const char);
private:
    int state;
    Mealy mealy[MAXMINE_STATE][MAXMINE_CHAR];

};

