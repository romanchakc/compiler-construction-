/***********************************************************************
*** NAME: Romancha Khatri											 ***
*** CLASS:  CSC 446													 ***
*** ASSIGNMENT:	#8													 ***	
*** DUE DATE: 05/04/2018 5:00PM										 ***
*** INSTRUCTOR:	G. HAMER											 ***
***********************************************************************/
#ifndef _PARSER
#define _PARSER

int off_1 =0, off_2, off_temp; // global variable for variable offset
EntryPtr lookUpPtr; // EntryPtr reference
char tkn[10] = "idt";
char lexeme_temp[255] = ""; //temp lexeme variable
char type_temp[10] = "";
int nOfParam = 0,size_of_params=0;
char nameOfParameter_temp[100];
int sizeOfLocal_temp = 0;
char CONSTT[10]="constt";
char tmpUnaryop[50] = "", code_for_unary[100];
EntryPtr tp,tmpptr_unary;
int tmp_unary=0,check_return = 0;

stack <string> st;

EntryType checkEntType();
void checkEntry();
void checkMain();
void VariableInsertion(char type_temp[], char lex_temp[], int &i);
void fcTAC();
void emit(char *code);
void createTemp(EntryPtr &tmpptr);
void makeTempVar(EntryPtr &idptr, char *code);
void setBP(EntryPtr setBPptr,char *code);
void CheckIdt();
EntryPtr createBP(char lex_input[]);
void createTemplateASM(char file_tac[], char file_asm[]);
EntryPtr insertLiteral(char[]);
void acm_generate(string fdetail);

class parse 
{	
	public:		void PROG();
				bool TYPE(int c);
				void REST();
				void PARAMETERLIST();
				void PARAMATAIL();
				void COMPOUND();
				void STAT_LIST();
				void SimpleExpr(EntryPtr &Eplace);
				void DECL();
				void IDLIST();
				void IDTAIL();
				void MATCH(token t );
				void process();
				void Term(EntryPtr &Tplace);
				void MoreTerm(EntryPtr &Rplace);
				void MoreFactor(EntryPtr &Rplace);
				void Factor(EntryPtr &Tplace);
				void RET_STAT();
				void AssignStat();
				void FuncCall(int);
				void PARAMS(int &tmp_params);
				void PARAMSTAIL(int &tmp_params);
				void In_Stat();
				void Out_Stat();
				void In_End();
				void Out_End();
				void OutOptions();
}; // parse

/*** FUNCTION <MATCH>			***************
*** DESCRIPTION: <matching valid inputs>	***
*** INPUT ARGS:  <enum word  >				***
*** OUTPUT ARGS:							***	
*** IN/OUT ARGS:							***
*** RETURN: <>								**/

void parse::MATCH(token t )
{
		if(t == commat)
		{
			if(strcmp(buffer,",") == 0) return;			
			else errorReport(" Expected comma \',\' ");
		}
		else if(t == semiColont)
		{
			if(strcmp(buffer,";") == 0) return;			
			else errorReport(" Expected semicolon \';\' ");
		}
		else if(t == lCurlyt)
		{
			if(strcmp(buffer,"{") == 0)	return;			
			else errorReport(" Expected left curly brace \'{\' ");
		}
		else if(t == rCurlyt)
		{
			if(strcmp(buffer,"}") == 0)	return;			
			else errorReport(" Expected right curly brace \'}\' ");
		}
		else if(t == LParent)
		{
			if(strcmp(buffer,"(") == 0) return;			
			else errorReport(" Expected left parenthesis \'(\' ");
		}
		else if(t == RParent)
		{
			if(strcmp(buffer,")") == 0)	return;			
			else errorReport(" Expected right parenthesis \')\' ");
		}
		else if(t == AssignOpt)
		{
			if(strcmp(buffer,"=") == 0) return;			
			else errorReport(" Expected assignment operator \'=\' ");
		}
		else if(t == SignOpT)
		{
			if(strcmp(buffer,"!") == 0 || strcmp(buffer,"-") == 0) return;
			else errorReport(" Expected signOpT \'!\' or \'-\' ");
				
		}
		else if(t == AddOpt)
		{
			if(strcmp(buffer,"+") == 0 || strcmp(buffer,"-") == 0 || strcmp(buffer,"||")==0) return;
			else errorReport(" Expected AddOpt + , - or || ");
		}
		else if(t == MulOpt)
		{
			if(strcmp(buffer,"*") == 0 || strcmp(buffer,"/") == 0 || strcmp(buffer,"&&")==0) return;
			else errorReport(" Expected MulOpt * ,  / or && ");
		}
		else if(t == returnt)
		{
			if(strcmp(buffer,"return") == 0) return;
			else errorReport(" Expected \'return\' ");
		}
		else if(t == inT)
		{
			if(strcmp(buffer,">>") == 0) return;
			else errorReport(" Expected >> ");
		}
		else if(t == outT)
		{
			if(strcmp(buffer,"<<") == 0) return;
			else errorReport(" Expected << ");
		}
		else if(t == endlT)
		{
			if(strcmp(buffer,"endl") == 0) return;
			else errorReport(" Expected endl ");
		}
		else if(t == idt) matchId(); // check valid identifier
		else if(t == NUMt) checkDigits(buffer);
} //MATCH



/*** FUNCTION <PROG>						***************
*** DESCRIPTION: < Checking Grammar rules				***
*** INPUT ARGS:  < >									***
*** OUTPUT ARGS: < >									***	
*** IN/OUT ARGS: < >									***
*** RETURN: < >											**/

