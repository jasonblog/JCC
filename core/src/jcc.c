#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "lex.h"
#include "global_var.h"
#include "token.h"
#include "jcc.h"

int main(int argc, char **argv)
{
	cur_File = fopen(argv[1], "r");
	if (!cur_File) {
		printf("Opening JCC source file failed!\n");
		return -1;
	}

	init();

	getCHAR();
	do {
		getToken();
		color_token(LEX_NORMAL);
	} while (cur_token != tk_EOF);

	printf("Total code line number: %d lines\n", cur_line_num);

	cleanup();
	fclose(cur_File);
	printf("Lexical Analysis SUCCESS! File:%s \n", argv[1]);
	return 0;
}

void init(void)
{
	cur_line_num = 1;
	init_lex();
}

void cleanup(void)
{
	printf("\ntkTable.count = %d \n", tkTable.count);

	for (uint32_t i = tk_IDENT; i < tkTable.count; i++) {
		free(tkTable.data[i]);
	}
	free(tkTable.data);
}


#define C_NORMAL      "\x1B[0m"
#define C_RED         "\x1B[31;1m"
#define C_GREEN       "\x1B[32;1m"
#define C_YELLOW      "\x1B[33;1m"
#define C_BLUE        "\x1B[34;1m"
#define C_MAGENTA     "\x1B[35;1m"
#define C_CYAN        "\x1B[36;1m"
#define C_WHILE_DARK  "\x1B[37;1m"

void color_token(int lex_state)
{
	switch (lex_state) {
	case LEX_NORMAL: {
			if (cur_token >= tk_IDENT) {
				printf("%s", C_CYAN);
			} else if (cur_token >= kw_CHAR) {
				printf("%s", C_GREEN);
			} else if (cur_token >= tk_cINT) {
				printf("%s", C_YELLOW);
			} else {
				printf("%s", C_RED);
			}

			printf("%s", get_tkSTR(cur_token));
			break;
		}
	case LEX_SEPARATOR: {
			printf("%c", cur_CHAR);
			break;
		}
	}
}



