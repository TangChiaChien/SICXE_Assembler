#include <string.h>
#include <stdio.h>
#include "symbol_table.h"
#include "operation_table.h"

typedef struct line
{
    char SYMBOL[10], OPCODE[10], OPERAND1[30], OPERAND2[30];
    int OPCODE_extended;
    int OPCODE_format;
    int LOCCTR;
    int OBJECT_CODE;
} Line;

Line line[60];
Line *word;

FILE *source_file; 
FILE *object_file; 

SICXE_OP *instPtr;

int  BASE = 0;                                       
int  reg_A = 0, reg_X = 0, reg_L = 0;               
int  reg_S = 0, reg_R = 0, reg_F = 0;               

int  SYMBOL_ADDR;  

int program_Length = 0;
char *instr;
char token[30] = "\0";
char text_line[100] = "\0";

