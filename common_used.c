#include <string.h>
#include <stdio.h>

/* Line_Format */
void line_Format(char *str)
{
    int i;
    char *ptr;
    ptr = (char *)malloc(sizeof(char) * strlen(str));

    for (i = 0; i < strlen(str); i++)
    {
        if (str[i] == '\n')
        {
            break;
        }
        if (str[i] == ' ' && str[i + 1] == ' ')
        {
            break;
        }
        if (str[i] == ' ' && str[i + 1] == '\n')
        {
            break;
        }
        ptr[i] = str[i];
    }
    strcpy(token, ptr);
    free(ptr);
}

void initialize(char *str, int line_cnt)
{
    char temp[30];

    strlcpy(temp, str, 10);
    line_Format(temp);
    strcpy(line[line_cnt].SYMBOL, token);
    str += 10;

    strlcpy(temp, str, 10);
    line_Format(temp);
    strcpy(line[line_cnt].OPCODE, token);
    str += 10;

    strlcpy(temp, str, 30);
    line_Format(temp);
    strtok(token, ",");
    strcpy(line[line_cnt].OPERAND1, token);

    str += strlen(line[line_cnt].OPERAND1) + 1;
    strlcpy(temp, str, 30);
    line_Format(temp);

    strcpy(line[line_cnt].OPERAND2, token);
}

/* Read next line */
void get_nextline(FILE *source_file, int line_cnt)
{
    char temp[100] = "\0";

    fgets(temp, 100, source_file);
    initialize(temp, line_cnt);
    word = &line[line_cnt];
}

void output_pass1()
{
    int i = 0;
    printf("\n\n======================== PASS 1 RESULT. =========================\n\n");

    while (strcmp(line[i].OPCODE, "END"))
    {
        printf("%04X      %-10s %-10s %s %s\n", line[i].LOCCTR, line[i].SYMBOL, line[i].OPCODE, line[i].OPERAND1, line[i].OPERAND2);
        i++;
    }
    printf("%04X      %-10s %-10s %s %s\n", line[i].LOCCTR, line[i].SYMBOL, line[i].OPCODE, line[i].OPERAND1, line[i].OPERAND2);
}

void output_pass2()
{
    int i = 0;
    char OPERAND[30] = "\0";
    printf("\n\n======================== PASS 2 RESULT. =========================\n\n");

    while (strcmp(line[i].OPCODE, "END"))
    {
        strcat(line[i].OPERAND1, line[i].OPERAND2);
        if (line[i].OBJECT_CODE == 0)
        {
            printf("%04X      %-10s %-10s %-30s\n", line[i].LOCCTR, line[i].SYMBOL, line[i].OPCODE, line[i].OPERAND1);
        }
        else
        {
            printf("%04X      %-10s %-10s %-30s %-5X\n", line[i].LOCCTR, line[i].SYMBOL, line[i].OPCODE, line[i].OPERAND1, line[i].OBJECT_CODE);
        }
        i++;
    }
    printf("%04X      %-10s %-10s %-30s\n", line[i].LOCCTR, line[i].SYMBOL, line[i].OPCODE, line[i].OPERAND1);
}

void read_file()
{
    if (!(source_file = fopen("code.txt", "r")))
    {
        printf("\n\nSOURCE CODE OPENED ERROR!\n\n");
        exit(1);
    }
    else
    {
        printf("\n\nSOURCE CODE OPENED SUCCESS!\n\n");
    }
}

void write_file()
{
    if (!(object_file = fopen("object_code.txt", "w")))
    {
        printf("\n\nOBJECT FILE CREATED ERROR!\n\n");
        exit(1);
    }
    else
    {
        printf("\n\nOBJECT FILE CREATED SUCCESS!\n\n");
    }
}

/* Return Length of instruction */
int inst_Length(SICXE_OP *instPtr)
{
    int x = instPtr->Format;

    switch (x)
    {
    case 1:
        return 1;
    case 2:
        return 2;
    default:
        if (word->OPCODE_extended == 1)
            return 4;
        else
            return 3;
    }

    return 0;
}

/* Return Length of BYTE Type */
int byte_Length(void)
{
    char *temp = word->OPERAND1;
    char str1 = temp[0];
    char str2[10] = "\0";

    temp += 2;
    strlcpy(str2, temp, 10);
    strtok(str2, "'");

    // X = Hex Interger.
    if (str1 == 'X')
    {
        return 1;
    }
    // C = string.
    else
    {
        return strlen(str2);
    }

    return 0;
}

