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

/* Write dest mnemonic binary code to temp_dest */
void write_dest_bin(char *temp_dest, dest_mnemonic dm)
{
    if (dm.DEST_M == 1)
        set_buffer(temp_dest, "001", 3);
    else if (dm.DEST_D == 1)
        set_buffer(temp_dest, "010", 3);
    else if (dm.DEST_MD == 1)
        set_buffer(temp_dest, "011", 3);
    else if (dm.DEST_A == 1)
        set_buffer(temp_dest, "100", 3);
    else if (dm.DEST_AM == 1)
        set_buffer(temp_dest, "101", 3);
    else if (dm.DEST_AD == 1)
        set_buffer(temp_dest, "110", 3);
    else if (dm.DEST_AMD == 1)
        set_buffer(temp_dest, "111", 3);
    else
        set_buffer(temp_dest, "000", 3);
}

/* Write comp mnemonic binary code to temp_comp */
void write_comp_bin(char *temp_comp, comp_mnemonic cm)
{
    if (cm.COMP_ZERO == 1)
        set_buffer(temp_comp, "0101010", 7);
    else if (cm.COMP_PLSONE == 1)
        set_buffer(temp_comp, "0111111", 7);
    else if (cm.COMP_MINONE == 1)
        set_buffer(temp_comp, "0111010", 7);
    else if (cm.COMP_PLSD == 1)
        set_buffer(temp_comp, "0001100", 7);
    else if (cm.COMP_PLSA == 1)
        set_buffer(temp_comp, "0110000", 7);
    else if (cm.COMP_NOTD == 1)
        set_buffer(temp_comp, "0001101", 7);
    else if (cm.COMP_NOTA == 1)
        set_buffer(temp_comp, "0110001", 7);
    else if (cm.COMP_MIND == 1)
        set_buffer(temp_comp, "0001111", 7);
    else if (cm.COMP_MINA == 1)
        set_buffer(temp_comp, "0110011", 7);
    else if (cm.COMP_DPLSONE == 1)
        set_buffer(temp_comp, "0011111", 7);
    else if (cm.COMP_APLSONE == 1)
        set_buffer(temp_comp, "0110111", 7);
    else if (cm.COMP_DMINONE == 1)
        set_buffer(temp_comp, "0001110", 7);
    else if (cm.COMP_AMINONE == 1)
        set_buffer(temp_comp, "0110010", 7);
    else if (cm.COMP_DPLSA == 1)
        set_buffer(temp_comp, "0000010", 7);
    else if (cm.COMP_DMINA == 1)
        set_buffer(temp_comp, "0010011", 7);
    else if (cm.COMP_AMIND == 1)
        set_buffer(temp_comp, "0000111", 7);
    else if (cm.COMP_DANDA == 1)
        set_buffer(temp_comp, "0000000", 7);
    else if (cm.COMP_DORA == 1)
        set_buffer(temp_comp, "0010101", 7);
    else if (cm.COMP_PLSM == 1)
        set_buffer(temp_comp, "1110000", 7);
    else if (cm.COMP_NOTM == 1)
        set_buffer(temp_comp, "1110001", 7);
    else if (cm.COMP_MINM == 1)
        set_buffer(temp_comp, "1110011", 7);
    else if (cm.COMP_MPLSONE == 1)
        set_buffer(temp_comp, "1110111", 7);
    else if (cm.COMP_MMINONE == 1)
        set_buffer(temp_comp, "1110010", 7);
    else if (cm.COMP_DPLSM == 1)
        set_buffer(temp_comp, "1000010", 7);
    else if (cm.COMP_DMINM == 1)
        set_buffer(temp_comp, "1010011", 7);
    else if (cm.COMP_MMIND == 1)
        set_buffer(temp_comp, "1000111", 7);
    else if (cm.COMP_DANDM == 1)
        set_buffer(temp_comp, "1000000", 7);
    else if (cm.COMP_DORM == 1)
        set_buffer(temp_comp, "1010101", 7);
}

/* Write jump mnemonic binary code to temp_jump */
void write_jump_bin(char *temp_jump, jump_mnemonic jm)
{
    if (jm.JUMP_JGT == 1)
        set_buffer(temp_jump, "001", 3);
    else if (jm.JUMP_JEQ == 1)
        set_buffer(temp_jump, "010", 3);
    else if (jm.JUMP_JGE == 1)
        set_buffer(temp_jump, "011", 3);
    else if (jm.JUMP_JLT == 1)
        set_buffer(temp_jump, "100", 3);
    else if (jm.JUMP_JNE == 1)
        set_buffer(temp_jump, "101", 3);
    else if (jm.JUMP_JLE == 1)
        set_buffer(temp_jump, "110", 3);
    else if (jm.JUMP_JMP == 1)
        set_buffer(temp_jump, "111", 3);
    else
        set_buffer(temp_jump, "000", 3);
}
