%%
<<EOF>>	return 0;
STORY	return 1;
END_STORY	return 2;
%%

int yywrap () {
	return 1;
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
