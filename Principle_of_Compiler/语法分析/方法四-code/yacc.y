%{
#include <stdio.h>
int yylex();
void yyerror(const char* msg){}
%}

%token NUM

%left '+' '-'
%left '*' '/'

%%
S   :   E           {printf("Ans = %d\n", $1);}
    ;
E   :   E '+' T     {$$ = $1+$3; printf("E->E+T\n");}
    |   E '-' T     {$$ = $1-$3; printf("E->E-T\n");}
    |   T           {$$ = $1; printf("E->T\n");}
    ;
T   :   T '*' F     {$$ = $1*$3; printf("T->T*F\n");}
    |   T '/' F     {$$ = $1/$3; printf("T->T/F\n");}
    |   F           {$$ = $1; printf("T->F\n");}
    ;
F   :   '(' E ')'   {$$ = $2; printf("F->(E)\n");}
    |   NUM         {$$ = $1; printf("F->NUM\n");}

%%
int main(){
    return yyparse();
}