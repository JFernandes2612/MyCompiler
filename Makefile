all:
	gcc Utils/utils.c Lexer/token.c Lexer/lexer.c Parser/ast.c Parser/parser.c CodeGeneration/visitor.c main.c