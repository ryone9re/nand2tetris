#include "jack_compiler.h"
#include <stdlib.h>
#include <string.h>

Symbol_table *initialize_symbol_table()
{
    Symbol_table *symbol_table = (Symbol_table *)malloc(sizeof(symbol_table));
    return (symbol_table);
}

void free_symbol_table(Symbol_table *symbol_table)
{
    free(symbol_table);
}

Symbol *new_symbol()
{
    Symbol *s = NULL;

    s = (Symbol *)malloc(sizeof(Symbol));
    s->name = NULL;
    s->type = NULL;
    s->next = NULL;
    return (s);
}

void free_symbol(Symbol *symbols)
{
    Symbol *s = NULL;
    Symbol *t = NULL;

    while (symbols->next != NULL)
    {
        s = symbols;
        while (s->next != NULL)
        {
            t = s;
            s = s->next;
        }
        free(s->type);
        free(s->name);
        free(s);
        t->next = NULL;
    }
    free(symbols->type);
    free(symbols->name);
    free(symbols);
}

Symbol *add_symbol(Symbol *symbol, Symbol *new)
{
    int index = 0;
    Symbol *s = symbol;
    Symbol *n = new;

    if (s->name == NULL)
    {
        s->name = new->name;
        s->type = new->type;
        s->kind = new->kind;
        s->index = index;
        s->next = NULL;
        free(new);
        return (s);
    }
    if (s->kind == new->kind)
        index++;
    while (s->next != NULL)
    {
        s = s->next;
        if (s->kind == new->kind)
            index++;
    }
    n->index = index;
    s->next = n;
    return (s->next);
}
