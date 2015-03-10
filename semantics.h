#include  <stdbool.h>  
#include  "SymTab.h"  

extern  void  printSymTab();  
extern  void  storeVar(char  *name,  int  v);  
extern  int  doADD(int  v1,  int  v2);  
extern  int  doMULT(int  v1,  int  v2);  
extern  int  doNEGATE(int  v1);  
extern  int  getVal(char  *name);