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
<program> ::= <function> '\0'
<function> ::= 'int' [_a-zA-Z]([_a-zA-Z0-9])* '(' ')' '{' <body> '}'
<body> ::= <return>
<return> ::= 'return' <expression> ';'
<expression> ::= <orop>
<orop> ::= <andop> ( '||' <andop> )*
<andop> ::= <equaliyop> ( '&&' <equaliyop> )*
<equaliyop> ::= <relationop> ( ( '==' | '!=' ) <relationop> )*
<relationop> ::= <addsubop> ( ( '>' | '<' | '<=' | '>=' ) <addsubop> )*
<addsubop> ::= <multdivop> ( ( '+' | '-' ) <multdivop> )*
<multdivop> ::= <unaryop> ( ( '*' | '/' ) <unaryop> )*
<unaryop> ::= ( ( '~' | '!' | '-' ) <unaryop> ) | ( '(' <expression> ')' ) | ([0-9])*
```

### Semantic Rules

1. Checks for the `main` function declaration