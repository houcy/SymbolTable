#include "SymTab.h"

int main()
{
    struct SymTab* table = CreateSymTab(10);
    struct SymEntry* entry1 = NULL;
    struct SymEntry* entry2 = NULL;
    struct SymEntry* entry3 = NULL;
    const char* Name1 = "Mike";
    const char* Name2 = "John";

    bool worked = EnterName(table, Name1, &entry1);
    bool worked2 = EnterName(table, Name2, &entry2);
    bool worked3 = EnterName(table, "Peter", &entry1);
    bool worked4 = EnterName(table, "Jessica", &entry1);
    bool worked5 = EnterName(table, "Thompson", &entry3);
    bool worked6 = EnterName(table, "Reuters", &entry1);
    bool worked7 = EnterName(table, "389432", &entry1);

    entry1 = FirstEntry(table);

    printf("First entry: %s\n", entry1->Name);

    PrintTable(table);
    DestroySymTab(table);
    table = NULL;
    PrintTable(table);
    return 0;
}