// PROG -> TYPE  idt  REST PROG | const idt = num ; PROG | $
void parse::PROG()
{
	varDec = 0;
	if(strcmp(buffer,"const") == 0)
	{
		int i = 0;
		char value_temp[50] = "", tkn[10] = "constt";
		EntryPtr lptr;
		
		getToken();
		MATCH(idt);
		strcpy(lexeme_temp,""); // set lexeme_temp empty
		strcpy(lexeme_temp, buffer);
		insert(lexeme_temp,tkn,DEPTH,varDec);
		lptr = lookup(lexeme_temp);
		lptr->TypeOfEntry = constEntry;
		
		getToken();
		MATCH(AssignOpt);
		getToken();
		MATCH(NUMt);
		strcpy(value_temp,buffer);
		if(checkValueType(value_temp) == intType)
			{
                lptr->constant.TypeOfConstant = intType;
                lptr->constant.Value = atoi(value_temp);
                lptr->constant.Offset = i;
                i+=2;
            }
        else
            {
                lptr->constant.TypeOfConstant = floatType;
                lptr->constant.ValueR = atof(value_temp);
                lptr->constant.Offset = i;
                i+=4;
            }
				
		getToken();
		MATCH(semiColont);
		
		getToken();
		if(in.eof())	{ parse_successful = true; }
		else			PROG();	
	}
	else
	{
	TYPE(1);  // verifying the type
	strcpy(type_temp,buffer);
	getToken();
	MATCH(idt); //match id
	strcpy(lexeme_temp , buffer); //store lexeme in temp variable
	
	getToken();
	REST(); 
	
	getToken();
	if(in.eof())	{ parse_successful = true;}
	else			PROG();	
	}
} //PROG

// TYPE -> int | float | char
bool parse::TYPE(int c)
{
	bool check = false;
	for(int i=0;i<3;i++)
	{
		if(strcmp(buffer,type[i])==0)
		{
			check = true;
			break;
		}	
	}
	if(check == false && c == 1)
		errorReport(" Expected valid TYPE (int, float, or char) ");
return check;
} // TYPE

// REST -> ( PARAMLIST ) COMPOUND |IDTAIL  ;  PROG
void parse::REST()
{
	char code[MAXSIZE] = "";
	if(strcmp(buffer,"(") == 0)
	{
		varDec = 1;
		MATCH(LParent);
		off_2 = 0;
		
		strcpy(procName,"");
		strcpy(procName,lexeme_temp);
		strcat(code,"PROC             ");
		strcat(code,procName);
		emit(code);
					
		insert(lexeme_temp,tkn,DEPTH,varDec);
		lookUpPtr = lookup(lexeme_temp); 
		lookUpPtr->TypeOfEntry = functionEntry;
		lookUpPtr->function.ReturnType = getType(type_temp);
				
		getToken();
		DEPTH++;
		
		if(TYPE(2)) 
			PARAMETERLIST();
		
		MATCH(RParent);
		varDec = 0;
		lookUpPtr->function.SizeOfParameters = size_of_params;
		size_of_params = 0;
		getToken();
		COMPOUND();		
	}
	else
	{
		VariableInsertion(type_temp, lexeme_temp,  off_1); //inserting variables with details   // issue here regarding offset 1 and offset 2
		if(strcmp(buffer, ",") == 0) 
			IDTAIL();
		MATCH(semiColont);
		getToken();
		if(!in.eof())
			PROG();
	}
} // REST

// PARAMETERLIST -> TYPE  idt  PARAMTAIL
void parse::PARAMETERLIST()
{
	TYPE(1); // verifying the type
	
	ParamPtr paramPtr = new ParamNode();
	paramPtr->typeOfParameter = getType(buffer); //insert type
	char type_store[10];
	strcpy(type_store,buffer);
	
	if(strcmp(type_store,"int") == 0) size_of_params = size_of_params + 2;
	else if(strcmp(type_store,"float") == 0) size_of_params = size_of_params + 4;
	else if(strcmp(type_store,"char") == 0) size_of_params = size_of_params + 1;
	
	getToken();
	MATCH(idt);
	
	strcpy(paramPtr->nameOfParameter, buffer); //insert name of parameter
	paramPtr->depth = DEPTH; // depth for formal parameters
	VariableInsertion(type_store, paramPtr->nameOfParameter, off_2);
	
	if(lookUpPtr->function.ParamList != NULL) // check if any parameters exist before
		{
		ParamPtr current = lookUpPtr->function.ParamList;
			while(current->Next != NULL) // move forward until last parameter
                current = current->Next;
			current->Next = paramPtr;
		}
		else
		{
		paramPtr->Next = NULL;
		lookUpPtr->function.ParamList = paramPtr;
		}
		lookUpPtr->function.NumberOfParameters = lookUpPtr->function.NumberOfParameters+1;
		
	
	getToken();
	if(strcmp(buffer, ",") == 0)
	PARAMATAIL();
} //PARAMETERLIST

// PARAMATAIL ->  , TYPE idt PARAMTAIL
void parse::PARAMATAIL()
{
	MATCH(commat);
	getToken();
	TYPE(1);  // verifying the type
	
	ParamPtr paramPtr = new ParamNode();
	paramPtr->typeOfParameter = getType(buffer); //insert type
	char type_store[10];
	strcpy(type_store,buffer);
	
	if(strcmp(type_store,"int") == 0) size_of_params = size_of_params + 2;
	else if(strcmp(type_store,"float") == 0) size_of_params = size_of_params + 4;
	else if(strcmp(type_store,"char") == 0) size_of_params = size_of_params + 1;
	
	
	getToken();
	MATCH(idt);
	strcpy(paramPtr->nameOfParameter, buffer); //insert name of parameter
	paramPtr->depth = DEPTH; // depth for formal parameters
	VariableInsertion(type_store, paramPtr->nameOfParameter, off_2);
	
	if(lookUpPtr->function.ParamList != NULL) // check if any parameters exist before
		{
		ParamPtr current = lookUpPtr->function.ParamList;
			while(current->Next != NULL) // move forward until last parameter
                current = current->Next;
			current->Next = paramPtr;
		}
	else
	{
	paramPtr->Next = NULL;
	lookUpPtr->function.ParamList = paramPtr;
	}
	lookUpPtr->function.NumberOfParameters = lookUpPtr->function.NumberOfParameters+1;
		
	getToken();
	if(strcmp(buffer, ",") == 0)
		PARAMATAIL();
	
} //PARAMATAIL

