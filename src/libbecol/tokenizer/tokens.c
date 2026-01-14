#include "tokens.h"
#include "../errors/errors.h"
#include "../memory/mem.h"
#include <stddef.h>
#include <string.h>

// utility file for stuff relating to Token stuff.

// Allocating tokens

// Allocating Programs / Getting a program reference
Program *BecolGetProgram(MemoryArena *arena, char *prog) {
    Program *ret = BecolArenaPushStruct(arena, Program, false);
    ret->text = BecolArenaPushArray(arena, char, strlen(prog) + 1, false);
    ret->at_end = false;
    return ret;
}

void BecolFreeProgram(MemoryArena *arena) {
    BecolArenaPop(arena, sizeof(Program));
}

Token *BecolAllocCharToken(MemoryArena *arena, char c, Program *prog,
                           TokenType type) {
    Token *tok = BecolArenaPushStruct(arena, Token, false);
    tok->text[0] = c;
    tok->text[1] = 0;
    tok->type = type;
    tok->addr = prog->next_token_start + 1;
    prog->next_token_start++;
    return tok;
}

Token *BecolAllocTwoCharToken(MemoryArena *arena, char c, char c2,
                              Program *prog, TokenType type) {
    Token *tok = BecolArenaPushStruct(arena, Token, false);
    tok->text[0] = c;
    tok->text[1] = c2;
    tok->text[2] = 0;
    tok->type = type;
    tok->addr = prog->next_token_start += 2;
    return tok;
}

