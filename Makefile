all:
	gcc -W Utils/utils.c Lexer/token.c Lexer/lexer.c Parser/ast.c Parser/parser.c main.c