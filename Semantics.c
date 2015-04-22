/* Semantics.c
   Support and semantic action routines.
   
*/

#include <string.h>
#include <stdlib.h>
#include "CodeGen.h"
#include "Semantics.h"
#include "SymTab.h"
#include "IOManager/IOMngr.h"

extern struct SymTab *table;

/* Semantics support routines */

struct ExprRes *  doIntLit(char * digits)  { 

   struct ExprRes *res;
  
  res = (struct ExprRes *) malloc(sizeof(struct ExprRes));
  res->Reg = AvailTmpReg();
  res->Instrs = GenInstr(NULL,"li",TmpRegName(res->Reg),digits,NULL);

  return res;
}

struct ExprRes *  doRval(char * name)  { 

   struct ExprRes *res;
  
   if (!FindName(table, name)) {
		WriteIndicator(GetCurrentColumn());
		WriteMessage("Undeclared variable");
   }

  res = (struct ExprRes *) malloc(sizeof(struct ExprRes));
  res->Reg = AvailTmpReg();
  res->Instrs = GenInstr(NULL,"lw",TmpRegName(res->Reg),name,NULL);
  printf("NAME: %s\n", name);
  res->Name = name;

  return res;
}

// int isBool(struct ExprRes * Expr) {
//     struct SymEntry* entry;

//     entry = FindName(table, Expr->Instrs->Label)
// }

struct ExprRes *  doAdd(struct ExprRes * Res1, struct ExprRes * Res2)  { 

   int reg;
   
  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"add",
                                       TmpRegName(reg),
                                       TmpRegName(Res1->Reg),
                                       TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

struct ExprRes *  doSub(struct ExprRes * Res1, struct ExprRes * Res2)  { 

   int reg;
   
  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"sub",
                                       TmpRegName(reg),
                                       TmpRegName(Res1->Reg),
                                       TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

struct ExprRes *  doMult(struct ExprRes * Res1, struct ExprRes * Res2)  { 

   int reg;
   
  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"mul",
                                       TmpRegName(reg),
                                       TmpRegName(Res1->Reg),
                                       TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

struct ExprRes *  doDiv(struct ExprRes * Res1, struct ExprRes * Res2)  { 

   int reg;
   
  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"div",
                                       TmpRegName(reg),
                                       TmpRegName(Res1->Reg),
                                       TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

struct ExprRes *  doMod(struct ExprRes * Res1, struct ExprRes * Res2)  { 

   int reg;
   
  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"rem",
                                       TmpRegName(reg),
                                       TmpRegName(Res1->Reg),
                                       TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

struct ExprRes *  doExpon(struct ExprRes * Res1, struct ExprRes * Res2)  { 


  char* zeroLabel = GenLabel();
  char* loopLabel = GenLabel();
  char* finishLabel = GenLabel();
   
  int oneReg = AvailTmpReg();
  int valueReg = AvailTmpReg();


  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs, GenInstr(NULL, "li", TmpRegName(oneReg), "1", NULL));
  AppendSeq(Res1->Instrs, GenInstr(NULL, "add", TmpRegName(valueReg), TmpRegName(Res1->Reg), "$zero" ));
  //Check if zero
  AppendSeq(Res1->Instrs, GenInstr(NULL, "beq", TmpRegName(Res2->Reg), "$zero", zeroLabel));
  //Check if one
  AppendSeq(Res1->Instrs, GenInstr(loopLabel, "beq", TmpRegName(Res2->Reg), TmpRegName(oneReg), finishLabel));
  AppendSeq(Res1->Instrs, GenInstr(NULL, "mul", TmpRegName(Res1->Reg), TmpRegName(Res1->Reg), TmpRegName(valueReg)));
  AppendSeq(Res1->Instrs, GenInstr(NULL, "sub", TmpRegName(Res2->Reg), TmpRegName(Res2->Reg), TmpRegName(oneReg)));
  AppendSeq(Res1->Instrs, GenInstr(NULL, "j", loopLabel, NULL, NULL));
  AppendSeq(Res1->Instrs, GenInstr(zeroLabel, "addi", TmpRegName(Res1->Reg), "$zero", "1"));
  AppendSeq(Res1->Instrs, GenInstr(finishLabel, NULL, NULL, NULL, NULL));

