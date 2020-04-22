#ifndef SYM_H
#define SYM_H

const int TableSize = 211; //use a prime number
int DEPTH=1;
int varDec;
enum VarType {charType, intType, floatType, literalType };

enum EntryType {constEntry, varEntry, functionEntry};

struct ParamNode;

typedef ParamNode * ParamPtr;

struct ParamNode {
  VarType typeOfParameter;
  char nameOfParameter[255];
  ParamPtr Next;
  int depth;
};

struct TableEntry;

typedef TableEntry * EntryPtr; //pointer to actual table entry

struct TableEntry {
  char Token[255];
  char Lexeme[255];
  char LexemeBP[255];
  int depth;
  int varDec;
  EntryType TypeOfEntry; // tag field for the union
  union {
    struct {
      VarType TypeOfVariable;
      int Offset;
      int size;
    } var;
    struct {
      VarType TypeOfConstant; //int or float constant
      int Offset;
      union {
        int Value;
        float ValueR;
      };
    } constant;
    struct { //this is the entry for a function
      int SizeOfLocal;
      int NumberOfParameters;
	  int SizeOfParameters;
      VarType ReturnType;
      ParamPtr ParamList; //linked list of paramter types
    } function;
  }; //end of union
 EntryPtr next; //points to next list item
};

//function prototypes for symbol table routines

extern EntryPtr lookup(char*);
extern void insert(char*, char*, int, int);
extern void writeTable(int);
extern void deleteDepth(int);
extern void InitTable();
extern VarType getType(char t[]);
extern VarType checkValueType (char val[]);
extern int hash(char* lexeme);
extern char* getTypeFromId(VarType);
extern string getTypeFromId(int);

#endif
