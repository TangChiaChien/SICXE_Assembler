void pass2(void)
{
    char str[100];
    int line_Counter = 0;

    // // H record.
    fprintf(object_file, "H%-6s00%04X%05X\n", word->SYMBOL, word->LOCCTR, program_Length);
    // text_record();

    word = &line[line_Counter];
    get_OPformat();

    while (strcmp(word->OPCODE, "END"))
    {
        // If SYMBOL[0]='.' , means jump over.
        if (!strcmp(word->SYMBOL, "."))
        {
            word->OBJECT_CODE = 0;
        }
        else
        {
            get_OPformat();
        }
        // printf("%s %d\n", word->OPCODE, word->OPCODE_format);
        //  format 1.
        if (word->OPCODE_format == 1)
        {
            word->OBJECT_CODE = instPtr->Opcode;
        }
        // format 2.
        else if (word->OPCODE_format == 2)
        {
            // Shit left 1 Byte.
            word->OBJECT_CODE = (instPtr->Opcode) << 8;
            // If have OPERAND2.
            if (strcmp(word->OPERAND2, ""))
            {
                // 2 Registers.
                word->OBJECT_CODE += ((reg_value(word->OPERAND1) << 4) | reg_value(word->OPERAND2));
            }
            else
            {
                // 1 Register.
                word->OBJECT_CODE += (reg_value(word->OPERAND1) << 4);
            }
        }
        // format 3.
        else if (word->OPCODE_format == 3)
        {
            int n = 0, i = 0, x = 0, b = 0, p = 0, e = 0;
            int disp = 0;
            // If get '#' , Immediate addressing.
            if (word->OPERAND1[0] == '#')
            {
                n = 0;
                i = 1;
            }
            // Else if get '@' , Indirect addressing.
            else if (word->OPERAND1[0] == '@')
            {
                n = 1;
                i = 0;
            }
            // Else.
            else
            {
                i = 1;
                n = 1;
            }
            // Calculate disp.
            // If OPERAND1 is immediate address constant.
            if ((word->OPERAND1[0] == '#') && (word->OPERAND1[1] >= '0') && (word->OPERAND1[1] <= '9'))
            {
                // Use atoi convert to int.
                disp = atoi(&word->OPERAND1[1]);
                b = 0;
                p = 0;
            }
            // Else if PC relative.
            else if (relative_mode(&disp))
            {
                b = 0;
                p = 1;
            }
            // Else BASE relative.
            else
            {
                b = 1;
                p = 0;
            }
            // If is index address.
            if (strcmp(word->OPERAND2, "") && (word->OPERAND2[0] == 'X'))
            {
                x = 1;
            }
            // Else.
            else
            {
                x = 0;
            }
            // Format 3
            e = 0;
            // Exception 'RSUB' , <-- L.
            if (!strcmp(word->OPCODE, "RSUB"))
            {
                n = 1;
                i = 1;
                x = 0;
                b = 0;
                p = 0;
                e = 0;
                disp = reg_L;
            }
            // Generate object code.
            // Shift left 16 bits.
            word->OBJECT_CODE = (instPtr->Opcode) << 16;
            word->OBJECT_CODE += ((n << 17) + (i << 16) + (x << 15) + (b << 14) + (p << 13) + (e << 12));
            word->OBJECT_CODE |= (disp & 0x00000FFF);
        }
        // Else if is format 4.
        else if (word->OPCODE_format == 4)
        {
            int n = 0, i = 0, x = 0, b = 0, p = 0, e = 0;
            int disp = 0;
            // If get '#' , Immediate addressing..
            if (word->OPERAND1[0] == '#')
            {
                n = 0;
                i = 1;
            }
            // If get '@' , Indirect addressing.
            else if (word->OPERAND1[0] == '@')
            {
                n = 1;
                i = 0;
            }
            // Else.
            else
            {
                i = 1;
                n = 1;
            }

            // Calculate address.
            // If OPERAND1 is immediate address constant.
            if ((word->OPERAND1[0] == '#') && (word->OPERAND1[1] >= '0') && (word->OPERAND1[1] <= '9'))
            {
                // Use atoi convert to int.
                disp = atoi(&word->OPERAND1[1]);
                b = 0;
                p = 0;
            }
            // Else (direct addressing).
            else
            {
                char OPERAND1[10] = "\0";
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
                // Search SYMTAB.
                SYMBOL_ADDR = search_SYMTAB(OPERAND1);
                b = 0;
                p = 0;
                disp = SYMBOL_ADDR;
            }
            // If is index address.
            if (strcmp(word->OPERAND2, "") && (word->OPERAND2[0] == 'X'))
            {
                x = 1;
            }
            // Else.
            else
            {
                x = 0;
            }
            // Format 4.
            e = 1;
            // Generate object code.
            // shift left 16 bits.
            word->OBJECT_CODE = (instPtr->Opcode) << 24;
            word->OBJECT_CODE += ((n << 25) + (i << 24) + (x << 23) + (b << 22) + (p << 21) + (e << 20));
            word->OBJECT_CODE += disp;
        }
        // Else if OPCODE = 'BYTE'.
        else if (!strcmp(word->OPCODE, "BYTE"))
        {
            // If string.
            if (word->OPERAND1[0] == 'C')
            {
                int j = 0;
                word->OBJECT_CODE = (int)word->OPERAND1[2];
                for (j = 3; word->OPERAND1[j] != '\''; j++)
                {
                    word->OBJECT_CODE = (int)word->OPERAND1[j] + (word->OBJECT_CODE << 8);
                }
            }
            // Else (char).
            else
            {
                char *temp = &word->OPERAND1[2];
                char num[10] = "\0";
                strcpy(num, temp);
                strtok(num, "'");
                word->OBJECT_CODE = (int)strtol(num, NULL, 16);
            }
        }
        // Else if OPCODE = 'WORD'.
        else if (!strcmp(word->OPCODE, "WORD"))
        {
            word->OBJECT_CODE = atoi(word->OPERAND1);
        }
        // Else if OPCODE = 'BASE'.
        else if (word->OPCODE_format == 7)
        {
            // search SYMTAB
            SYMBOL_ADDR = search_SYMTAB(word->OPERAND1);
            BASE = SYMBOL_ADDR;
        }
        // Else if OPCODE_format = 0 , do nothing.
        else if (word->OPCODE_format == 0)
        {
            word->OBJECT_CODE = 0;
        }
        // OPCODE = C'EOF'.
        else if (word->OPCODE_format == 8)
        {
            char *temp = &word->OPCODE[3];
            char num[10] = "\0";
            char asc[10] = "\0";
            char tmp[10] = "\0";
            int j = 0;
            strcpy(num, temp);
            strtok(num, "'");
            for(j=0;j<strlen(num);j++){
                sprintf(tmp,"%X",num[j]);
                strcat(asc,tmp);
            }
            word->OBJECT_CODE = hex2dec(asc);
        }
        else
        {
            printf("ERROR.\n");
            exit(0);
        }
        restore();
        word = &line[++line_Counter];
    }
}
