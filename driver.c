#include "SymTab.h"

int main()
{
    struct SymTab* table = CreateSymTab(10);
    struct SymEntry* entry1 = NULL;
    struct SymEntry* entry2 = NULL;
    struct SymEntry* entry3 = NULL;
    struct SymEntry* entry4 = NULL;

    const char* Name1 = "Mike";
    const char* Name2 = "John";

    bool worked = EnterName(table, Name1, &entry1);
    bool worked2 = EnterName(table, Name2, &entry2);

    entry3 = FindName(table, Name1);
    entry4 = FindName(table, "notReal");

    printf("findName test: %s\n", entry3->Name);

    if( entry4 == NULL )
        printf("FindName worked\n");

    if( worked == true && worked2 == true ) {
        PrintTable(table);
        DestroySymTab(table);
        table = NULL;
        PrintTable(table);
    }   
    return 0;
}