// IDTAIL ->  , idt IDTAIL
void parse::IDTAIL()
{
MATCH(commat);
if(strcmp(type_temp,"int")==0) sizeOfLocal_temp = sizeOfLocal_temp+2;	
else if(strcmp(type_temp,"float")==0) sizeOfLocal_temp = sizeOfLocal_temp+4;	
else sizeOfLocal_temp = sizeOfLocal_temp+1;

getToken();
MATCH(idt);

strcpy(lexeme_temp , buffer);
VariableInsertion(type_temp,lexeme_temp, off_1); //inserting variables with details

getToken();
if(strcmp(buffer,",") == 0)
	IDTAIL();	
MATCH(semiColont);
} //IDTAIL

// COMPOUND ->   { DECL  STAT_LISTRET_STAT }
void parse::COMPOUND()
{
	char code[MAXSIZE] = "";
	MATCH(lCurlyt);
	varDec = 2;
	off_temp = off_1;
	off_1 = 0;
	sizeOfLocal_temp = 0;
	getToken();
	DECL();
	lookUpPtr->function.SizeOfLocal = sizeOfLocal_temp;

	if(strcmp(buffer,"return") == 0)
	RET_STAT();
	else if(strcmp(buffer,"}")!=0) { tmp_i = sizeOfLocal_temp+2; STAT_LIST();}
	MATCH(rCurlyt);
	out<<"endP		"<<procName<<"\r\n"<<"\r\n";
	varDec = 0;
	off_1= off_temp;
	writeTable(DEPTH); // display contents for current depth
	deleteDepth(DEPTH); // delete all lexemes inside functionType
	DEPTH--;
} //COMPOUND

// DECL -> TYPE  IDLIST | const idt = num ; DECL
void parse::DECL()
{ 
	if(strcmp(buffer,"const") == 0) // check for constant
	{
		EntryPtr lptr;
		
		getToken();
		MATCH(idt);
			
		strcpy(lexeme_temp, buffer);
		insert(lexeme_temp, CONSTT, DEPTH, varDec);
		lptr = lookup(lexeme_temp);
		lptr->TypeOfEntry = constEntry;
		
		getToken();
		MATCH(AssignOpt);
		getToken();
		MATCH(NUMt);
		
		if(checkValueType(buffer) == intType)
		{
			lptr->constant.TypeOfConstant = intType;
			lptr->constant.Value = atoi(buffer);
		}
		else
		{
		lptr->constant.TypeOfConstant = floatType;
		lptr->constant.ValueR = atof(buffer);
		}	
		
		getToken();
		MATCH(semiColont);
		getToken();
		DECL();
	}
	else if(TYPE(2)) // check if valid type or not
	{
	TYPE(1);  // verifying the type
	
	strcpy(type_temp,buffer); // store type
	if(strcmp(type_temp,"int")==0) sizeOfLocal_temp = sizeOfLocal_temp+2;	
	else if(strcmp(type_temp,"float")==0) sizeOfLocal_temp = sizeOfLocal_temp+4;	
	else sizeOfLocal_temp = sizeOfLocal_temp+1;	

	getToken();
	IDLIST();
	}
} //DECL

// IDLIST -> idt  IDTAIL ; DECL
void parse::IDLIST()
{
	MATCH(idt);
	strcpy(lexeme_temp , buffer); //store lexeme in temp variable
	VariableInsertion(type_temp, lexeme_temp,  off_1); // insert details
	
	getToken();
	if(strcmp(buffer,",") == 0)
		IDTAIL();
	MATCH(semiColont);
	getToken();
	DECL();
} //IDLIST


// AssignStat -> idt = Expr | idt = FuncCall
void parse::AssignStat()
{
	MATCH(idt);
	
	char code[MAXSIZE],lex1[20];
	EntryPtr idptr ;
	EntryPtr tmp;
	EntryPtr Eplace;
	idptr = lookup(buffer);
	if(idptr == NULL)
	errorReport(" Undeclared variable ! Expected declared variable");
	if(idptr){
		if(idptr->TypeOfEntry == constEntry) // check if const entry
			errorReport(" Constant cannot be modified, Expected valid variable Entry");
		else if(idptr->TypeOfEntry == functionEntry) // check if function entry
			errorReport(" FunctionEntry cannot be assigned, Expected valid variable Entry");
			}
	
	strcpy(lex1,buffer);
	getToken();
	MATCH(AssignOpt);
	getToken();
	// code to check whether function call or not!
	tmp = lookup(buffer);
	if(tmp && tmp->TypeOfEntry == functionEntry)
	{
		st.push(lex1);
		st.push("=");
		int n_params = tmp->function.NumberOfParameters;
			FuncCall(n_params);
	}
	else
	{
	SimpleExpr(Eplace); // synthesize a ptr to expression
	setBP(idptr,code);
	strcat(code,"   =   ");
	strcat(code, Eplace->LexemeBP);
	//setBP(Eplace,code);
	emit(code);
	
	}
}

// STAT_LIST ->	Statement ; STAT_LIST | $
void parse:: STAT_LIST()
{
	if(strcmp(buffer,"cin") == 0) In_Stat();
	else if(strcmp(buffer,"cout") == 0) Out_Stat();
	else AssignStat();
	MATCH(semiColont);
	getToken();
	if(strcmp(buffer,"}")!=0 && strcmp(buffer,"return") !=0 )
	STAT_LIST();
	else if(strcmp(buffer,"return")==0)
	RET_STAT();
} // STAT_LIST

