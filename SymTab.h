#include <stdbool.h>    //This  exists  in  the  C99  language  standard  
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define BOOLEAN 1
#define INTEGER 2
#define STRING 3
/*
 The  symbol  table  structure  proper.  Implemented  as  a  hash  table  that  uses  
 separate  chaining  to  resolve  collisions    
 Contents  is  dynamically  allocated  according  to  size  
*/  
struct SymTab { int size;
                struct SymEntry **Contents;
};

/*  The Name/Attributes association structure used in the symbol tables linked lists */
struct SymEntry { char *Name;
                  void *Attributes;
                  struct SymEntry *Next;
                  int Type;
};

/*  CreateSymTab: create and return a reference to a symbol table of approximately Size
    many entries.

    DestroySymTab: Destroy all storage associated with a Symbol Table which is under the
    table's control. This does not include the attributes
*/

struct SymTab* CreateSymTab(int Size);

void DestroySymTab(struct SymTab *ATable);

/*  EnterName: enter a Name into a symbol table.  Passes back an argument containing an
    entry reference for the name. Return true if the name was not already in the symbol
    table, otherwise return false.  EnterName must allocate space for the Name and copy
    the contents the paramater Name.

    FindName: find a Name in a symbol table. Return an entry reference or NULL depending
    on whether the Name was found
*/ 

bool EnterName(struct SymTab *ATable, const char *Name, struct SymEntry **AnEntry);

struct SymEntry* FindName(struct SymTab *ATable, const char *Name);

/*  SetAttr: Set the attribute pointer associated with an entry.
    GetAttr: Get the attribute pointer associated with an entry.
    GetName: Get the name string associated with an entry.
*/

void SetAttr(struct SymEntry *AnEntry, void *Attributes);
void* GetAttr(struct SymEntry *AnEntry);
void SetType(struct SymEntry *AnEntry, int type);
int GetType(struct SymEntry *AnEntry);
const char* GetName(struct SymEntry *AnEntry);

/*  These two functions can be used to enumerate the contents of a table. The enumeration order
    is arbitrary.

    FirstEntry: Return the "first" entry in a symbol table or NULL if the table is empty.
    "First" does not imply a particular order (e.g. Alphabetical) it is simply the order found
    in the table.

    NextEntry: Return the next entry after the supplied entry or NULL if no more entries.
*/

struct SymEntry* FirstEntry(struct SymTab *ATable);
struct SymEntry* NextEntry(struct SymTab *ATable, struct SymEntry *AnEntry);

/*  HashCode: Hashes the Name in the SymTable */

int HashCode(const char *Name);
void PrintTable(struct SymTab *ATable);