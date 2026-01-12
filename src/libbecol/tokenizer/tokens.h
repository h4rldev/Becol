
#ifndef BECOL_TOKENS
#define BECOL_TOKENS

#include "../memory/mem.h"
#include <stdbool.h>

typedef enum {
    TOKEN_ACTION,
    TOKEN_AND,
    TOKEN_AS,
    TOKEN_ASK,
    TOKEN_DO,
    TOKEN_EVERY,
    TOKEN_FOR,
    TOKEN_IDENTIFIER,
    TOKEN_IF,
    TOKEN_TELL,
    TOKEN_TO,
    TOKEN_WAIT,
    TOKEN_WITH,
    TOKEN_WHILE,
    TOKEN_VARIABLE,
    TOKEN_LEFT_CURLY_BRACKET,  // {
    TOKEN_RIGHT_CURLY_BRACKET, // }
    TOKEN_NEWLINE,             // \n
    TOKEN_PLUS,                // +
    TOKEN_MINUS,               // -
    TOKEN_FORWARD_SLASH,       // /
    TOKEN_ASTRISK,             // *
    TOKEN_PIPE,                // |
    TOKEN_DOUBLE_PIPE,         // ||
    TOKEN_AMPERSAND,           // &
    TOKEN_DOUBLE_AMPERSAND,    // &&
    TOKEN_STRING,
    TOKEN_NUMBER,
    TOKEN_FLOAT,
    TOKEN_PERCENT,           // %
    TOKEN_EQUAL,             // =
    TOKEN_GREATER_THAN,      // >
    TOKEN_LESS_THAN,         // <
    TOKEN_GREATER_OR_EQUAL,  // >=
    TOKEN_LESS_OR_EQUAL,     // <=
    TOKEN_NOTEQUAL,          // !=
    TOKEN_NOT,               // !
    TOKEN_LEFT_PARENTHESIS,  // (
    TOKEN_RIGHT_PARENTHESIS, // )
    TOKEN_EOF
} TokenType;

// Stores a token, text is only really needed for TOKEN_IDENTIFIER
typedef struct Token {
    TokenType type;
    char *text;
    void *info;
    struct Token *next;
    int addr;
} Token;

// Used to store a program's information. Used primarly for token functions to
// store state
typedef struct {
    char *text;
    int next_token_start;
    Token *next_token;
    bool at_end;
} Program;

void BecolFreeToken(Token *token);

Program *BecolGetProgram(MemoryArena *arena, char *prog);
void BecolFreeProgram(MemoryArena *arena);

Token *BecolNextToken(MemoryArena *arena, Program *prog);
Token *BecolPeekToken(Program *prog);

#endif
