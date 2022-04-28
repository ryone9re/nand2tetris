#include "assembler.h"
#include <stdlib.h>
#include <string.h>

/* Create symbol table */
symbol_table *create_symbol_table()
{
    symbol_table *sp = (symbol_table *)malloc(sizeof(symbol_table));
    symbol_table *stp = sp;
    char *default_label[] = {
        "LCL",
        "ARG",
        "THIS",
        "THAT",
        "R0",
        "R1",
        "R2",
        "R3",
        "R4",
        "R5",
        "R6",
        "R7",
        "R8",
        "R9",
        "R10",
        "R11",
        "R12",
        "R13",
        "R14",
        "R15",
        "SCREEN",
        "KBD"};
    int default_address[] = {1, 2, 3, 4, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16384, 24576};

    sp->symbol = strdup("SP");
    sp->address = 0;
    sp->next_symbol = NULL;
    for (int i = 0; i < 22; i++)
    {
        add_entry(stp, default_label[i], default_address[i]);
        stp = stp->next_symbol;
    }
    return (sp);
}

/* Search label and add to symbol_table stp */
void first_path(symbol_table *stp, char **instructions)
{
    char *symbol = NULL;
    int instruction_count = 0;

    for (int i = 0; instructions[i] != NULL; i++)
    {
        command_type t = check_command_type(instructions[i]);

        if (t.L_COMMAND == 1)
        {
            symbol = pick_symbol(instructions[i], t);
            if (contains(stp, symbol) == FALSE)
                add_entry(stp, symbol, instruction_count);
            free(symbol);
        }
        else
            instruction_count++;
    }
}

/* Add (symbol, address) to symbol_table stp */
void add_entry(symbol_table *stp, char *symbol, int address)
{
    symbol_table *stpp = stp;
    symbol_table *p = (symbol_table *)malloc(sizeof(symbol_table));

    while (stpp->next_symbol != NULL)
        stpp = stpp->next_symbol;
    p->symbol = strdup(symbol);
    p->address = address;
    p->next_symbol = NULL;
    stpp->next_symbol = p;
}

/* Check containing symbol in symbol_table stp */
int contains(symbol_table *stp, char *symbol)
{
    symbol_table *p = stp;

    while (p != NULL)
    {
        if (strcmp(p->symbol, symbol) == 0)
            return (TRUE);
        p = p->next_symbol;
    }
    return (FALSE);
}

/* Return address of symbol */
int get_address(symbol_table *stp, char *symbol)
{
    symbol_table *p = stp;

    while (p != NULL)
    {
        if (strcmp(p->symbol, symbol) == 0)
            return (p->address);
        p = p->next_symbol;
    }
    return (-1);
}

/* Free symbol_table stp */
void free_symbol_table(symbol_table *stp)
{
    symbol_table *p = stp;
    symbol_table *q = NULL;

    while (p != NULL)
    {
        q = p->next_symbol;
        free(p);
        p = q;
    }
}
