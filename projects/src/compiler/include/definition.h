#ifndef DEFINITION_H
#define DEFINITION_H

/* defenition */
enum Token_type
{
    NOTOKEN,
    KEYWORD,
    SYMBOL,
    IDENTIFIER,
    INT_CONST,
    STRING_CONST
};

typedef struct Token Token;

struct Token
{
    int row;
    char *word;
    Token *next;
    enum Token_type token_type;
};

enum Kind
{
    NoKind,
    Static,
    Field,
    Argument,
    Var
};

typedef struct Symbol Symbol;

struct Symbol
{
    char *name;
    char *type;
    enum Kind kind;
    int index;
    Symbol *next;
};

typedef struct Symbol_table Symbol_table;

struct Symbol_table
{
    Symbol *class;
    Symbol *subroutine;
};

enum Segment
{
    CONST,
    ARG,
    LOCAL,
    STATIC,
    THIS,
    THAT,
    POINTER,
    TEMP
};

enum Command
{
    ADD,
    SUB,
    NEG,
    EQ,
    GT,
    LT,
    AND,
    OR,
    NOT
};

enum Subroutine_Kind
{
    FUNCTION,
    CONSTRUCTOR,
    METHOD,
};

#define FILE_EXT ".vm"

#define KEYWORD_COUNT 21
#define SYMBOL_COUNT 19
#define SEGMENT_COUNT 8
#define COMMAND_COUNT 9

#define MIN_INT 0
#define MAX_INT 32767

/* globals */
extern const char *KEYWORDS[KEYWORD_COUNT];
extern const char SYMBOLS[SYMBOL_COUNT];
extern const char *SEGMENT[SEGMENT_COUNT];
extern const char *COMMAND[COMMAND_COUNT];
extern int LABEL_NUM;

#endif /* DEFINITION_H */