// In_Stat -> cin >> idt In_End
void parse::In_Stat()
{
		getToken();
		MATCH(inT);
		getToken();
		matchId();
		CheckIdt(); // varify idt is declared or not
		EntryPtr tmp1 = createBP(buffer);
		if(tmp1->var.TypeOfVariable == intType)
		{
		//cout<<"\nrdi       "<<tmp1->LexemeBP<<endl;
		out<<"\nrdi       "<<tmp1->LexemeBP<<"\r\n"; //TAC
		}
		else if(tmp1->var.TypeOfVariable == charType)
		{
		//cout<<"\nrdc       "<<tmp1->LexemeBP<<endl;
		out<<"\nrdc       "<<tmp1->LexemeBP<<"\r\n"; //TAC
		}
		getToken();
		if(strcmp(buffer,">>") == 0)
		In_End();
} // In_Stat

// In_End -> >> idt In_End | $
void parse::In_End()
{
	MATCH(inT);
	getToken();
	matchId();
	CheckIdt(); 
	EntryPtr tmp1 = createBP(buffer);
	if(tmp1->var.TypeOfVariable == intType)
	{
	//cout<<"\nrdi       "<<tmp1->LexemeBP<<endl;
	out<<"\nrdi       "<<tmp1->LexemeBP<<"\r\n"; //TAC
	}
	else if(tmp1->var.TypeOfVariable == charType)
	{
	//cout<<"\nrdc       "<<tmp1->LexemeBP<<endl;
	out<<"\nrdc       "<<tmp1->LexemeBP<<"\r\n"; //TAC
	}
	getToken();
	if(strcmp(buffer,">>") == 0)
		In_End();
} // In_End

// Out_Stat -> cout << OutOptions Out_End
void parse::Out_Stat()
{
	getToken();
	MATCH(outT);
	getToken();
	OutOptions();
	if(strcmp(buffer,"<<") == 0)
	Out_End();
} //Out_Stat

// Out_End -> << OutOptions Out_End | $
void parse::Out_End()
{
	MATCH(outT);
	getToken();
	OutOptions();
	if(strcmp(buffer,"<<") == 0)
	Out_End();
} // Out_End

// OutOptions -> idt | Literal | endl
void parse::OutOptions()
{
		if(strcmp(buffer,"endl")==0)
		{
			MATCH(endlT);
			out<<"\ncall writeln"<<"\r\n";
		}
		else if(processWordToken(buffer) == idt)
		{
			MATCH(idt);
			checkEntry();
			EntryPtr tmp1 = createBP(buffer);
			if(tmp1->var.TypeOfVariable == intType)
			{
			out<<"\nwri       "<<tmp1->LexemeBP<<"\r\n";
			}
			else if(tmp1->var.TypeOfVariable == charType)
			{
			out<<"\nwrc       "<<tmp1->LexemeBP<<"\r\n";
			}
		}
		else if(strcmp(buffer,"\"") == 0)
		{
			char tmp;
			int i=0;
			strcpy(stringLit,"");
			tmp = in.get();
			while(tmp != '"' && tmp != '\n' )
			{
			stringLit[i] = tmp;
			tmp = in.get();
			i++;
			}
			stringLit[i] = '\0';
			//cout<<stringLit;
			if(tmp == '\n' || tmp== '\0')
					errorMsg(" Unterminated String literal found, expected valid string literal");
			else
			{
			//cout<<"wrs      "<<stringLit<<endl;
			EntryPtr litTmp = insertLiteral(stringLit);
			out<<"wrs       "<<litTmp->Token<<"\r\n";
			}
		} // string literal
		else
			errorReport("Expected idt, literal or endl tokens");
		getToken();
} // OutOptions

//SimpleExpr ->	SignOp Term MoreTerm
void parse::SimpleExpr(EntryPtr &Eplace)
{
	EntryPtr Tplace;
	tmp_unary = 0;
	strcpy(tmpUnaryop,""); strcpy(code_for_unary, "");
	if(strcmp(buffer,"-")==0 || strcmp(buffer,"!")==0){
	strcpy(tmpUnaryop,buffer);
	tmp_unary = 1;
	MATCH(SignOpT);  // SignOp	->	! | - | $
	createTemp(tmpptr_unary);
	strcpy(code_for_unary,tmpptr_unary->LexemeBP);
	strcat(code_for_unary, "   =   ");
	strcat(code_for_unary,tmpUnaryop);
	getToken(); 
	}
	Term(Tplace);
	MoreTerm(Tplace);
	Eplace = Tplace; // return Eplace
} // SimpleExpr

//Term ->	Factor  MoreFactor
void parse::Term(EntryPtr &Tplace)
{
	Factor(Tplace);
	getToken();
	if(strcmp(buffer,"*")==0 || strcmp(buffer,"/")==0|| strcmp(buffer,"&&")==0)
	MoreFactor(Tplace);
} //Term

//MoreTerm		->	Addop Term MoreTerm |	$
void parse::MoreTerm(EntryPtr &Rplace)
{
	EntryPtr Tplace;
	EntryPtr tmpptr, tmpptr1;
	char op[2];
	char code[MAXSTRING] = "";
	if(tmp_unary == 1)
	{
	strcat(code_for_unary,Rplace->LexemeBP);
	emit(code_for_unary);
	Rplace = tmpptr_unary;
	tmp_unary=0;
	strcpy(code_for_unary,"");
	}
	
	if(check_return == 1)
	{
		strcat(code,"ax");
		strcat(code,"     =   ");
		strcat(code,Rplace->LexemeBP);
		check_return = 0;
		emit(code);
	}
	if(strcmp(buffer,";") == 0)
	{
	createTemp(tmpptr);
	strcpy(code,tmpptr->LexemeBP);
	strcat(code, "   =   ");
	strcat(code, Rplace->LexemeBP);
	//setBP(Rplace,code);
	Rplace = tmpptr;
	emit(code);
	}
	
	MT: if(strcmp(buffer,"+")==0 || strcmp(buffer,"-")==0|| strcmp(buffer,"||")==0)
	{// Addop	->	+ | - | '||'
	strcpy(op,buffer); // store operator
	getToken(); // read next value
	Term(Tplace);
	strcpy(code,"");
	
	createTemp(tmpptr);
	strcat(code, tmpptr->LexemeBP);
	strcat(code,"   =   ");
	strcat(code,Rplace->LexemeBP);
	strcat(code,"   ");
	strcat(code,op);
	strcat(code,"   ");
	strcat(code, Tplace->LexemeBP);
	//setBP(Tplace,code);
	Rplace = tmpptr; // Let R inherits
	emit(code);
	goto MT;
	}
} //MoreTerm