/* Search OPTAB */
int search_OPTAB(SICXE_OP **current)
{

    for (*current = OPTAB[hash_Func(word->OPCODE)];;)
    {
        // Get Operation Successfully.
        if (!strcmp((*current)->Mnemonic, word->OPCODE))
        {
            return 1; // Found
        }
        // If not find index -> next.
        else if ((*current)->next != NULL)
        {
            *current = (SICXE_OP *)(*current)->next;
            continue;
        }
        // Get Operation Fail.
        else
        {
            return 0; // not found
        }
    }
}

/* Get OPCODE Format */
void get_OPformat()
{

    // Initialization extended tag.
    word->OPCODE_extended = 0;

    // If OPCODE is extended format.
    if (word->OPCODE[0] == '+')
    {
        char *instr = &word->OPCODE[1];
        char temp[10] = "\0";
        strcpy(temp, instr);
        strcpy(word->OPCODE, temp);
        word->OPCODE_extended = 1;
    }

    // Search OPTAB , if OPCODE is found.
    if (search_OPTAB(&instPtr))
    {
        // Use inst_Length() to return OPCODE_format.
        word->OPCODE_format = inst_Length(instPtr);
    }
    // Else if OPCODE is "BYTE".
    else if (!strcmp(word->OPCODE, "BYTE"))
    {
        word->OPCODE_format = 5;
    }
    // Else if OPCODE is "WORD".
    else if (!strcmp(word->OPCODE, "WORD"))
    {
        word->OPCODE_format = 6;
    }
    // Else if OPCODE is "RESB".
    else if (!strcmp(word->OPCODE, "RESB"))
    {
        word->OPCODE_format = 0;
    }
    // Else if OPCODE is "RESW".
    else if (!strcmp(word->OPCODE, "RESW"))
    {
        word->OPCODE_format = 0;
    }
    // Else if OPCODE is "BASE".
    else if (!strcmp(word->OPCODE, "BASE"))
    {
        word->OPCODE_format = 7;
    }
    // Else if OPCODE is "START".
    else if (!strcmp(word->OPCODE, "START"))
    {
        word->OPCODE_format = 0;
    }
    // Else if OPCODE is "END".
    else if (!strcmp(word->OPCODE, "END"))
    {
        word->OPCODE_format = 0;
    }
    // Else if OPCODE is "EQU".
    else if (!strcmp(word->OPCODE, "EQU"))
    {
        word->OPCODE_format = 0;
    }
    // Else if OPCODE is "=C'EOF'".
    else if (!strcmp(word->OPCODE, "=C'EOF'"))
    {
        word->OPCODE_format = 8;
    }
    // Else if OPCODE is "LTORG".
    else if (!strcmp(word->OPCODE, "LTORG"))
    {
        word->OPCODE_format = 0;
    }
    // Else (undefined OPCODE).
    else
    {
        printf("ERROR. '%s' is undefined OPCODE.\n", word->OPCODE);
        exit(0);
    }
}

/* Reg_value */
int reg_value(char *str)
{
    if (!strcmp(str, "A"))
    {
        return 0;
    }
    else if (!strcmp(str, "X"))
    {
        return 1;
    }
    else if (!strcmp(str, "L"))
    {
        return 2;
    }
    else if (!strcmp(str, "B"))
    {
        return 3;
    }
    else if (!strcmp(str, "S"))
    {
        return 4;
    }
    else if (!strcmp(str, "T"))
    {
        return 5;
    }
    else if (!strcmp(str, "F"))
    {
        return 6;
    }
    else if (!strcmp(str, "PC"))
    {
        return 8;
    }
    else if (!strcmp(str, "SW"))
    {
        return 9;
    }
    else
    {
        printf("ERROR. %s is not a defined register.\n", str);
        exit(0);
    }

    return 0;
}

/*Relative_mode*/
int relative_mode(int *disp)
{
    int PC;
    char OPERAND1[30] = "\0";

    // Clean '@' or '#'.
    if ((word->OPERAND1[0] == '@') || (word->OPERAND1[0] == '#'))
    {
        char *temp = &word->OPERAND1[1];
        strcpy(OPERAND1, temp);
    }
    else
    {
        strcpy(OPERAND1, word->OPERAND1);
    }

    // If OPERAND1 =C'EOF' , find '*'.
    if (!strcmp(OPERAND1, "=C'EOF'"))
    {
        SYMBOL_ADDR = search_SYMTAB("*");
    }
    // IF OPERAND1 =X'05' , SYMBOL_ADDR =4211.
    else if (!strcmp(OPERAND1, "=X'05'"))
    {
        PC = word->LOCCTR;
        SYMBOL_ADDR = 4211;
        *disp = SYMBOL_ADDR - PC;
        return 0;
    }
    else
    {
        SYMBOL_ADDR = search_SYMTAB(OPERAND1);
    }
    // PC point to next instruction.
    PC = word->LOCCTR + word->OPCODE_format;
    // Try PC relative first.
    *disp = SYMBOL_ADDR - PC;

    // Set range.
    if ((*disp >= -2048) && (*disp < 2048))
    {
        return 1;
    }

    // Try Base relative.
    *disp = SYMBOL_ADDR - BASE;

    // Set range.
    if (*disp < 4096)
    {
        return 0;
    }
    // Else.
    else
    {
        printf("ERROR! Invalid Addressing Mode.\n");
        exit(1);
    }
}

