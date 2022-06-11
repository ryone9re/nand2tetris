#include "jack_compiler.h"
#include <stdlib.h>
#include <string.h>

Symbol_table *initialize_symbol_table()
{
    Symbol_table *symbol_table = (Symbol_table *)malloc(sizeof(symbol_table));
    symbol_table->class = NULL;
    symbol_table->subroutine = NULL;
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

    if (symbols != NULL)
    {
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

int var_count(Symbol_table *symbol_table, enum Kind kind)
{
    int count = 0;
    Symbol *c = symbol_table->class;
    Symbol *s = symbol_table->subroutine;

    if (s != NULL)
    {
        while (s->next != NULL)
        {
            if (s->kind == kind)
                count++;
            s = s->next;
        }
        if (s->name != NULL && s->kind == kind)
            count++;
    }
    if (c != NULL)
    {
        while (c->next != NULL)
        {
            if (c->kind == kind)
                count++;
            c = c->next;
        }
        if (c->name != NULL && c->kind == kind)
            count++;
    }
    return (count);
}

int index_of(Symbol_table *symbol_table, char *variable)
{
    Symbol *s = symbol_table->subroutine;
    Symbol *c = symbol_table->class;

    if (s != NULL && s->name != NULL)
    {
        while (s->next != NULL)
        {
            if (strcmp(s->name, variable) == 0)
                return (s->index);
            s = s->next;
        }
        if (strcmp(s->name, variable) == 0)
            return (s->index);
    }
    if (c != NULL && c->name != NULL)
    {
        while (c->next != NULL)
        {
            if (strcmp(c->name, variable) == 0)
                return (c->index);
            c = c->next;
        }
        if (strcmp(c->name, variable) == 0)
            return (c->index);
    }
    return (-1);
}

enum Kind kind_of(Symbol_table *symbol_table, char *variable)
{
    Symbol *s = symbol_table->subroutine;
    Symbol *c = symbol_table->class;

    if (s != NULL && s->name != NULL)
    {
        while (s->next != NULL)
        {
            if (strcmp(s->name, variable) == 0)
                return (s->kind);
            s = s->next;
        }
        if (strcmp(s->name, variable) == 0)
            return (s->kind);
    }
    if (c != NULL && c->name != NULL)
    {
        while (c->next != NULL)
        {
            if (strcmp(c->name, variable) == 0)
                return (c->kind);
            c = c->next;
        }
        if (strcmp(c->name, variable) == 0)
            return (c->kind);
    }
    return (NoKind);
}

char *type_of(Symbol_table *symbol_table, char *variable)
{
    Symbol *s = symbol_table->subroutine;
    Symbol *c = symbol_table->class;

    if (s != NULL && s->name != NULL)
    {
        while (s->next != NULL)
        {
            if (strcmp(s->name, variable) == 0)
                return (s->type);
            s = s->next;
        }
        if (strcmp(s->name, variable) == 0)
            return (s->type);
    }
    if (c != NULL && c->name != NULL)
    {
        while (c->next != NULL)
        {
            if (strcmp(c->name, variable) == 0)
                return (c->type);
            c = c->next;
        }
        if (strcmp(c->name, variable) == 0)
            return (c->type);
    }
    return (NULL);
}
