%%
%{
#include <errno.h>
#include <stdio.h>
#include <tokens.h>
%}
<<EOF>>		return TOK_EOF;
EOF			return TOK_EOF;
INCLUDE		return TOK_INCLUDE;
ENDB		return TOK_ENDB;
STORYB		return TOK_STORYB;
SCENEB		return TOK_SCENEB;
LOOKB		return TOK_LOOKB;
MOVEB		return TOK_MOVEB;
TALKB		return TOK_TALKB;
DIAB		return TOK_DIAB;
DIAOPTB		return TOK_DIAOPTB;
USEB		return TOK_USEB;
WINB		return TOK_WINB;
DEATHB		return TOK_DEATHB;
TITLE		return TOK_ST_TITLE;
ISCENE		return TOK_ST_ISCENE;
PROMPT		return TOK_ST_PROMPT;
HELP		return TOK_ST_HELP;
NAME		return TOK_SC_NAME;
MOVE		return TOK_SC_MOVE;
LOOK		return TOK_SC_LOOK;
\"[ a-zA-Z0-9_]+\"	return TOK_STRING;
[ \t\n]+	/* Eliminate whitespace */
%%

int yywrap () {
	return 0;
}

int parseFile (const char *pszFileName) {
	FILE* fp = fopen(pszFileName, "r");
	if (fp == NULL) {
		perror("fopen");
		return 1;
	}

	yyin = fp;
	int token;
	do {
		token = yylex();
		printf("Found token: \"%s\" (%d).\n", yytext, token);
		//if (feof(yyin)) break;
	} while (token != 0);

	fclose(fp);
	return 0;
}
