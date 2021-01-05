%{
    int tot_token = 0;
    int charCounter=0;
    int col =1,line=1;
    void addline();
    void addcol(int);
    void clearcol();
    void addWord();
%}
letter                 [A-Za-z_]
digit                  [0-9]
LINE_COMMENT           (\/\/.*\n)
CONSTANT_CHAR          '[^']'|'\\[0nrtvabf\'\"\\]'
CONSTANT_STRING        \"[^'"\]*\"

IDENTIFIER             {letter}({letter}|{digit})*
KEYWORD                "char"|"int"|"long"|"float"|"double"|"void"|"unsigned"|"signed"|"const"|"static"|"extern"|"struct"|"union"|"typedef"|"sizeof"|"if"|"else"|"do"|"while"|"for"|"switch"|"case"|"default"|"continue"|"break"|"goto"
CONSTANT_INT           {digit}+
CONSTANT_REAL          {digit}*(\.{digit}+)?(e|E[+\-]?{digit}+)?
SIGNAL                 "="|"+"|"++"|"+="|"-"|"--"|"-="|"*="|"/"|"/="|"%"|"%="|"&"|"&="|"|"|"|="|"^"|"^="|"~"|"<<"|"<<="|">>"|">>="|"&&"|"&&="|"||"|"||="|"!"|"<"|"<="|"=="|"!="|">"|">="|"##"|"?"|","|":"|";"|"."|"*"|"->"|"("|")"|"["|"]"|"{"|"}"
LINE_BREAK             \n

%%

{LINE_COMMENT} {
    printf ("< LINE_COMMENT , \"");
    int i = 0;
	while (i < yyleng - 1)
        printf ("%c", yytext[i++]);
    printf ("\" > in ( %d , %d)\n", line, col);
	addcol(yyleng);
	addWord();
}

{CONSTANT_CHAR}  {
    printf("< LINE_COMMENT , %s > in ( %d , %d)\n",yytext, line, col);
    addcol(yyleng);
    addWord();
}
{CONSTANT_STRING}  {
    printf("< LINE_STRING , %s > in ( %d , %d)\n",yytext, line, col);
    addcol(yyleng);
    addWord();
}
{KEYWORD} {
    printf("< KEYWORD , \"%s\" > in ( %d , %d)\n",yytext, line, col);
    addcol(yyleng);
    addWord();
}
{IDENTIFIER}    {
    printf("< IDENTIFIER , \"%s\" > in ( %d , %d)\n",yytext, line, col);
    addcol(yyleng);
    addWord();
}

{CONSTANT_INT}   {
    printf("< CONSTANT_INT , \"%s\" > in ( %d , %d)\n",yytext, line, col);
    addcol(yyleng);
    addWord();
}
{CONSTANT_REAL} {
    printf("< CONSTANT_REAL , \"%s\" > in ( %d , %d)\n",yytext, line, col);
    addcol(yyleng);
    addWord();
}
{SIGNAL}  {
    printf("< SIGNAL , \"%s\" > in ( %d , %d)\n",yytext, line, col);
    addcol(yyleng);
    addWord();
}

{LINE_BREAK} {
    addline();
	clearcol();
}

%%
int main(void)
{   
    printf ("开始分析\n");
    yylex();
    printf("统计结果： 共 %d 行, 共 %d 个单词\n",line, tot_token);
    return 0;
}
int yywrap()    {
    return 1;
}

void addline()   {
    line ++;
}
void addcol(int cnt) {
    col += cnt;
}
void clearcol()  {
    col = 1;
}
void addWord()   {
    tot_token ++;
}