// Getting tokens
Token *BecolNextToken(MemoryArena *arena, Program *prog) {
    if (!prog->at_end && prog->text[prog->next_token_start] == '\0') {
        Token *tok = BecolArenaPushStruct(arena, Token, false);
        tok->type = TOKEN_EOF;
        tok->addr = prog->next_token_start;
        prog->at_end = true;
        return tok;
    } else if (prog->at_end) {
        prog->at_end = true;
        BecolReportError(arena, BECOL_ERROR_PARSING,
                         "Tried to get token when at end of string");
        return NULL;
    }
    int type = 0; // 0 - normal, 1 - number, 2 - string, 3 - identifier /
                  // keyword, 4 - variable
    char *ptr = prog->text + prog->next_token_start;
    while (*ptr == ' ' || *ptr == '\t') {
        prog->next_token_start++;
        ptr++;
    }
    switch (ptr[0]) {
    case '\n':
        return BecolAllocCharToken(arena, ptr[0], prog, TOKEN_NEWLINE);
    case '"':
        type = 2; // string
        break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        type = 1; // number
        break;
    case '$':
        type = 4; // var
        break;
    case '{':
        return BecolAllocCharToken(arena, ptr[0], prog,
                                   TOKEN_LEFT_CURLY_BRACKET);
    case '}':
        return BecolAllocCharToken(arena, ptr[0], prog,
                                   TOKEN_RIGHT_CURLY_BRACKET);
    case '+':
        return BecolAllocCharToken(arena, ptr[0], prog, TOKEN_PLUS);
    case '-':
        return BecolAllocCharToken(arena, ptr[0], prog, TOKEN_MINUS);
    case '*':
        return BecolAllocCharToken(arena, ptr[0], prog, TOKEN_ASTRISK);
    case '/':
        return BecolAllocCharToken(arena, ptr[0], prog, TOKEN_FORWARD_SLASH);
    case '%':
        return BecolAllocCharToken(arena, ptr[0], prog, TOKEN_PERCENT);
    case '=':
        return BecolAllocCharToken(arena, ptr[0], prog, TOKEN_EQUAL);
    case '|':
        if (ptr[1] == '|')
            return BecolAllocTwoCharToken(arena, ptr[0], ptr[1], prog,
                                          TOKEN_DOUBLE_PIPE);
        return BecolAllocCharToken(arena, ptr[0], prog, TOKEN_PIPE);
    case '&':
        if (ptr[1] == '&')
            return BecolAllocTwoCharToken(arena, ptr[0], ptr[1], prog,
                                          TOKEN_DOUBLE_AMPERSAND);
        return BecolAllocCharToken(arena, ptr[0], prog, TOKEN_AMPERSAND);
    case '>':
        if (ptr[1] == '=')
            return BecolAllocTwoCharToken(arena, ptr[0], ptr[1], prog,
                                          TOKEN_GREATER_OR_EQUAL);
        return BecolAllocCharToken(arena, ptr[0], prog, TOKEN_GREATER_THAN);
    case '<':
        if (ptr[1] == '=')
            return BecolAllocTwoCharToken(arena, ptr[0], ptr[1], prog,
                                          TOKEN_LESS_OR_EQUAL);
        return BecolAllocCharToken(arena, ptr[0], prog, TOKEN_LESS_THAN);
    case '!':
        if (ptr[1] == '=')
            return BecolAllocTwoCharToken(arena, ptr[0], ptr[1], prog,
                                          TOKEN_NOTEQUAL);
        return BecolAllocCharToken(arena, ptr[0], prog, TOKEN_NOT);
    case '(':
        return BecolAllocCharToken(arena, ptr[0], prog, TOKEN_LEFT_PARENTHESIS);
    case ')':
        return BecolAllocCharToken(arena, ptr[0], prog,
                                   TOKEN_RIGHT_PARENTHESIS);
    default:
        type = 3; // identifier / keyword
        break;
    }
    if (type == 2) { // string
        int i = 1;
        for (; ptr[i] != '"'; i++) {
        }
        Token *tok = BecolArenaPushStruct(arena, Token, false);
        tok->type = TOKEN_STRING;
        memcpy(tok->text, prog->text + prog->next_token_start + 1, i - 1);
        tok->text[i - 1] = 0;
        tok->addr = prog->next_token_start;
        prog->next_token_start += i + 1;
        return tok;
    } else if (type == 1) { // number
        bool is_float = false;
        int left = 0;  // left of .
        int right = 0; // right of .
        int i = 0;
        char c = ptr[0];
        while ((c >= '0' && c <= '9') || c == '.') {
            if (c == '.')
                is_float = true;
            else {
                if (is_float) {
                    right *= 10;
                    right += c - '0';
                } else {
                    left *= 10;
                    left += c - '0';
                }
            }
            c = ptr[++i];
        }
        Token *tok = BecolArenaPushStruct(arena, Token, false);
        tok->type = is_float ? TOKEN_FLOAT : TOKEN_NUMBER;
        memcpy(tok->text, ptr, i);
        tok->text[i] = 0;
        if (is_float) {
            double num = 0;
            num = right;
            while (num >= 1)
                num /= 10;
            num += left;
            memcpy(tok->info, &num, sizeof(num));
        } else {
            memcpy(tok->info, &left, sizeof(left));
        }
        tok->addr = prog->next_token_start;
        prog->next_token_start += i;
        return tok;
    } else if (type == 3) { // ident / keyword
        int i = 0;
        while ((ptr[i] >= 'A' && ptr[i] <= 'Z') ||
               (ptr[i] >= 'a' && ptr[i] <= 'z') ||
               (ptr[i] >= '0' && ptr[i] <= '9') || ptr[i] == '-' ||
               ptr[i] == '_')
            i++;

        Token *tok = BecolArenaPushStruct(arena, Token, false);
        memcpy(tok->text, ptr, i);
        tok->text[i] = 0;
        tok->type = TOKEN_IDENTIFIER;
        char *capital = BecolArenaPushArray(arena, char, i, false);
        for (int i = 0; capital[i] != 0; i++)
            capital[i] = capital[i] >= 'a' ? capital[i] - 32 : capital[i];
        if (strcmp(capital, "ACTION") == 0)
            tok->type = TOKEN_ACTION;
        else if (strcmp(capital, "AND") == 0)
            tok->type = TOKEN_AND;
        else if (strcmp(capital, "AS") == 0)
            tok->type = TOKEN_AS;
        else if (strcmp(capital, "ASK") == 0)
            tok->type = TOKEN_ASK;
        else if (strcmp(capital, "DO") == 0)
            tok->type = TOKEN_DO;
        else if (strcmp(capital, "EVERY") == 0)
            tok->type = TOKEN_EVERY;
        else if (strcmp(capital, "FOR") == 0)
            tok->type = TOKEN_FOR;
        else if (strcmp(capital, "IF") == 0)
            tok->type = TOKEN_IF;
        else if (strcmp(capital, "TELL") == 0)
            tok->type = TOKEN_TELL;
        else if (strcmp(capital, "TO") == 0)
            tok->type = TOKEN_TO;
        else if (strcmp(capital, "WAIT") == 0)
            tok->type = TOKEN_WAIT;
        else if (strcmp(capital, "WITH") == 0)
            tok->type = TOKEN_WITH;
        else if (strcmp(capital, "WHILE") == 0)
            tok->type = TOKEN_WHILE;
        BecolArenaPop(arena, i);
        tok->addr = prog->next_token_start;
        prog->next_token_start += i;
        return tok;
    } else if (type == 4) { // var
        int i = 1;
        while ((ptr[i] >= 'A' && ptr[i] <= 'Z') ||
               (ptr[i] >= 'a' && ptr[i] <= 'z') ||
               (ptr[i] >= '0' && ptr[i] <= '9') || ptr[i] == '-' ||
               ptr[i] == '_')
            i++;
        Token *tok = BecolArenaPushStruct(arena, Token, false);
        memcpy(tok->text, ptr + 1, i);
        tok->text[i] = 0;
        tok->type = TOKEN_VARIABLE;
        tok->addr = prog->next_token_start;
        prog->next_token_start += i;
        return tok;
    }
    BecolReportError(arena, BECOL_ERROR_PARSING,
                     "Reached end of program without finding a token");
    return NULL;
}

Token *BecolPeekToken(Program *prog);
