#ifndef _GLOBAL
#define _GLOBAL


bool parse_successful = false;
enum token {LParent, RParent, lCurlyt, rCurlyt, lIndext,
			rIndext, semiColont, periodt, commat, stringLiteralt,
			emptyStringLiteralt, SingleQuotet, AddOpt, MulOpt, SignOpT, RelOpt, 
			AssignOpt, idt, invalidt, endofcommentt, NUMt,intt, floatt, chart, returnt, inT, outT, endlT};
char resWords[100][100] = {"if", "else", "while", "float", "int", "char", "break", "continue", "void" , "const"}; //valid keywords
char type[20][20] = {"int","float","char"};	
char vs[50] = "()[]{}+-%,|%&*!=/<>;/'\""; //valid symbols
char sp[50] = "(){};,=*+/-!><&|\""; //valid symbols
char buffer[255],previous[255];
int line = 1,error = 0;
const int MAXSIZE = 1000;
const int MAXSTRING = 1000;
int tmp_i =0;
ifstream in;
char fname[100], procName[50],stringLit[255],asmCode[10000];
ofstream out, writeAsm;
#endif
