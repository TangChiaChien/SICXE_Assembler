#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SYMTAB_SIZE 54

/* ------------------- SYMBOL_TABLE ---------------------- */
typedef struct symbol
{
    char label_Name[10];
    int address;
} Symbol;

/* --------------------- SETTING ------------------------- */
Symbol SYMTAB[SYMTAB_SIZE];

/* Initialization SYMTAB */
void init_Func(void)
{

    int i;

    // Define all SYMTAB.label_Name to " ".
    for (i = 0; i < SYMTAB_SIZE; i++)
    {
        strcpy(SYMTAB[i].label_Name, " ");
    }
}

/* Insert the Symbol Table */
void insert_SYMTAB(char *SYMBOL, int LOCCTR)
{

    int x = (int)SYMBOL[0] % SYMTAB_SIZE;

    // Search SYMTAB for LABEL
    while (1)
    {
        // If SYMTAB[x].label_Name = " " , mean null.
        if (!strcmp(SYMTAB[x].label_Name, " "))
        {
            strcpy(SYMTAB[x].label_Name, SYMBOL);
            SYMTAB[x].address = LOCCTR;
            break;
        }
        // Else if found same ,mean Error.
        else if (!strcmp(SYMTAB[x].label_Name, SYMBOL))
        {
            printf("ERROR! %s has already declared.\n", SYMBOL);
            exit(0);
        }
        // Not found , insert (LABEL, LOCCTR) into SYMTAB.
        else
        {
            x = (x + 1) % SYMTAB_SIZE;
        }
    }
}

/* Search the Symbol Table */
int search_SYMTAB(char *SYMBOL)
{
    int x = (int)SYMBOL[0] % SYMTAB_SIZE;

    // Search SYMTAB.
    for (;;)
    {
        // If found , store ADDR.
        if (!strcmp(SYMTAB[x].label_Name, SYMBOL))
        {
            return SYMTAB[x].address;
        }
        // Else If empty slot.
        else if (!strcmp(SYMTAB[x].label_Name, " "))
        {
            return 0;
        }
        // Not found.
        else
        {
            x++;
            x %= SYMTAB_SIZE;
        }
    }
    return 0;
}

/* Print_SYMTAB */
void print_SYMTAB(void)
{
    int i;

    for (i = 0; i < SYMTAB_SIZE; i++)
    {
        if (strcmp(SYMTAB[i].label_Name, " "))
        {
            printf("line = %2d , label_Name = %-6s , address = %04X.\n", i, SYMTAB[i].label_Name, SYMTAB[i].address);
        }
    }
}