  ReleaseTmpReg(oneReg);
  ReleaseTmpReg(Res2->Reg);
  ReleaseTmpReg(valueReg);
  free(Res2);
  return Res1;
}

struct ExprRes *  doNegate(struct ExprRes * Res1)  { 
   int reg;
   int reg2;
   
  reg = AvailTmpReg();
  reg2 = AvailTmpReg();
  AppendSeq(Res1->Instrs, GenInstr(NULL, "li", TmpRegName(reg2), "-1", NULL));
  AppendSeq(Res1->Instrs,GenInstr(NULL,"mul",
                                       TmpRegName(reg),
                                       TmpRegName(Res1->Reg),
                                       TmpRegName(reg2)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(reg2);
  Res1->Reg = reg;
  return Res1;
}

struct InstrSeq * doPrint(struct ExprRes * Expr, int PrintNewLine) { 

  struct InstrSeq *code;
    
  code = Expr->Instrs;
  
    AppendSeq(code,GenInstr(NULL,"li","$v0","1",NULL));
    AppendSeq(code,GenInstr(NULL,"move","$a0",TmpRegName(Expr->Reg),NULL));
    AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));

    if(PrintNewLine == 1) {
        AppendSeq(code,GenInstr(NULL,"li","$v0","4",NULL));
        AppendSeq(code,GenInstr(NULL,"la","$a0","_nl",NULL));
        AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
    } else {
        AppendSeq(code,GenInstr(NULL,"li","$v0","4",NULL));
        AppendSeq(code,GenInstr(NULL,"la","$a0","space",NULL));
        AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
    }

    ReleaseTmpReg(Expr->Reg);
    free(Expr);

  return code;
}

struct InstrSeq * doPrintBool(struct ExprRes * Expr, int PrintNewLine) {
      char* falseLabel = GenLabel();
      char* finishLabel = GenLabel();

      struct InstrSeq *code;
      code = Expr->Instrs;

      AppendSeq(code,GenInstr(NULL, "beq", TmpRegName(Expr->Reg), "$zero", falseLabel));
      AppendSeq(code,GenInstr(NULL,"li","$v0","4",NULL));
      AppendSeq(code,GenInstr(NULL,"la","$a0","True",NULL));
      AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
      AppendSeq(code,GenInstr(NULL,"j",finishLabel,NULL,NULL));
      AppendSeq(code,GenInstr(falseLabel,"li","$v0","4",NULL));
      AppendSeq(code,GenInstr(NULL,"la","$a0","False",NULL));
      AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
      AppendSeq(code,GenInstr(finishLabel,NULL,NULL,NULL,NULL));

      if(PrintNewLine == 1) {
        AppendSeq(code,GenInstr(NULL,"li","$v0","4",NULL));
        AppendSeq(code,GenInstr(NULL,"la","$a0","_nl",NULL));
        AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
     } else {
        AppendSeq(code,GenInstr(NULL,"li","$v0","4",NULL));
        AppendSeq(code,GenInstr(NULL,"la","$a0","space",NULL));
        AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
    }

      ReleaseTmpReg(Expr->Reg);
      free(Expr);
    
      return code;
}

struct InstrSeq * doPrintLine() {
    struct InstrSeq *code = GenInstr(NULL,NULL,NULL,NULL,NULL);

    AppendSeq(code,GenInstr(NULL,"li","$v0","4",NULL));
    AppendSeq(code,GenInstr(NULL,"la","$a0","_nl",NULL));
    AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));

    return code;
}

struct InstrSeq * doPrintSpaces(struct ExprRes * Expr) {
    char* loopLabel = GenLabel();
    char* finishLabel = GenLabel();
    struct InstrSeq *code;
    code = Expr->Instrs;