//MoreFactor ->  Mulop Factor MoreFactor | 
void parse::MoreFactor(EntryPtr &Rplace)
{
	EntryPtr Tplace;
	EntryPtr tmpptr;
	char op[2];
	char code[MAXSTRING] = "";
	
	if(strcmp(buffer,";") == 0)
	{
	createTemp(tmpptr);
	strcpy(code,tmpptr->LexemeBP);
	strcat(code, "   =   ");
	strcat(code, Rplace->LexemeBP);
	//setBP(Rplace,code);
	Rplace = tmpptr;
	emit(code);
	}
	MF:if(strcmp(buffer,"*")==0 || strcmp(buffer,"/")==0|| strcmp(buffer,"&&")==0)
	{ // Mulop	-> 	*  | /  | &&
	strcpy(op,buffer); // store operator
	
	getToken();
	Factor(Tplace);
	strcpy(code,"");
	createTemp(tmpptr);
	strcat(code, tmpptr->LexemeBP);
	strcat(code,"   =   ");
	strcat(code,Rplace->LexemeBP);
	strcat(code,"   ");
	strcat(code,op);
	strcat(code,"   ");
	strcat(code, Tplace->LexemeBP);
	//setBP(Tplace,code);
	Rplace = tmpptr; // Let R inherits
	emit(code);
	getToken();
	goto MF;
	}
} // MoreFactor

// Factor -> id | num | ( Expr )
void parse::Factor(EntryPtr &Tplace)
{
	if(strcmp(buffer,"(")==0) // ( Expr )
	{
		MATCH(LParent);
		getToken();
		SimpleExpr(Tplace);
		MATCH(RParent);
	}
	else if(processWordToken(buffer) == idt)
	{
		MATCH(idt);  // idt
		checkEntry();
		Tplace = lookup(buffer);
		if(Tplace)
		{
			char tmpLx[50] = "", cd[50] = "";
			if(Tplace->TypeOfEntry == varEntry)
			{
			//sprintf(tmpLx,"%d",Tplace->var.Offset);
			if(Tplace->depth == 1)
			strcpy(Tplace->LexemeBP,Tplace->Lexeme);
			if(Tplace->depth == 2 && Tplace->varDec == 1) // params
			{
				sprintf(tmpLx,"%d",Tplace->var.Offset+4);
				strcat(cd,"[bp+");
				strcat(cd,tmpLx);
				strcat(cd,"]");
				strcpy(Tplace->LexemeBP,cd);
			}
			else if(Tplace->depth == 2 && Tplace->varDec == 2) //local
			{
				sprintf(tmpLx,"%d",Tplace->var.Offset+2);
				strcat(cd,"[bp-");
				strcat(cd,tmpLx);
				strcat(cd,"]");
				strcpy(Tplace->LexemeBP,cd);
			}
			}
			else if(Tplace->TypeOfEntry == constEntry )
			{
				if(Tplace->constant.TypeOfConstant == intType)
				{		
					sprintf(tmpLx,"%d",Tplace->constant.Value);
					strcpy(Tplace->LexemeBP, tmpLx );
				}
				else if(Tplace->constant.TypeOfConstant == floatType)
				{
					sprintf(tmpLx,"%f",Tplace->constant.ValueR);
					strcpy(Tplace->LexemeBP, tmpLx );
				}
			}
		}
		
		
	}
	else if(processWordToken(buffer) == NUMt) 
	{
		MATCH(NUMt); // numt
		tp = new TableEntry();
		strcpy(tp->LexemeBP, buffer);
		Tplace = tp;
	}
	else errorReport("Expected valid IDT or NUMT or left parenthesis ");
} // Factor

// RET_STAT -> return Expr ;
void parse:: RET_STAT()
{
	check_return = 1;
	EntryPtr Eplace;
	MATCH(returnt);
	getToken();
	SimpleExpr(Eplace);
	MATCH(semiColont);
	getToken();
} //RET_STAT

//FuncCall	->	idt ( PARAMS )
void parse::FuncCall(int n_params)
{
	int tmp_params=0;
	st.push(buffer);
	getToken();
	MATCH(LParent);
	getToken();
	PARAMS(tmp_params);
	MATCH(RParent);	
	if(n_params != tmp_params)
	{
		cout<<"At line: "<<line<<","<<"The Number of parameters does not match for a function call, Expected "<<n_params<<" but found "<<tmp_params<<endl<<endl;
		exit(0);		
	}
	fcTAC();
	getToken();
} //FuncCall

//PARAMS ->	idt PARAMSTAIL | num PARAMSTAIL | $
void parse::PARAMS(int &tmp_params)
{
	if(processWordToken(buffer) == idt)
	{
		MATCH(idt);  // idt
		checkEntry();
		tmp_params++;
		st.push(buffer);
		getToken();
	}
	else if(processWordToken(buffer) == NUMt) { MATCH(NUMt); tmp_params++ ; st.push(buffer); getToken(); }// numt
	PARAMSTAIL(tmp_params);
} //PARAMS

