all:
	gcc Utils/utils.c Lexer/token.c Lexer/lexer.c Parser/ast.c Parser/parser.c CodeGeneration/visitor.c SemanticAnalysis/symbolTable.c SemanticAnalysis/analyzer.c main.c