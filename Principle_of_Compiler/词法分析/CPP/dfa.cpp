#include "dfa.h"
#define FOR(_i,_a,_b) for (int _i = (_a) ; _i <= (_b) ; _i++)
#define FOR_LETTER(_i) for (int _i = 'A' ; _i <= 'z' ; _i != 'Z' ? (_i++) : (_i = 'a'))
#define FOR_DIGITAL(_i) for (int _i = '0' ; _i <= '9' ; _i++)
#define FOR_OTHER(_i) for (int _i = 0 ; _i < MAXMINE_CHAR ; _i++)
#define TO_END(_i, _out) FOR_OTHER(_j) mealy[_i][_j] = Mealy(STATE_T, _out, 1);

DFA::DFA()
{
    /*进行状态转移路径的初始化*/
    for (int i = 0; i < MAXMINE_STATE; i++)
        FOR_OTHER(j) mealy[i][j] = Mealy(STATE_T, UNKNOWN, 0);

    /*Begin with letter*/
    FOR_LETTER(j)
        mealy[STATE_S][j] = 1;
    TO_END(1, IDENTIFIER);
    FOR_LETTER(j)
        mealy[1][j] = 1;
    FOR_DIGITAL(j)
        mealy[1][j] = 1;

    /*begin with digital*/
    FOR_DIGITAL(j)
        mealy[STATE_S][j] = 2;

    TO_END(2, CONSTANT_INT);
    FOR_DIGITAL(j)
        mealy[2][j] = 2;
    mealy[2]['.'] = 3;
    mealy[2]['e'] = 5;
    mealy[2]['E'] = 5;

    TO_END(3, INCOMPLETE_NUMBERIC_ERROR);
    FOR_DIGITAL(j)
        mealy[3][j] = 4;

    TO_END(4, CONSTANT_REAL)
    FOR_DIGITAL(j)
        mealy[4][j] = 4;
    mealy[4]['e'] = 5;
    mealy[4]['E'] = 5;

    TO_END(5, INCOMPLETE_NUMBERIC_ERROR);
    FOR_DIGITAL(j)
        mealy[5][j] = 6;
    mealy[5]['+'] = 7;
    mealy[5]['-'] = 7;

    TO_END(6, CONSTANT_SCIENTIFIC);
    FOR_DIGITAL(j)
        mealy[6][j] = 6;

    FOR_DIGITAL(j)
        mealy[7][j] = 6;

    /*begin with signal*/
    mealy[STATE_S]['~'] = 8;
    TO_END(8, BITWISE_NOT);

    mealy[STATE_S]['!'] = 9;
    TO_END(9, NOT);
    mealy[9]['='] = 10;
    TO_END(10, NOT_EQUAL);

    mealy[STATE_S]['#'] = 11;
    FOR_OTHER(j)
        mealy[11][j] = 11;
    mealy[11]['\n'] = Mealy(STATE_T, IGNORE, 0);
    mealy[11]['#'] = 27;
    TO_END(27, CONNECT);

    mealy[STATE_S]['%'] = 12;
    TO_END(12, MOD);
    mealy[12]['='] = 13;
    TO_END(13, MOD_ASSIGN);

    mealy[STATE_S]['&'] = 16;
    TO_END(16, BITWISE_AND);
    mealy[16]['='] = 17;
    mealy[16]['&'] = 18;
    TO_END(17, BITWISE_AND_ASSIGN);
    TO_END(18, AND);
    mealy[18]['='] = 19;
    TO_END(19, BITWISE_AND)

    mealy[STATE_S]['*'] = 20;
    TO_END(20, STAR);

    mealy[STATE_S]['('] = 21;
    TO_END(21, LEFT_PARENTHESIS);

    mealy[STATE_S][')'] = 22;
    TO_END(22, RIGHT_PARENTHESIS);

    mealy[STATE_S]['-'] = 23;
    TO_END(23, SUB);
    mealy[23]['-'] = 24;
    mealy[23]['='] = 25;
    mealy[23]['>'] = 26;
    TO_END(24, SELF_DECREASE);
    TO_END(25, SUB_ASSIGN);
    TO_END(26, ARROW);

    mealy[STATE_S]['+'] = 28;
    TO_END(28, ADD);
    mealy[28]['+'] = 29;
    mealy[28]['='] = 30;
    TO_END(29, SELF_INCREASE);
    TO_END(30, ADD_ASSIGN);

    mealy[STATE_S]['='] = 31;
    TO_END(31, ASSIGN);
    mealy[31]['='] = 32;
    TO_END(32, EQUAL);

    mealy[STATE_S]['['] = 33;
    TO_END(33, LEFT_BRACKET);
    mealy[STATE_S][']'] = 34;
    TO_END(34, RIGHT_BRACKET);

    mealy[STATE_S]['{'] = 35;
    TO_END(35, LEFT_BRACE);
    mealy[STATE_S]['}'] = 36;
    TO_END(36, RIGHT_BRACE);

    mealy[STATE_S]['|'] = 37;
    TO_END(37, BITWISE_OR);
    mealy[37]['|'] = 38;
    mealy[37]['='] = 39;
    TO_END(38, OR);
    TO_END(39, BITWISE_OR_ASSIGN);

    mealy[STATE_S][';'] = 40;
    TO_END(40, SEMICOLON);

    mealy[STATE_S][':'] = 41;
    TO_END(41, COLON);

    mealy[STATE_S]['\''] = 42;
    FOR_OTHER(j)
        mealy[42][j] = 45;
    mealy[42]['\\'] = 43;
    mealy[42]['\n'] = Mealy(STATE_T, UNCLOSED_CHAR_ERROR, 0);
    mealy[42][1] = Mealy(STATE_T, UNCLOSED_CHAR_ERROR, 0);
    FOR_OTHER(j)
        mealy[43][j] = 63;
    mealy[43]['0'] = 44;
    mealy[43]['n'] = 44;
    mealy[43]['r'] = 44;
    mealy[43]['t'] = 44;
    mealy[43]['v'] = 44;
    mealy[43]['a'] = 44;
    mealy[43]['b'] = 44;
    mealy[43]['f'] = 44;
    mealy[43]['\''] = 44;
    mealy[43]['\"'] = 44;
    mealy[43]['\\'] = 44;
    FOR_OTHER(j)
        mealy[44][j] = 64;
    mealy[44]['\''] = Mealy(STATE_T, CONSTANT_CHAR, 0);
    FOR_OTHER(j)
        mealy[45][j] = 64;
    mealy[45]['\''] = Mealy(STATE_T, CONSTANT_CHAR, 0);
    FOR_OTHER(j)
        mealy[63][j] = 64;
    mealy[63]['\''] = Mealy(STATE_T, ESCAPE_CHARACTER_NOT_EXIST, 0);
    FOR_OTHER(j)
        mealy[64][j] = 64;
    mealy[64]['\''] = Mealy(STATE_T, TOO_MUCH_CHARACTER, 0);
    mealy[64]['\n'] = Mealy(STATE_T, UNCLOSED_CHAR_ERROR, 0);
    mealy[64][1] = Mealy(STATE_T, UNCLOSED_CHAR_ERROR, 0);

    mealy[STATE_S]['\"'] = 46;
    FOR_OTHER(j)
        mealy[46][j] = 46;
    mealy[46]['\n'] = Mealy(STATE_T, UNFINISHED_STRING_ERROR, 0);
    mealy[46][1] = Mealy(STATE_T, UNFINISHED_STRING_ERROR, 0);
    mealy[46]['\"'] = Mealy(STATE_T, CONSTANT_STRING, 0);

    mealy[STATE_S]['<'] = 47;
    TO_END(47, LESS);
    mealy[47]['='] = 48;
    mealy[47]['<'] = 49;
    TO_END(48, LESS_OR_EQUAL);
    TO_END(49, SHL);
    mealy[49]['='] = 50;
    TO_END(50, SHL_ASSIGN);

    mealy[STATE_S]['>'] = 51;
    TO_END(51, GREATER);
    mealy[51]['='] = 52;
    mealy[51]['>'] = 53;
    TO_END(52, GREATER_OR_EQUAL);
    TO_END(53, SHR);
    mealy[53]['='] = 54;
    TO_END(54, SHR_ASSIGN);

    mealy[STATE_S]['/'] = 55;
    TO_END(55, DIV);
    mealy[55]['*'] = 57;
    mealy[55]['/'] = 56;
    mealy[55]['='] = 59;
    FOR_OTHER(j)
        mealy[56][j] = 56;
    mealy[56]['\n'] = Mealy(STATE_T, LINE_COMMENT, 0);
    FOR_OTHER(j)
        mealy[57][j] = 57;
    mealy[57]['*'] = 58;
    mealy[57][1] = Mealy(STATE_T, UNCLOSED_BLOCK_COMMENT_ERROR, 0);
    FOR_OTHER(j)
        mealy[58][j] = 57;
    mealy[58]['*'] = 58;
    mealy[58]['/'] = Mealy(STATE_T, BLOCK_COMMENT, 0);
    mealy[58][1] = Mealy(STATE_T, UNCLOSED_BLOCK_COMMENT_ERROR, 0);
    TO_END(59, DIV_ASSIGN);

    mealy[STATE_S][','] = 60;
    TO_END(60, COMMA);

    mealy[STATE_S]['.'] = 61;
    TO_END(61, DOT);

    mealy[STATE_S]['?'] = 62;
    TO_END(62, QUESTION);

    mealy[STATE_S]['\n'] = Mealy(STATE_T, LINE_BREAK);
    mealy[STATE_S][' '] = Mealy(STATE_T, IGNORE);
    mealy[STATE_S][1] = Mealy(STATE_T, IGNORE);
    mealy[STATE_S]['$'] = Mealy(STATE_T, ILLEGAL_CHAR_ERROR);
    mealy[STATE_S]['@'] = Mealy(STATE_T, ILLEGAL_CHAR_ERROR);
    state = STATE_S;
}

Mealy DFA::Input(const char c)
{
    if (!(c >= 0 && c < 128))
    {
        return Mealy(0, ILLEGAL_CHAR_ERROR, 0);
    }
    Mealy out = mealy[state][c];
    state = mealy[state][c].next;
    if (state == STATE_T)
        state = STATE_S;
    return out;
}

DFA::~DFA()
{

}
