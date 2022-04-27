#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#define TRUE 1
#define FALSE 0
#define MAXSIZE 2097152
#define READSIZ 1024

/* typedef */
typedef struct
{
    int A_COMMAND;
    int C_COMMAND;
    int L_COMMAND;
} command_type;

typedef struct
{
    int DEST_NULL;
    int DEST_M;
    int DEST_D;
    int DEST_MD;
    int DEST_A;
    int DEST_AM;
    int DEST_AD;
    int DEST_AMD;
} dest_mnemonic;

typedef struct
{
    int COMP_ZERO;
    int COMP_PLSONE;
    int COMP_MINONE;
    int COMP_PLSD;
    int COMP_PLSA;
    int COMP_NOTD;
    int COMP_NOTA;
    int COMP_MIND;
    int COMP_MINA;
    int COMP_DPLSONE;
    int COMP_APLSONE;
    int COMP_DMINONE;
    int COMP_AMINONE;
    int COMP_DPLSA;
    int COMP_DMINA;
    int COMP_AMIND;
    int COMP_DANDA;
    int COMP_DORA;
    int COMP_PLSM;
    int COMP_NOTM;
    int COMP_MINM;
    int COMP_MPLSONE;
    int COMP_MMINONE;
    int COMP_DPLSM;
    int COMP_DMINM;
    int COMP_MMIND;
    int COMP_DANDM;
    int COMP_DORM;
    int COMP_NULL;
} comp_mnemonic;

typedef struct
{
    int JUMP_NULL;
    int JUMP_JGT;
    int JUMP_JEQ;
    int JUMP_JGE;
    int JUMP_JLT;
    int JUMP_JNE;
    int JUMP_JLE;
    int JUMP_JMP;
} jump_mnemonic;

/* Prototypes */
/* main.c */
/* Parser */
void parser(char *file_name);
void constructor(char *file_name);
char **buffer_splitter(char *buffer);
command_type check_command_type(char *word);
void deep_free(char **instructions);
/* Code */
void a_commander(char *symbol, char *file_name);
void c_commander(char *word, char *file_name);

/* parse_tools.c */
char *pick_symbol(char *word, command_type type);
dest_mnemonic pick_dest(char *word);
comp_mnemonic pick_comp(char *word);
jump_mnemonic pick_jump(char *word);
void write_dest_bin(char *temp_dest, dest_mnemonic dm);
void write_comp_bin(char *temp_comp, comp_mnemonic cm);
void write_jump_bin(char *temp_jump, jump_mnemonic jm);

/* util.c */
int row_counter(char *buffer);
int get_line_length(char *buffer, int start_word);
char *word_slicer(char *buffer, int now_word_num, int line_length);
void buffer_initializer(char *buffer);
char *file_ext_converter(char *src, char *new_ext);
void write_output(char *file_name, char *ext, char *output);
void set_buffer(char *buffer, char *word, int size);

#endif /* ASSEBMLER_H */