int getIndexOfString(char ch)
{
    if (ch >= '0' && ch <= '9')
    {
        return ch - '0';
    }
    if (ch >= 'A' && ch <= 'F')
    {
        return ch - 'A' + 10;
    }
    if (ch >= 'a' && ch <= 'f')
    {
        return ch - 'a' + 10;
    }
    return -1;
}
int hex2dec(char *hex)
{
    int sum = 0;
    int t = 1;
    int i, len;

    len = strlen(hex);
    for (i = len - 1; i >= 0; i--)
    {
        sum += t * getIndexOfString(*(hex + i));
        t *= 16;
    }
    return sum;
}

/* Output for testing */
void restore()
{
    if(word->OPCODE_extended==1){
        char ex_OPCODE[10] = "+";
        strcat(ex_OPCODE,word->OPCODE);
        strcpy(word->OPCODE, ex_OPCODE);
    }
    //printf("%04X      %-10s %-10s %s %s\n",word->LOCCTR, word->SYMBOL, word->OPCODE, word->OPERAND1, word->OPERAND2);
}

// /* text_record */
// void text_record(void)
// {
//     static init = 1;
//     char str[5];
//     char temp1[10], temp2[10];

//     // Check buffer full , if yes.
//     if ((69 - strlen(text_line)) < (2 * word->OPCODE_format))
//     {
//         // Calculate string length.
//         sprintf(str, "%02X", (strlen(text_line) - 9) / 2);
//         text_line[7] = str[0];
//         text_line[8] = str[1];
//         // Write into file.
//         fprintf(object_file, "%s\n", text_line);
//         // Expand LOC to 6 bits string.
//         strcpy(temp1, word->LOCCTR);
//         sprintf(temp2, "%06s", temp1);
//         // Set new text record.
//         strcpy(text_line, "T");
//         strcat(text_line, temp2);
//         strcat(text_line, "XX");
//         // Insert object.
//         strcat(text_line, word->OBJECT_CODE);
//     }
//     else
//     {
//         // If init = 1.
//         if (init == 1)
//         {
//             // Expand LOC to 6 bits string.
//             strcpy(temp1, word->LOCCTR);
//             sprintf(temp2, "%06s", temp1);
//             // Set new text record.
//             strcpy(text_line, "T");
//             strcat(text_line, temp2);
//             strcat(text_line, "XX");
//             // Insert object into line.
//             strcat(text_line, word->OBJECT_CODE);
//             init = 0;
//         }
//         // If init = 2.
//         else if (init == 2)
//         {
//             // Calculate string length.
//             sprintf(str, "%02X", (strlen(text_line) - 9) / 2);
//             text_line[7] = str[0];
//             text_line[8] = str[1];
//             // Write into file.
//             fprintf(object_file, "%s\n", text_line);
//             // Expand LOC to 6 bits string.
//             strcpy(temp1, word->LOCCTR);
//             sprintf(temp2, "%06s", temp1);
//             // Set new text record.
//             strcpy(text_line, "T");
//             strcat(text_line, temp2);
//             strcat(text_line, "XX");
//             // Insert object into line.
//             strcat(text_line, word->OBJECT_CODE);
//             init = 0;
//         }
//         // Else if OPCODE = 'END'.
//         else if (!strcmp(word->OPCODE, "END"))
//         {
//             // Calculate string length.
//             sprintf(str, "%02X", (strlen(text_line) - 9) / 2);
//             text_line[7] = str[0];
//             text_line[8] = str[1];
//             // Write into file.
//             fprintf(object_file, "%s\n", text_line);
//         }
//         // Else.
//         else
//         {
//             // Insert line.
//             strcat(text_line, word->OBJECT_CODE);
//             if (!strcmp(word->SYMBOL, "EOF"))
//             {
//                 init = 2;
//             }
//         }
//     }
// }