    int oneReg = AvailTmpReg();
    AppendSeq(code, GenInstr(NULL, "li", TmpRegName(oneReg), "1", NULL));
    AppendSeq(code, GenInstr(NULL, "blt", TmpRegName(Expr->Reg), TmpRegName(oneReg), finishLabel));
    AppendSeq(code,GenInstr(loopLabel,"li","$v0","4",NULL));
    AppendSeq(code,GenInstr(NULL,"la","$a0","space",NULL));
    AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
    AppendSeq(code,GenInstr(NULL, "sub", TmpRegName(Expr->Reg), TmpRegName(Expr->Reg), TmpRegName(oneReg)));
    AppendSeq(code,GenInstr(NULL, "bgt", TmpRegName(Expr->Reg), "$zero", loopLabel));
    AppendSeq(code,GenInstr(finishLabel, NULL, NULL, NULL, NULL));

    return code;

}

struct InstrSeq * doPrintStringLit(char* input) {
    struct InstrSeq *code;
    char* name = GenLabel();
    struct SymEntry *entry;

    EnterName(table, name, &entry); 
    SetType(entry, STRING);
    SetAttr( entry, (void*) input);

    code = GenInstr(NULL,"li","$v0","4",NULL);
    AppendSeq(code,GenInstr(NULL,"la","$a0",name,NULL));
    AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));

    return code;
}

struct InstrSeq * doPrintList(struct ExprResList * List) {
    struct InstrSeq *code = GenInstr(NULL,NULL,NULL,NULL,NULL);

    if( List == NULL) {
        return code;
    }

    while (List -> Next  != NULL) {
        if(List->isBool == 1)
            AppendSeq(code, doPrintBool(List->Expr, 0));
        else
            AppendSeq(code, doPrint(List->Expr, 0));

        List = List -> Next;
    }

    if(List->isBool == 1)
        AppendSeq(code, doPrintBool(List->Expr, 0));
    else
        AppendSeq(code, doPrint(List->Expr, 0));

    AppendSeq(code,GenInstr(NULL,"li","$v0","4",NULL));
    AppendSeq(code,GenInstr(NULL,"la","$a0","_nl",NULL));
    AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));

    return code;
}

struct InstrSeq * doPrintArr(char* input, struct ExprRes * Expr, int isBool) {
    int reg = AvailTmpReg();
    int reg2 = AvailTmpReg();

    char buffer[80];
    sprintf(buffer, "(%s)", TmpRegName(reg2));

    AppendSeq(Expr->Instrs,GenInstr(NULL,"la",TmpRegName(reg), input,NULL)); //load address of array into reg
    AppendSeq(Expr->Instrs,GenInstr(NULL,"mul",TmpRegName(reg2), TmpRegName(Expr->Reg), "4")); //multiply arrayIndex by 4. offset is reg2
    AppendSeq(Expr->Instrs,GenInstr(NULL,"add",TmpRegName(reg2), TmpRegName(reg), TmpRegName(reg2))); //reg2 becomes address of arr[idx]
    AppendSeq(Expr->Instrs,GenInstr(NULL,"lw",TmpRegName(Expr->Reg), buffer, NULL));

    ReleaseTmpReg(reg);
    ReleaseTmpReg(reg2);

    if (isBool)
        return doPrintBool(Expr,1);
    else
        return doPrint(Expr,1);
}

struct IdList * addToIDList(char * curr, struct IdList * nextItem, struct ExprRes * Res1) {
    struct IdList *list = (struct IdList *) malloc(sizeof(struct IdList));
    struct SymEntry * entry = FindName(table, curr);
    if(!entry) {
        WriteIndicator(GetCurrentColumn());
        WriteMessage("Undeclared variable");
    }

    list -> TheEntry = entry;
    list -> Next = nextItem;
    list -> Expr = Res1;

    return list;

}

struct ExprResList * addToExpressionList(struct ExprRes * curr, struct ExprResList * nextItem, int isBoolean) {
    struct ExprResList *list = (struct ExprResList *) malloc(sizeof(struct ExprResList));
    list -> Expr = curr;
    list -> Next = nextItem;
    switch(isBoolean) {
        case 0: list->isBool = 0;
            break;
        case 1: list->isBool = 1;
            break;
    }
    return list;
}

struct ExprResList * makeExprResList(struct ExprRes * Res1, int isBoolean) {
    struct ExprResList *list = (struct ExprResList *) malloc(sizeof(struct ExprResList));
    list -> Expr = Res1;
    list -> Next = NULL;
    switch(isBoolean) {
        case 0: list->isBool = 0;
            break;
        case 1: list->isBool = 1;
            break;
    }
    return list;
}

