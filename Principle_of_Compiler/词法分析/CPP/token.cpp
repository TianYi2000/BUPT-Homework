#include "token.h"

Token::Token()
{
    type = UNKNOWN;
    linePos = 0;
    colPos = 0;

}

Token::Token(const int _linePos, const int _colPos)
{
    linePos = _linePos;
    colPos = _colPos;
}

Token::Token(const int _linePos, const int _colPos, const TokenType _type)
{
    linePos = _linePos;
    colPos = _colPos;
    type = _type;
}


Token::~Token()
{

}
