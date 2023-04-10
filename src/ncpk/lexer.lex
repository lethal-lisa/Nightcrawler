%%
%{
#include <errno.h>
#include <stdio.h>
#include <tokens.h>
%}
<<EOF>>		return TOK_EOF;
EOF			return TOK_EOF;
INCLUDE		return TOK_INCLUDE;
STORY		return TOK_STORYB;
ENDSTORY	return TOK_ENDSTORYB;
SCENE		return TOK_SCENEB;
ENDSCENE	return TOK_ENDSCENEB;
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
		printf("Found token: %d.\n", token);
	} while (token != 0);

	fclose(fp);
	return 0;
}
