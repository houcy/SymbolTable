#include "SymTab.h"

struct SymTab* CreateSymTab(int size) {
    struct SymTab* table;
    int i;
    table = (struct SymTab*)malloc(sizeof(struct SymTab));
    table->size = size;
    table->Contents = (struct SymEntry**)malloc(sizeof(struct SymEntry*) * size);

    for( i = 0; i < size; ++i ) {
        table->Contents[i] = NULL;
    }
    return table;
}

void DestroySymTab(struct SymTab *ATable) {
    struct SymEntry* currEntry = NULL;
    struct SymEntry* nextEntry = NULL;

    currEntry = FirstEntry(ATable);

    if( currEntry != NULL ) {
        nextEntry = NextEntry(ATable, currEntry);
    
        while( nextEntry != NULL ) {
            currEntry->Next = NULL;
            free(currEntry->Name);
            free(currEntry);
            currEntry = nextEntry;
            nextEntry = NextEntry(ATable, nextEntry);
        }
        free(currEntry->Name);
        free(currEntry);
    }
    
    free(ATable->Contents);
    free(ATable);
}

bool EnterName(struct SymTab *ATable, const char *Name, struct SymEntry **AnEntry) {
    struct SymEntry* entry = NULL;
    struct SymEntry* temp = NULL;
    int index;

    if( ATable == NULL )
        return false;

    entry = FindName(ATable, Name);

    if(entry != NULL) {
        //Name already found in table
        (*AnEntry) = entry;
        return false;
    } else {
        //allocating space for AnEntry
        char* name = strdup(Name);
        entry = (struct SymEntry*)malloc(sizeof(struct SymEntry));
        entry->Name = name;
        entry->Attributes = NULL;
        entry->Next = NULL;

        index = HashCode(Name)%ATable->size;

        temp = ATable->Contents[index];
        if( temp == NULL ) {
            ATable->Contents[index] = entry;
        } else {
            while( temp->Next != NULL ) {
                temp = temp->Next;
            }
            temp->Next = entry;
        }
        (*AnEntry) = entry;
        return true;
    }
}

struct SymEntry* FindName(struct SymTab *ATable, const char *Name) {
    struct SymEntry* CurrEntry = NULL;

    if( ATable == NULL )
        return NULL;
    //Find index for the name
    int index = HashCode(Name)%ATable->size;

    if( ATable->Contents[index] == NULL ) {
        return NULL;
    }
    else {
        //Search the index for name.  Return it if found.
        CurrEntry = ATable->Contents[index];
        while( CurrEntry != NULL ) {
            if( strcmp(CurrEntry->Name, Name) == 0 )
                return CurrEntry;
            CurrEntry = CurrEntry->Next;
        }
    }
    return NULL;
}

void SetAttr(struct SymEntry *AnEntry, void *Attributes) {
    if( AnEntry != NULL )
        AnEntry->Attributes = Attributes;
}

void* GetAttr(struct SymEntry *AnEntry) {
    if( AnEntry == NULL )
        return NULL;
    else
        return AnEntry->Attributes;
}

const char* GetName(struct SymEntry *AnEntry) {
    if( AnEntry == NULL )
        return NULL;
    else
        return AnEntry->Name;
}

struct SymEntry* FirstEntry(struct SymTab *ATable) {
    int i;
    if( ATable == NULL )
        return NULL;

    for( i = 0; i < ATable->size; ++i ) {
        if( ATable->Contents[i] != NULL)
            return ATable->Contents[i];
    }
    return NULL;
}

struct SymEntry* NextEntry(struct SymTab *ATable, struct SymEntry *AnEntry) {
    int index;

    if( ATable == NULL )
        return NULL;

    if( AnEntry->Next != NULL )
        return AnEntry->Next;
    else {
        index = HashCode(AnEntry->Name)%ATable->size;
        index++;

        while( index < ATable->size ) {
            if( ATable->Contents[index] != NULL )
                return ATable->Contents[index];
            index++;
        }
        return NULL;
    }
}

int HashCode(const char *Name)  {
    int hash = 1;
    int i;
    int length = strlen(Name);

    for( i = 0; i < length; ++i ) {
        hash = hash * 31 + Name[i];
    }

    if( hash < 0 )
        hash *= -1;

    return hash;
}

void PrintTable(struct SymTab *ATable) {
    int i;
    struct SymEntry *currEntry = NULL;

    if( ATable == NULL ) {
        printf("The table does not exist\n");
    } else {
        for( i = 0; i < ATable->size; ++i ) {
            printf("[%d]", i);
            currEntry = ATable->Contents[i];
            while( currEntry != NULL ) {
                printf("%s -> ", currEntry->Name);
                currEntry = currEntry->Next;
            }
            printf("\n");
        }
    }
}