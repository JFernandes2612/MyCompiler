# MyCompiler - Compiler experiments

This repository show my experience building a compiler from scratch in C.

## Dependencies

`gcc`

`make`

## How to run

`make` to compile the project. Then run the generated executable with a file as the parameter. 

Example: `.\a input.txt` should generate a new `input` executable which you can run by entering `.\input`.

## Development status

In very early development. Only simple operations fully implemented in the current moment.

### Syntax grammar

```html
<program> ::= <function> "\0"
<function> ::= "int" <id> "(" ")" "{" <body> "}"
<body> ::= <return>
<return> ::= "return" <expression> ";"
<expression> ::= <addsubop>
<addsubop> ::= <multdivop> ( ( '+' | '-' ) <multdivop> )*
<multdivop> ::= <unaryop> ( ( '*' | '/' ) <unaryop> )*
<unaryop> ::= ( ( '~' | '!' | '-' ) <unaryop> ) | ( '(' <expression> ')' ) | ([0-9])*
```

### Semantic Rules

1. Checks for 'main' function declaration