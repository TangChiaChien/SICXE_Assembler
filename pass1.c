void pass1(FILE *source_file)
{
    int temp_loc = 0;
    int LOCCTR = 0;
    int line_counter = 0;

    init_Func();
    get_nextline(source_file, line_counter);

    while (strcmp(word->OPCODE, "END"))
    {
        word->LOCCTR = LOCCTR;
        if (!strcmp(word->SYMBOL, "."))
        {
            word->OPCODE_extended = 0;
            word->OPCODE_format = 0;
        }
        else{
            get_OPformat();
        }
        // If there is a symbol in the LABEL field.
        if (strcmp(word->SYMBOL, "") && strcmp(word->SYMBOL, "."))
        {
            insert_SYMTAB(word->SYMBOL, word->LOCCTR);
        }
        // Search OPTAB for OPCODE.
        if (search_OPTAB(&instPtr))
        {
            // Add instruction length to LOCCTR.
            LOCCTR += (word->OPCODE_format);
        }
        // Else if OPCODE = 'WORD'.
        else if (!strcmp(word->OPCODE, "WORD"))
        {
            // Add 3 to LOCCTR.
            LOCCTR += 3;
        }
        // Else if OPCODE = 'RESW'.
        else if (!strcmp(word->OPCODE, "RESW"))
        {
            // Add 3 * #[OPERAND] to LOCCTR.
            LOCCTR += (3 * atoi(word->OPERAND1));
        }
        // Else if OPCODE = 'RESB'.
        else if (!strcmp(word->OPCODE, "RESB"))
        {
            // Add #[OPERAND] to LOCCTR.
            LOCCTR += atoi(word->OPERAND1);
        }
        // Else if OPCODE = 'BYTE'.
        else if (!strcmp(word->OPCODE, "BYTE"))
        {
            // Find length of constant in bytes , add length to LOCCTR.
            LOCCTR += byte_Length();
        }
        else if (!strcmp(word->OPCODE, "=C'EOF'"))
        {
            LOCCTR += 3;
        }
        // Else if OPCODE = 'EQU'.
        else if (!strcmp(word->OPCODE, "EQU"))
        {
            if (!strcmp(word->SYMBOL, "BUFEND"))
            {
                // Temporary record orig_LOCCTR.
                temp_loc = LOCCTR;
                // LOCCTR = BUFEND - BUFFER.
                LOCCTR = search_SYMTAB("BUFEND")-search_SYMTAB("BUFFER");
            }
            else
            {
                LOCCTR = temp_loc;
            }
        }
        // FOR TESTING: OUTPUT EACH STEP
        restore();
        // Read first line.
        get_nextline(source_file, ++line_counter);
    }
    word->LOCCTR = LOCCTR;
    restore();
    
    // Save (LOCCTR - starting address) as program length
    program_Length = (LOCCTR - 0);
}