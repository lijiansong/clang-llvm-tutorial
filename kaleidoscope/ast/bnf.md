## Backus-Naur Form
Backus-Naur Form of Kaleidoscope is shown below:
```
numberexpr ::= number

parenexpr ::= '(' expression ')'

identifierexpr ::= identifier |
                    identifier '(' expression* ')'

primary ::= identifierexpr |
            numberexpr |
            parenexpr

binoprhs ::= ('+' primary)*

expression ::= primary binoprhs

prototype ::= id '(' id* ')'

definition ::= 'def' prototype expression

toplevelexpr ::= expression

external ::= 'extern' prototype

number ::= [0-9.]+

identifier ::= [a-zA-Z][a-zA-Z0-9]*

```
