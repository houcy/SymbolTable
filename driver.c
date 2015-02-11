#include "SymTab.h"

int main()
{
    struct SymTab* table = CreateSymTab(10);
    struct SymEntry* entry1 = NULL;
    struct SymEntry* entry2 = NULL;

    const char* Name1 = "Mike";
    const char* Name2 = "John";

    bool worked = EnterName(table, Name1, &entry1);
    bool worked2 = EnterName(table, Name2, &entry2);

    if( worked == true && worked2 == true ) {
        PrintTable(table);
        DestroySymTab(table);
        table = NULL;
        PrintTable(table);
    }   
    return 0;
}