//PARAMSTAIL		->	, idt PARAMSTAIL | , num PARAMSTAIL  | $
void parse::PARAMSTAIL(int &tmp_params)
{
		if(strcmp(buffer,",") == 0)
		{
			getToken();
			if(processWordToken(buffer) == idt)
			{
			MATCH(idt);  // idt
			checkEntry();
			st.push(buffer);
			tmp_params++;
			}
			else if(processWordToken(buffer) == NUMt) {  MATCH(NUMt) ; st.push(buffer);tmp_params++;} // numt
			else errorReport("Expected valid idt or num ");
			getToken();
			PARAMSTAIL(tmp_params);
		}
} // PARAMSTAIL


/*** function <VariableInsertion>**************************************************
*** DESCRIPTION: <write every details for variable entry						***
*** INPUT ARGS:  < type_temp, lexeme_temp >										***
*** OUTPUT ARGS:																***	
*** IN/OUT ARGS: < off >														***
*** RETURN: <>	********************************************	                **/
void VariableInsertion(char type_temp[], char lex_temp[], int &off)
{	
			char tkn[10] = "idt";
			EntryPtr lookUpPtr1;
			insert(lex_temp,tkn , DEPTH, varDec); //insert
			lookUpPtr1 = lookup(lex_temp);
			lookUpPtr1->TypeOfEntry = varEntry; // type of entry
			
			if(strcmp(type_temp,"int")==0) 
			{
			lookUpPtr1->var.TypeOfVariable = intType; //type
			lookUpPtr1->var.size = 2; //size
			lookUpPtr1->var.Offset = off;	//offset				
			off=off+2;
			}
			
			else if(strcmp(type_temp,"float")==0)
			{
			lookUpPtr1->var.TypeOfVariable = floatType;
			lookUpPtr1->var.size = 4;
			lookUpPtr1->var.Offset = off;	
			off=off+4;
			}
			
			else if(strcmp(type_temp,"char")==0)
			{
			lookUpPtr1->var.TypeOfVariable = charType;
			lookUpPtr1->var.size = 1;
			lookUpPtr1->var.Offset = off;
			off= off+1;
			}
			
} // VariableInsertion

void CheckIdt()
{
	EntryPtr tmpChk;
	tmpChk = lookup(buffer);
	if(tmpChk == NULL)
	errorReport(" Undeclared variable ! Expected declared variable");
	if(tmpChk)
	{
		if(tmpChk->TypeOfEntry == functionEntry)
		errorReport(" FunctionEntry can not be used in input / output operation, Expected valid varEntry");
		else if(tmpChk->TypeOfEntry == constEntry)
		errorReport("Constant entry cannot be used here, Expected valid idt");
	}
}

void checkEntry()
{
	EntryPtr tmpChk;
	tmpChk = lookup(buffer);
	if(tmpChk == NULL)
	errorReport(" Undeclared variable ! Expected declared variable");
	if(tmpChk)
	{
		if(tmpChk->TypeOfEntry == functionEntry)
		errorReport(" FunctionEntry can not be used in assignment operation, Expected valid varEntry/constEntry");
	}
}

EntryType checkEntType()
{
	EntryPtr tmpChkT;
	tmpChkT = lookup(buffer);
	if(tmpChkT)
	{
	if(tmpChkT->TypeOfEntry == functionEntry)
		return functionEntry;
	else if(tmpChkT->TypeOfEntry == varEntry)
		return varEntry;
	else if(tmpChkT->TypeOfEntry == constEntry)
		return constEntry;
	}
}

void checkMain()
{
	char main[] = "main";
	EntryPtr chkmain;
	chkmain = lookup(main);
	if(chkmain == NULL || chkmain->TypeOfEntry != functionEntry)
		errorMsg("Expected main function in the program, but , does not exist !\n\n");
}

void fcTAC()
{
	EntryPtr funcTmp;
	string s;
	char s1[50];
	while(!st.empty())
	{
		s = st.top();
		strcpy(s1,s.c_str());
		funcTmp = lookup(s1);
		if(funcTmp)
		{
			if(funcTmp->TypeOfEntry == functionEntry)
			{
				//cout<<"\ncall "<<st.top()<<endl;
				out<<"call "<<st.top()<<"\r\n";
			}
			
			if(funcTmp->TypeOfEntry == varEntry && funcTmp->depth == 2)
			{
				if(funcTmp->varDec == 1)
				{
				//cout<<"\npush "<<"[bp+"<<funcTmp->var.Offset+4<<"]"<<endl;	
				out<<"push        [bp+"<<funcTmp->var.Offset+4<<"]"<<"\r\n";
				}
				else
				{
				//cout<<"\npush "<<"[bp-"<<funcTmp->var.Offset+2<<"]"<<endl;
				out<<"push        [bp-"<<funcTmp->var.Offset+2<<"]"<<"\r\n";
				}
			}
			else if(funcTmp->TypeOfEntry == varEntry && funcTmp->depth == 1)
			{
			//cout<<"\npush "<<s1<<endl;
			out<<"push      "<<s1<<"\r\n";
			}
			else if(funcTmp->TypeOfEntry == constEntry)
			{
				if(funcTmp->constant.TypeOfConstant == intType)
				{
				//cout<<"\npush "<<funcTmp->constant.Value<<endl;
				out<<"push      "<<funcTmp->constant.Value<<"\r\n";
				}
				else
				{
				//cout<<"\npush   "<<funcTmp->constant.ValueR<<endl;
				out<<"push      "<<funcTmp->constant.ValueR<<"\r\n";
				}
			}
		}
		else if(strcmp(s1,"=") == 0)
		{
			st.pop();
			s = st.top();
			strcpy(s1,s.c_str());
			funcTmp = lookup(s1);
			if(funcTmp->depth == 2)
			{
				int val = funcTmp->var.Offset+2;
			//cout<<"\n[bp-"<<funcTmp->var.Offset <<"]"<<"= ax " <<endl;
			out<<"[bp-"<<val<<"]   =   ax"<<"\r\n";
			}
			else
			{
			//cout<<"\n"<<s1 <<" = ax " <<endl;
			out<<s1<<"   =   ax"<<"\r\n";
			}
		}
		else
		{
		//cout<<"\npush "<<s1<<endl;
		out<<"push       "<<s1<<"\r\n";
		}
		st.pop();
	}
	
}

