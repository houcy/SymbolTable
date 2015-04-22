/* Semantics.h
   The action and supporting routines for performing semantics processing.
*/

/* Semantic Records */
struct IdList {
  struct SymEntry * TheEntry;
  struct IdList * Next;
  struct ExprRes *Expr;
};

struct ExprRes {
  int Reg;
  struct InstrSeq * Instrs;
  char* Name;
};

struct ExprResList {
	struct ExprRes *Expr;
	struct ExprResList * Next;
             int isBool;
};

struct BExprRes {
  char * Label;
  char * FinishLabel;
  char * LoopLabel;
  struct InstrSeq * Instrs;
};


/* Semantics Actions */

extern struct IdList * addToIDList(char * curr, struct IdList * nextItem, struct ExprRes * Res1);
// extern struct IdList * addArrToIDLIst(char * curr, struct ExprRes * Res1, struct IdList * nextItem);

extern struct ExprRes *  doIntLit(char * digits);
extern struct ExprRes *  doRval(char * name);
extern struct InstrSeq *  doAssign(char * name,  struct ExprRes * Res1);
extern struct InstrSeq * doAssignArr(char *name, struct ExprRes * Res1, struct ExprRes * Res2);

extern struct ExprRes *  doAdd(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doSub(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doMult(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doDiv(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doMod(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doExpon(struct ExprRes * Res1, struct ExprRes * Res2);
extern struct ExprRes *  doNegate(struct ExprRes * Res1);

extern struct BExprRes * doBExpr (struct ExprRes * Res1);//struct ExprRes * Res2, char* operation);
extern struct ExprRes *  doBoolOr(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doBoolAnd(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doBoolNot(struct ExprRes * Res1);
extern struct ExprRes * doBoolOperation(struct ExprRes * Res1,  struct ExprRes * Res2, char* operation);
extern struct InstrSeq * doIf(struct BExprRes *bRes, struct InstrSeq * seq);
extern struct InstrSeq * doIfElse(struct BExprRes *bRes, struct InstrSeq * seq1, struct InstrSeq * seq2);
extern struct InstrSeq * doWhile(struct BExprRes * bRes, struct InstrSeq * seq);
extern struct InstrSeq * doRead(struct IdList * List);

extern struct ExprRes * loadFromArr(char * name, struct ExprRes * Expr);

extern struct InstrSeq *  doPrint(struct ExprRes * Expr, int PrintNewLine);
extern struct InstrSeq * doPrintList(struct ExprResList * List);
extern struct InstrSeq * doPrintBool(struct ExprRes * Expr, int PrintNewLine);
extern struct InstrSeq * doPrintSpaces(struct ExprRes * Expr);
extern struct InstrSeq * doPrintLine();
extern struct InstrSeq * doPrintStringLit(char* input);
extern struct InstrSeq * doPrintArr(char* input, struct ExprRes * Expr, int isBool);

extern struct ExprResList * makeExprResList(struct ExprRes * Res1, int isBoolean);
extern struct ExprResList * addToExpressionList(struct ExprRes * curr, struct ExprResList * nextItem, int isBoolean);


extern void	Finish(struct InstrSeq *Code);