struct InstrSeq * doAssign(char *name, struct ExprRes * Expr) { 

  struct InstrSeq *code;
  

   if (!FindName(table, name)) {
		WriteIndicator(GetCurrentColumn());
		WriteMessage("Undeclared variable");
   }

  code = Expr->Instrs;
  
  AppendSeq(code,GenInstr(NULL,"sw",TmpRegName(Expr->Reg), name,NULL));

  ReleaseTmpReg(Expr->Reg);
  free(Expr);
  
  return code;
}

struct InstrSeq * doAssignArr(char *name, struct ExprRes * Res1, struct ExprRes * Res2) { 

  struct InstrSeq *code;
  int reg = AvailTmpReg();
  int reg2 = AvailTmpReg();

   if (!FindName(table, name)) {
        WriteIndicator(GetCurrentColumn());
        WriteMessage("Undeclared variable");
   }
    AppendSeq(Res1->Instrs,Res2->Instrs);
    code = Res1->Instrs;
    char buffer[80];
    sprintf(buffer, "(%s)", TmpRegName(reg2));

    AppendSeq(code,GenInstr(NULL,"la",TmpRegName(reg), name,NULL)); //load address of array into reg
    AppendSeq(code,GenInstr(NULL,"mul",TmpRegName(reg2), TmpRegName(Res1->Reg), "4")); //multiply arrayIndex by 4. offset is reg2
    AppendSeq(code,GenInstr(NULL,"add",TmpRegName(reg2), TmpRegName(reg), TmpRegName(reg2))); //reg2 becomes address of arr[idx]
    AppendSeq(code,GenInstr(NULL,"sw",TmpRegName(Res2->Reg),buffer,NULL)); //storing value into arr[idx]

  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  ReleaseTmpReg(reg);
  ReleaseTmpReg(reg2);
  free(Res1);
  free(Res2);
  
  return code;
}

extern struct ExprRes * doBoolOperation(struct ExprRes * Res1,  struct ExprRes * Res2, char* operation) {

  int reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL, operation,
                                       TmpRegName(reg),
                                       TmpRegName(Res1->Reg),
                                       TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

extern struct ExprRes *  doBoolOr(struct ExprRes * Res1,  struct ExprRes * Res2) {
  int reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL, "add",
                                       TmpRegName(reg),
                                       TmpRegName(Res1->Reg),
                                       TmpRegName(Res2->Reg)));

    AppendSeq(Res1->Instrs, GenInstr(NULL, "sgt", TmpRegName(reg), TmpRegName(reg), "$zero"));

  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

extern struct ExprRes *  doBoolAnd(struct ExprRes * Res1,  struct ExprRes * Res2) {
  int reg = AvailTmpReg();
  int reg2 = AvailTmpReg();

  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL, "add",
                                       TmpRegName(reg),
                                       TmpRegName(Res1->Reg),
                                       TmpRegName(Res2->Reg)));

  AppendSeq(Res1->Instrs, GenInstr(NULL, "addi", TmpRegName(reg2), "$zero", "2"));
    AppendSeq(Res1->Instrs, GenInstr(NULL, "seq", TmpRegName(reg), TmpRegName(reg), TmpRegName(reg2)));

  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

extern struct ExprRes *  doBoolNot(struct ExprRes * Res1) {

    char* finishLabel = GenLabel();
    char* zeroLabel = GenLabel();

  AppendSeq(Res1->Instrs, GenInstr(NULL, "beq", TmpRegName(Res1->Reg), "$zero", zeroLabel));
  AppendSeq(Res1->Instrs, GenInstr(NULL, "sub", TmpRegName(Res1->Reg), TmpRegName(Res1->Reg), TmpRegName(Res1->Reg)));
  AppendSeq(Res1->Instrs, GenInstr(NULL, "j", finishLabel, NULL, NULL));
  AppendSeq(Res1->Instrs, GenInstr(zeroLabel, "addi", TmpRegName(Res1->Reg), "$zero", "1"));
    AppendSeq(Res1->Instrs, GenInstr(finishLabel, NULL, NULL, NULL, NULL));


  return Res1;
}