void emit(char *code)
{
	out<<code;
	out<<"\r\n";
}

void createTemp(EntryPtr &tmpptr)
{
	char lex_tmp[20]="";
	char int_tmp[2] = "";
	tmpptr = new TableEntry();
	EntryPtr tmpfunptr;
	tmpfunptr = lookup(procName);
	strcat(lex_tmp,"[bp-");
	sprintf(int_tmp, "%d", tmp_i);
	strcat(lex_tmp,int_tmp);
	strcat(lex_tmp,"]");
	strcpy(tmpptr->LexemeBP,lex_tmp);
	
	tmpfunptr->function.SizeOfLocal = tmpfunptr->function.SizeOfLocal + 2;
	tmpptr->varDec = 2;
	tmp_i = tmp_i + 2;
}

void setBP(EntryPtr setBPptr,char *code)
{
	strcpy(code,"");
	if(setBPptr->varDec == 1)
	{
	char bpTmp[20] = "";
	sprintf(bpTmp,"%d",setBPptr->var.Offset+4);
	strcat(code,"[bp+");
	strcat(code,bpTmp);
	strcat(code,"]");
	}
	else if(setBPptr->varDec == 2)
	{
	char bpTmp[20] = "";
	sprintf(bpTmp,"%d",setBPptr->var.Offset+2);
	strcat(code,"[bp-");
	strcat(code,bpTmp);
	strcat(code,"]");
	}
	else
	strcat(code, setBPptr->Lexeme);
	
}

EntryPtr createBP(char lex_input[])
{
	EntryPtr bptr;
	bptr = lookup(lex_input);
		if(bptr)
		{
			char tmpLx[50] = "", cd[50] = "";
			if(bptr->TypeOfEntry == varEntry)
			{
			if(bptr->depth == 1)
			strcpy(bptr->LexemeBP,bptr->Lexeme);
			if(bptr->depth == 2 && bptr->varDec == 1) // params
			{
				sprintf(tmpLx,"%d",bptr->var.Offset+4);
				strcat(cd,"[bp+");
				strcat(cd,tmpLx);
				strcat(cd,"]");
				strcpy(bptr->LexemeBP,cd);
			}
			else if(bptr->depth == 2 && bptr->varDec == 2) //local
			{
				sprintf(tmpLx,"%d",bptr->var.Offset+2);
				strcat(cd,"[bp-");
				strcat(cd,tmpLx);
				strcat(cd,"]");
				strcpy(bptr->LexemeBP,cd);
			}
			}
			else if(bptr->TypeOfEntry == constEntry )
			{
				if(bptr->constant.TypeOfConstant == intType)
				{		
					sprintf(tmpLx,"%d",bptr->constant.Value);
					strcpy(bptr->LexemeBP, tmpLx );
				}
				else if(bptr->constant.TypeOfConstant == floatType)
				{
					sprintf(tmpLx,"%f",bptr->constant.ValueR);
					strcpy(bptr->LexemeBP, tmpLx );
				}
			}
		}
	return bptr;
}

// Inserting string literal into the symbol table 
EntryPtr insertLiteral(char lit[])
{
		int count=0;
		char t1[10], tkn[10] = "";
		EntryPtr lit_check = lookup(lit);
		if(lit_check == NULL)
		{ // insert
		//generate _S0
			for (int i = 0; i < TableSize; i++)
			{
			EntryPtr current;
			current = SymTab[i];
			while(current)
			{
				if(current->depth == 1 && current->TypeOfEntry == varEntry)
				{
					if(current->var.TypeOfVariable == literalType)
						count++;
				}
				current = current->next;
			} // count string literal
		}
		sprintf(t1,"%d",count);
		strcat(tkn,"_S");
		strcat(tkn,t1);
		insert(lit, tkn, 1, 0);	
		lit_check=lookup(lit);
		lit_check->TypeOfEntry = varEntry;
		lit_check->var.TypeOfVariable = literalType;
		}
return lit_check;
}


void createTemplateASM(char file_tac[], char file_asm[])
{
	fstream readTac;
	string fdetail;
	readTac.open(file_tac,ios::in); // reading TAC
	writeAsm.open(file_asm,ios::out); // create a file asm 
	EntryPtr ptr_asm; // entry pointer 

	writeAsm<<"		.model small"<<"\r\n"<<"		.stack 100h"<<"\r\n"<<"		.data"<<"\r\n";
	//code
	for (int i = 0; i < TableSize; i++)
	{
		EntryPtr current;
		current = SymTab[i];
		while(current)
		{
			if(current->depth == 1)
			{
				if(current->TypeOfEntry == varEntry)
				{
					if(current->var.TypeOfVariable == intType)
						writeAsm<<current->Lexeme<<"		DW	?"<<"\r\n";
					else if(current->var.TypeOfVariable == charType)
						writeAsm<<current->Lexeme<<"		DB	?"<<"\r\n";
					else if(current->var.TypeOfVariable == literalType)
						writeAsm<<current->Token<<"		DB	\""<<current->Lexeme<<"\",\"$\""<<"\r\n";
					//else if( condition for string
			}	}
			current=current->next;
		}
	}
	
	writeAsm<<"		.code"<<"\r\n"<<"		include io.asm"<<"\r\n"<<"\r\n";
	
	while(readTac)
	{
		getline(readTac,fdetail);
		acm_generate(fdetail);
	}
	
	writeAsm<<"_startproc		PROC"<<"\r\n"<<"				mov ax,@data"<<"\r\n";
	writeAsm<<"				mov ds,ax"<<"\r\n"<<"				call main"<<"\r\n";
	writeAsm<<"				mov ax,04ch"<<"\r\n"<<"				int 21h"<<"\r\n"<<"_startproc		ENDP"<<"\r\n"<<"				end _startproc"<<"\r\n";

	cout<<"The File \'"<<file_asm<<"\' containing assembly code has been successfully created !" <<endl<<endl;
}

