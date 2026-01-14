#include "tokenizer.h"
#include "../memory/mem.h"
#include "asttree.h"
#include "tokens.h"
#include <stddef.h>

ASTTreeNode *BecolGenASTTree(MemoryArena *arena
                             // , char *prog_text
) {
    // Program *prog = BecolGetProgram(arena, prog_text);

    ASTTreeNode *root = BecolAllocASTNode(arena);
    root->type = BECOL_AST_PROGRAM;

    BecolFreeProgram(arena);

    return root;
}

//  ENBF:
//  TODO: This does not contain loops cause that is something we can do later
//
//  Program
//      : {statement}
//      ;
//  statement
//      : 'ask' identifer 'to' identifer value_decl '\n'
//      | 'tell' identifer 'to' identifer value_decl '\n'
//      | 'ask' identifer 'for' identifer '\n'
//      | 'tell' identifer 'that' identifer 'is' expr {'and' identifer 'is'
//      expr} '\n' | variable 'is' expr '\n' | 'wait for' identifer '\n' |
//      'action' identifer '{' {statement} '}' '\n' | 'do action' identifer
//      named_decl '\n' | '\n'
//      ;
//
//  value_decl
//      : {expr}
//      | named_decl
//      ;
//  named_decl
//      : ['with' identifer ['as'] expr {'and' identifer ['as'] expr}]
//      ;
////Operators = (* / %) (+ -) (> < >= <=) (== !=) (&) (|) (&&) (||)
//  expr
//      : '(' expr ')'
//      | add_expr {('*' | '/' | '%') add_expr}
//      ;
//  add_expr = comp_expr {('+' | '-') comp_expr}
//  comp_expr = equal_expr {('>' | '<' | '<=' | '>=') equal_expr}
//  equal_expr = and_expr {('==' | '!=') and_expr}
//  and_expr = or_expr {'&' or_expr}
//  or_expr = band_expr {'|' band_expr}
//  band_expr = bor_expr {'&&' bor_expr}
//  bor_expr = value {'||' value}
//
//  value
//      : {/[0-9]/ '.' /[0-9]/}
//      | '"' {ANY_CHAR} '"'
//      | variable
//      ;
//
//  variable = '$' identifer;
//
//  identifer = /[A-Za-z_-]/ {/[A-Za-z0-9_-]/};