extern struct BExprRes * doBExpr(struct ExprRes * Res1) {


    struct BExprRes * bRes;
    // AppendSeq(Res1->Instrs, Res2->Instrs);
    bRes = (struct BExprRes *) malloc(sizeof(struct BExprRes));
    bRes->Label = GenLabel();
    bRes->FinishLabel = GenLabel();
    bRes->LoopLabel = GenLabel();
    AppendSeq(Res1->Instrs, GenInstr(bRes->LoopLabel, NULL, NULL, NULL, NULL));
    AppendSeq(Res1->Instrs, GenInstr(NULL, "beq", TmpRegName(Res1->Reg), "$zero", bRes->Label));
    bRes->Instrs = Res1->Instrs;
    ReleaseTmpReg(Res1->Reg);
    free(Res1);
    return bRes;

}


extern struct InstrSeq * doIf(struct BExprRes * bRes, struct InstrSeq * seq) {
	struct InstrSeq * seq2;
	seq2 = AppendSeq(bRes->Instrs, seq);
	AppendSeq(seq2, GenInstr(bRes->Label, NULL, NULL, NULL, NULL));
	free(bRes);
	return seq2;
}

extern struct InstrSeq * doIfElse(struct BExprRes * bRes, struct InstrSeq * seq1, struct InstrSeq * seq2) {
    struct InstrSeq * seq3;
    seq3 = AppendSeq(bRes->Instrs, seq1);
    AppendSeq(seq3, GenInstr(NULL, "j", bRes->FinishLabel, NULL, NULL));
    AppendSeq(seq3, GenInstr(bRes->Label, NULL, NULL, NULL, NULL));
    AppendSeq(seq3, seq2);
    AppendSeq(seq3, GenInstr(bRes->FinishLabel, NULL, NULL, NULL, NULL));

    free(bRes);
    return seq3;
}

extern struct InstrSeq * doWhile(struct BExprRes * bRes, struct InstrSeq * seq) {
    struct InstrSeq * seq2;
    seq2 = AppendSeq(bRes->Instrs, seq);
    AppendSeq(seq2, GenInstr(NULL, "j", bRes->LoopLabel, NULL, NULL));
    AppendSeq(seq2, GenInstr(bRes->Label, NULL, NULL, NULL, NULL));
    free(bRes);
    return seq2;
}

struct InstrSeq * doRead(struct IdList * List) {

    struct InstrSeq *code = GenInstr(NULL,NULL,NULL,NULL,NULL);
    int reg = AvailTmpReg();
    int reg2 = AvailTmpReg();

    if( List == NULL) {
        return code;
    }

    while (List -> Next  != NULL) {
        AppendSeq(code,GenInstr(NULL,"li","$v0","5",NULL));
        AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
        if( List->TheEntry->Type == INTARR || List->TheEntry->Type == BOOLARR ) {
            char buffer[80];
            sprintf(buffer, "(%s)", TmpRegName(reg2));
            AppendSeq(code, List->Expr->Instrs);
            AppendSeq(code, GenInstr(NULL,"lw",TmpRegName(reg), List->TheEntry->Name,NULL)); //<--------_WORD NOT BEING LOADED HERE
            AppendSeq(code,GenInstr(NULL,"la",TmpRegName(reg), List->TheEntry->Name,NULL)); //load address of array into reg
            AppendSeq(code,GenInstr(NULL,"mul",TmpRegName(reg2), TmpRegName(List->Expr->Reg), "4")); //multiply arrayIndex by 4. offset is reg2
            AppendSeq(code,GenInstr(NULL,"add",TmpRegName(reg2), TmpRegName(reg), TmpRegName(reg2))); //reg2 becomes address of arr[idx]
            AppendSeq(code,GenInstr(NULL,"sw","$v0",buffer,NULL)); //storing value into arr[idx]
            
        } else {
            AppendSeq(code,GenInstr(NULL, "sw", "$v0", List->TheEntry->Name, NULL));
        }
      
        List = List -> Next;
    }
        AppendSeq(code,GenInstr(NULL,"li","$v0","5",NULL));
        AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));
        if( List->TheEntry->Type == INTARR || List->TheEntry->Type == BOOLARR ) {
            char buffer[80];
            sprintf(buffer, "(%s)", TmpRegName(reg2));
            AppendSeq(code, List->Expr->Instrs);
            AppendSeq(code,GenInstr(NULL,"la",TmpRegName(reg), List->TheEntry->Name,NULL)); //load address of array into reg
            AppendSeq(code,GenInstr(NULL,"mul",TmpRegName(reg2), TmpRegName(List->Expr->Reg), "4")); //multiply arrayIndex by 4. offset is reg2
            AppendSeq(code,GenInstr(NULL,"add",TmpRegName(reg2), TmpRegName(reg), TmpRegName(reg2))); //reg2 becomes address of arr[idx]
            AppendSeq(code,GenInstr(NULL,"sw","$v0",buffer,NULL)); //storing value into arr[idx]
            
        } else {
            AppendSeq(code,GenInstr(NULL, "sw", "$v0", List->TheEntry->Name, NULL));
        }

        ReleaseTmpReg(reg);
        ReleaseTmpReg(reg2);

    return code;

}

