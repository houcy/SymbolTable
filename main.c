#include <stdio.h>
#include "SymTab.h"
#include "IOManager/IOMngr.h"

extern int yyparse();

struct SymTab *table;

int main(int argc, char *argv[]) {
    table = CreateSymTab(17);
    if (!OpenFiles(argv[1], argv[2])) {
        printf("open failed\n");
        exit(0);
    }

    yyparse(); 
    return 1;
}