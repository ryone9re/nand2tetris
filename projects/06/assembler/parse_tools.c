#include "assembler.h"
#include <stdlib.h>
#include <string.h>

/* Pick symbol from A/L command (need free after used) */
char *pick_symbol(char *word, command_type type)
{
    char *symbol;

    if (type.A_COMMAND == 1)
        symbol = strdup(++word);
    else if (type.L_COMMAND == 1)
        symbol = word_slicer(word, 1, strlen(word) - 2);
    else
        symbol = NULL;
    return (symbol);
}

/* Return dest mnemonic */
dest_mnemonic pick_dest(char *word)
{
    int i = 0;
    char *mnemonic = NULL;
    dest_mnemonic dm = {0};

    while (word[i] != '=')
    {
        if (word[i] == ';' || word[i] == '\0')
        {
            i = 0;
            break;
        }
        i++;
    }
    if (i <= 0)
    {
        dm.DEST_NULL = 1;
        return (dm);
    }
    mnemonic = word_slicer(word, 0, i);
    if (strcmp(mnemonic, "M") == 0)
        dm.DEST_M = 1;
    else if (strcmp(mnemonic, "D") == 0)
        dm.DEST_D = 1;
    else if (strcmp(mnemonic, "MD") == 0)
        dm.DEST_MD = 1;
    else if (strcmp(mnemonic, "A") == 0)
        dm.DEST_A = 1;
    else if (strcmp(mnemonic, "AM") == 0)
        dm.DEST_AM = 1;
    else if (strcmp(mnemonic, "AD") == 0)
        dm.DEST_AD = 1;
    else if (strcmp(mnemonic, "AMD") == 0)
        dm.DEST_AMD = 1;
    else
        dm.DEST_NULL = 1;
    free(mnemonic);
    return (dm);
}

/* Return comp mnemonic */
comp_mnemonic pick_comp(char *word)
{
    int start_comp = 0;
    char *eq_point = NULL;
    int end_comp = 0;
    char *sc_point = NULL;
    char *mnemonic = NULL;
    comp_mnemonic cm = {0};

    eq_point = strchr(word, '=');
    if (eq_point == NULL)
        start_comp = 0;
    else
        start_comp = eq_point - word + 1;
    sc_point = strchr(word, ';');
    if (sc_point == NULL)
        end_comp = strlen(word);
    else
        end_comp = sc_point - word;
    if (end_comp - start_comp <= 0)
        cm.COMP_NULL = 1;
    mnemonic = word_slicer(word, start_comp, end_comp - start_comp);
    if (strcmp(mnemonic, "0") == 0)
        cm.COMP_ZERO = 1;
    else if (strcmp(mnemonic, "1") == 0)
        cm.COMP_PLSONE = 1;
    else if (strcmp(mnemonic, "-1") == 0)
        cm.COMP_MINONE = 1;
    else if (strcmp(mnemonic, "D") == 0)
        cm.COMP_PLSD = 1;
    else if (strcmp(mnemonic, "A") == 0)
        cm.COMP_PLSA = 1;
    else if (strcmp(mnemonic, "!D") == 0)
        cm.COMP_NOTD = 1;
    else if (strcmp(mnemonic, "!A") == 0)
        cm.COMP_NOTA = 1;
    else if (strcmp(mnemonic, "-D") == 0)
        cm.COMP_MIND = 1;
    else if (strcmp(mnemonic, "-A") == 0)
        cm.COMP_MINA = 1;
    else if (strcmp(mnemonic, "D+1") == 0)
        cm.COMP_DPLSONE = 1;
    else if (strcmp(mnemonic, "A+1") == 0)
        cm.COMP_APLSONE = 1;
    else if (strcmp(mnemonic, "D-1") == 0)
        cm.COMP_DMINONE = 1;
    else if (strcmp(mnemonic, "A-1") == 0)
        cm.COMP_AMINONE = 1;
    else if (strcmp(mnemonic, "D+A") == 0)
        cm.COMP_DPLSA = 1;
    else if (strcmp(mnemonic, "D-A") == 0)
        cm.COMP_DMINA = 1;
    else if (strcmp(mnemonic, "A-D") == 0)
        cm.COMP_AMIND = 1;
    else if (strcmp(mnemonic, "D&A") == 0)
        cm.COMP_DANDA = 1;
    else if (strcmp(mnemonic, "D|A") == 0)
        cm.COMP_DORA = 1;
    else if (strcmp(mnemonic, "M") == 0)
        cm.COMP_PLSM = 1;
    else if (strcmp(mnemonic, "!M") == 0)
        cm.COMP_NOTM = 1;
    else if (strcmp(mnemonic, "-M") == 0)
        cm.COMP_MINM = 1;
    else if (strcmp(mnemonic, "M+1") == 0)
        cm.COMP_MPLSONE = 1;
    else if (strcmp(mnemonic, "M-1") == 0)
        cm.COMP_MMINONE = 1;
    else if (strcmp(mnemonic, "D+M") == 0)
        cm.COMP_DPLSM = 1;
    else if (strcmp(mnemonic, "D-M") == 0)
        cm.COMP_DMINM = 1;
    else if (strcmp(mnemonic, "M-D") == 0)
        cm.COMP_MMIND = 1;
    else if (strcmp(mnemonic, "D&M") == 0)
        cm.COMP_DANDM = 1;
    else if (strcmp(mnemonic, "D|M") == 0)
        cm.COMP_DORM = 1;
    else
        cm.COMP_NULL = 1;
    free(mnemonic);
    return (cm);
}

/* Return jump mnemonic */
jump_mnemonic pick_jump(char *word)
{
    int i = 0;
    char *mnemonic = NULL;
    jump_mnemonic jm = {0};

    while (word[i] != ';')
    {
        if (word[i] == '\0')
        {
            i = 0;
            break;
        }
        i++;
    }
    if (i <= 0)
    {
        jm.JUMP_NULL = 1;
        return (jm);
    }
    mnemonic = word_slicer(word, i + 1, 3);
    if (strcmp(mnemonic, "JGT") == 0)
        jm.JUMP_JGT = 1;
    else if (strcmp(mnemonic, "JEQ") == 0)
        jm.JUMP_JEQ = 1;
    else if (strcmp(mnemonic, "JGE") == 0)
        jm.JUMP_JGE = 1;
    else if (strcmp(mnemonic, "JLT") == 0)
        jm.JUMP_JLT = 1;
    else if (strcmp(mnemonic, "JNE") == 0)
        jm.JUMP_JNE = 1;
    else if (strcmp(mnemonic, "JLE") == 0)
        jm.JUMP_JLE = 1;
    else if (strcmp(mnemonic, "JMP") == 0)
        jm.JUMP_JMP = 1;
    else
        jm.JUMP_NULL = 1;
    free(mnemonic);
    return (jm);
}