struct ExprRes * loadFromArr(char * name, struct ExprRes * Expr) {
    int reg = AvailTmpReg();
    int reg2 = AvailTmpReg();

    char buffer[80];
    sprintf(buffer, "(%s)", TmpRegName(reg2));

    AppendSeq(Expr->Instrs,GenInstr(NULL,"la",TmpRegName(reg), name,NULL)); //load address of array into reg
    AppendSeq(Expr->Instrs,GenInstr(NULL,"mul",TmpRegName(reg2), TmpRegName(Expr->Reg), "4")); //multiply arrayIndex by 4. offset is reg2
    AppendSeq(Expr->Instrs,GenInstr(NULL,"add",TmpRegName(reg2), TmpRegName(reg), TmpRegName(reg2))); //reg2 becomes address of arr[idx]
    AppendSeq(Expr->Instrs,GenInstr(NULL,"lw",TmpRegName(Expr->Reg), buffer, NULL));
    Expr->Name = strdup(name);

    ReleaseTmpReg(reg);
    ReleaseTmpReg(reg2);
    return Expr;
}


void							 
Finish(struct InstrSeq *Code)
{ struct InstrSeq *code;
  struct SymEntry *entry;
  struct Attr * attr;


  code = GenInstr(NULL,".text",NULL,NULL,NULL);
  //AppendSeq(code,GenInstr(NULL,".align","2",NULL,NULL)); <-----------GAVE ERROR
  AppendSeq(code,GenInstr(NULL,".globl","main",NULL,NULL));
  AppendSeq(code, GenInstr("main",NULL,NULL,NULL,NULL));
  AppendSeq(code,Code);
  AppendSeq(code, GenInstr(NULL, "li", "$v0", "10", NULL)); 
  AppendSeq(code, GenInstr(NULL,"syscall",NULL,NULL,NULL));
  AppendSeq(code,GenInstr(NULL,".data",NULL,NULL,NULL));
  AppendSeq(code,GenInstr(NULL,".align","4",NULL,NULL));
  AppendSeq(code,GenInstr("_nl",".asciiz","\"\\n\"",NULL,NULL));
  AppendSeq(code,GenInstr("True",".asciiz","\"true\"",NULL,NULL));
    AppendSeq(code,GenInstr("False",".asciiz","\"false\"",NULL,NULL));
  AppendSeq(code,GenInstr("space",".asciiz","\" \"",NULL,NULL));

 entry = FirstEntry(table);
 while (entry) {
             if(entry->Type == STRING) {
                AppendSeq(code,GenInstr((char *) GetName(entry),".asciiz", (char*) GetAttr(entry),NULL,NULL));
             } else if (entry->Type == INTARR || entry->Type == BOOLARR){
                char buffer[80];
                sprintf(buffer, "0:%s", (char*)GetAttr(entry));
	   AppendSeq(code,GenInstr((char *) GetName(entry),".word",buffer,NULL,NULL));
            } else {
                AppendSeq(code,GenInstr((char *) GetName(entry),".word","0",NULL,NULL));
            }
        entry = NextEntry(table, entry);
 }
  PrintTable(table);
  WriteSeq(code);
  
  return;
}




