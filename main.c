#include <stdio.h>
#include <stdlib.h>
#include "declaration.h"
#include "common_used.c"
#include "pass1.c"
#include "pass2.c"

int main(void)
{

    // Open "source_code.txt" for read.
    read_file();

    /*------------------- PASS1 -------------------*/

    // Do PASS 1.
    pass1(source_file);

    // Close "source_code.txt".
    fclose(source_file);

    // // Use to testing.
    output_pass1();
    // print_SYMTAB();

    // Open "object_code.txt" for write.
    write_file();

    /*------------------- PASS2 -------------------*/

    // Do PASS 2.
    pass2();
    
    // Use to testing.
    output_pass2();

    // Close "object_code.txt".
    fclose(object_file);

}
