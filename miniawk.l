%{
#include <stdio.h> /* just for printf */

/*
struct token {
    char * v;
    int    t; 
};
*/

%}

%%

[ \t\n]    ;  /* Skip  whitespaces */
^/         { BEGIN action; }
    <action>[A-Za-z\.\*\{\}\(\) \t\n]+ {
     
    return TOKEN(pattern_str);
    }
<action>/        return {
"{"      return TOKEN(T_LEFT_BRACE)
"}"      return TOKEN(T_RIGHT_BRACE)
"("      return TOKEN(T_LEFT_parentheses)
")"      return TOKEN(T_RIGHT_parentheses)
"if"     return TOKEN(T_KEYWORD_IF)
"else"
"for"
"="
"=="
"<"
"<="
">="
">"
"!="
%%
