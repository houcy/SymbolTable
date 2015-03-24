#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "SymTab.h"

extern struct SymTab *table;

void printSymTab() {
    PrintTable(table);
}

void storeVar(char *name, int v) {
    struct SymEntry *entry;
    EnterName(table, name, &entry);
    SetAttr(entry, (void *)v );
}

int getVal(char *name) {
    struct SymEntry *entry = FindName(table, name);

    if (entry == NULL) {
        storeVar(name, 0);
        return 0;
    }
    else
        return (int)GetAttr(entry);
}

int doADD(int  v1,  int  v2) {
    return v1 + v2;
}

int  doMULT(int  v1,  int  v2) {
    return v1 * v2;
}

int  doNEGATE(int  v1) {
    return v1 * (-1);
}