void acm_generate(string fdetail)
{
	string arr[5];
	EntryPtr tmpPtr;
	int i = 0;
	stringstream ssin(fdetail);
	while (ssin.good() && i < 5){
    ssin >> arr[i];
    ++i;
	}
	
	if(arr[0] == "") // empty
	return;
	else if(arr[0].find("PROC") != string::npos) // start proc
	{
		int l = arr[1].length();
		char tmp[l];
		strcpy(tmp,arr[1].c_str());
		writeAsm<<arr[1]<<"			"<<arr[0]<<endl;
		tmpPtr = lookup(tmp);
		writeAsm<<"			push bp"<<endl;
		writeAsm<<"			mov bp,sp"<<endl;
		writeAsm<<"			sub sp,"<<tmpPtr->function.SizeOfLocal<<endl;
	}
	else if(arr[0].find("endP") != string::npos)  // end proc
	{
		int l = arr[1].length();
		char tmp[l];
		strcpy(tmp,arr[1].c_str());
		tmpPtr = lookup(tmp);
		writeAsm<<"			add sp,"<<tmpPtr->function.SizeOfLocal<<endl;
		writeAsm<<"			pop bp"<<endl;
		writeAsm<<"			ret "<<tmpPtr->function.NumberOfParameters<<endl;
		writeAsm<<arr[1]<<"			"<<arr[0]<<endl;
	}
	else if(arr[0].find("rdi") != string::npos)  // read integer
	{
		writeAsm<<"			 call readint"<<endl;
		writeAsm<<"			 mov"<<arr[1]<<",bx"<<"			;read integer"<<endl;
	}
	else if(arr[0].find("rdc") != string::npos)  // read character
	{
		writeAsm<<"			 call readch"<<endl;
		writeAsm<<"			 mov"<<arr[1]<<",bx"<<"			;read character"<<endl;
	}
	else if(arr[0].find("wri") != string::npos)  // write integer
	{
		writeAsm<<"			 mov ax,"<<arr[1]<<"			;write integer"<<endl;
		writeAsm<<"			 call writeint"<<endl;
	}
	else if(arr[0].find("wrc") != string::npos)  // write character
	{
		writeAsm<<"			 mov ax,"<<arr[1]<<"			;write character"<<endl;
		writeAsm<<"			 call writech"<<endl;
	}
	else if(arr[0].find("wrs") != string::npos)  // write string
	{
		writeAsm<<"			 mov ax,"<<arr[1]<<"			;write string"<<endl;
		writeAsm<<"			 call writestr"<<endl;
	}
	else if(arr[0].find("call") != string::npos) // call
		writeAsm<<"			"<<arr[0]<<" "<<arr[1]<<endl;
	else if(arr[2].find("ax") != string::npos) // assign after call
		writeAsm<<"			"<<"mov "<<arr[0]<<","<<arr[2]<<endl;
	else if(arr[0].find("ax") != string::npos) // return statement
		writeAsm<<"			"<<"mov ax,"<<arr[2]<<endl;
		
	else if(arr[0].find("push") != string::npos) // return statement
		writeAsm<<"			"<<"push "<<arr[1]<<endl;
	else if(arr[3] == "")
	{
	writeAsm<<"			"<<"mov ax,"<<arr[2]<<endl;
	writeAsm<<"			"<<"mov "<<arr[0]<<",ax"<<"		;"<<arr[0]<<" = "<<arr[2]<<endl;
	}
	else if(arr[3].find("+") != string::npos)
	{
		string arr[5];
		int i = 0;
		stringstream ssin(fdetail);
		while (ssin.good() && i < 5){
        ssin >> arr[i];
        ++i;
		}
	writeAsm<<"			"<<"mov ax, "<<arr[2]<<endl;
	writeAsm<<"			"<<"add ax, "<<arr[4]<<endl;
	writeAsm<<"			"<<"mov "<<arr[0]<<","<<"ax"<<endl;
	}
	
	else if(arr[3].find("-") != string::npos)
	{
		string arr[5];
		int i = 0;
		stringstream ssin(fdetail);
		while (ssin.good() && i < 5){
        ssin >> arr[i];
        ++i;
		}
	writeAsm<<"			"<<"mov ax, "<<arr[2]<<endl;
	writeAsm<<"			"<<"sub ax, "<<arr[4]<<endl;
	writeAsm<<"			"<<"mov "<<arr[0]<<","<<"ax"<<endl;
	}
	else if(arr[3].find("*") != string::npos)
	{
		string arr[5];
		int i = 0;
		stringstream ssin(fdetail);
		while (ssin.good() && i < 5){
        ssin >> arr[i];
        ++i;
		}
	writeAsm<<"			"<<"mov ax, "<<arr[2]<<endl;
	writeAsm<<"			"<<"mov bx, "<<arr[4]<<endl;
	writeAsm<<"			"<<"mul bx "<<endl;
	writeAsm<<"			"<<"mov "<<arr[0]<<","<<"ax"<<endl;
	}
	else if(arr[3].find("/") != string::npos)
	{
		string arr[5];
		int i = 0;
		stringstream ssin(fdetail);
		while (ssin.good() && i < 5){
        ssin >> arr[i];
        ++i;
		}
	writeAsm<<"			"<<"mov ax, "<<arr[2]<<endl;
	writeAsm<<"			"<<"mov bx, "<<arr[4]<<endl;
	writeAsm<<"			"<<"div bx "<<endl;
	writeAsm<<"			"<<"mov "<<arr[0]<<","<<"ax"<<endl;
	}
}
#endif
