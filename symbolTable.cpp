/*
*** NAME: Romancha Khatri								
*/

#include<iostream>
#include<cstdlib>
#include<iomanip>
#include<cstring>
#include<string>

#include "SYM_H.h"  //header for symbol table
#define EOS '\0'


using namespace std;
EntryPtr SymTab[TableSize];  // globle pointer decl for symbol table entry
int lexDep=0;

class STable
{
private: int hash(char *); //private method
public:	
	STable();
}; //STable

/*** CONSTRUCTOR***********************************************
*** DESCRIPTION: <initialize the table to empty>			***
*** INPUT ARGS:  < >										***
*** OUTPUT ARGS:											***	
*** IN/OUT ARGS:											***
*** RETURN: <>	************************************		**/

STable::STable() 
{
	for(int i = 0; i < TableSize; i++ )
		SymTab[i] = NULL; //initialize all table entries to NULL reference
} //STable

/*** function <insert>*************************************************************
*** DESCRIPTION: <Inserting lexeme, token and depth into the symbole table.>	***
*** INPUT ARGS:  < lex, token, depth >											***
*** OUTPUT ARGS:																***	
*** IN/OUT ARGS:																***
*** RETURN: <>	************************************							**/
void insert(char lex[], char token[], int depth, int VarDec )
{
	EntryPtr current, LookupPtr;
    current = new TableEntry(); //create a new list
	strcpy(current->Lexeme , lex); // copy given lexeme to list
	strcpy(current->Token , token); // copy given token to list
    current->depth = depth;
	current->varDec = VarDec;
	current->next = NULL;
	
	LookupPtr = lookup(lex);
	if(LookupPtr)
	{
	if(LookupPtr->depth == depth)
	{
		cout<<"Error! - At line: "<<line<<" ,Duplicate lexeme \" "<<lex<<" \" not allowed on same depth. "<<endl<<endl;
		exit(0);
	}
	}
	int index = hash(lex);

	if(SymTab[index] == NULL)
		SymTab[index] = current;
	
	else //check if another list is on same hash value?
	{	current->next = SymTab[index]; //new dataset points to previous dataset
        SymTab[index] = current; //hash table points to new dataset
    }
	
	
} //insert

/*** function <lookup>*************************************************************
*** DESCRIPTION: <find the entry & returns a pointer to that entry>				***
*** INPUT ARGS:  < lex  >														***
*** OUTPUT ARGS:																***	
*** IN/OUT ARGS:																***
*** RETURN: <pointer>	************************************					**/
EntryPtr lookup(char lex[])
{
    EntryPtr current = SymTab[hash(lex)];
	if(current != NULL)
	{
		while(current != NULL)
		{	
			if(strcmp(current->Lexeme, lex) == 0)
			{
				lexDep = current->depth; //gloabl variable lexDep for lexeme while lookup
				return current;
			}
			else
			{
				current = current->next;
			}
		}
    }
	else
		return NULL;
} //lookup

/*** function <deleteDepth> *********************************************************
*** DESCRIPTION: <delete all records at that depth & condtitional decrease in depth>*
*** INPUT ARGS:  < depth  >														  ***
*** OUTPUT ARGS:																  ***	
*** IN/OUT ARGS:																  ***
*** RETURN: <>			************************************					  **/

void deleteDepth(int depth)
{
	bool delcheck=false;
	EntryPtr temp, current;
    for(int i = 0; i < TableSize; i++)
	{
		current = SymTab[i];
		while(current != NULL && current->depth == depth) //checking for hashtable index
		{
			delcheck = true;
			SymTab[i] = current->next;
			delete current;
			current = SymTab[i];
			if(current != NULL)
			{
				temp = current->next;
			}
		}
		while(temp != NULL) // checking for rest of chaining list (processing forward in chain)
		{
			if(temp->depth == depth)
			{
				delcheck = true;
				current->next = temp->next;
				temp = current->next;
			}
			else
			{
				current = temp;
				if(current != NULL)
				temp = temp->next;
			}
		}
    }
	if(!delcheck) cout<<"Lexeme not found at given depth "<<depth<<endl;
	else cout<<"All lexemes at given depth "<<depth<<" are deleted successfully"<<endl;
	cout<<"------------------------------------------------------------------------------------------------------------------------------------------------------------"<<endl<<endl;
	
} //deleteDepth

/*** function <writeTable>*********************************************************
*** DESCRIPTION: <Write all variables in the table at specified depth>			***
*** INPUT ARGS:  < depth  >														***
*** OUTPUT ARGS:																***	
*** IN/OUT ARGS:																***
*** RETURN: <>	********************************************					**/
void writeTable(int depth)
{
cout<<"--------------------------------------------- LEXEMES WITH DETAILS AT DEPTH " <<DEPTH<<" ------------------------------------------------------------------------------"<<endl<<endl;
	for (int i = 0; i < TableSize; i++)
	{
		EntryPtr current;
		current = SymTab[i];
		int size_of_params=0;	
			if (current)
			{
				if(current->depth == depth)
				{    
					if (current->TypeOfEntry == varEntry) //varEntry
					{
                        cout <<setw(20)<< "Lexeme : " << current->Lexeme ;
						cout <<setw(20)<<"Depth : " << current->depth;
                        cout <<setw(40)<<"TypeOfEntry : varEntry ";
                        int id = current->var.TypeOfVariable;
						cout<<setw(20)<<"Type : "<<setw(10)<<getTypeFromId(id); // get type from id
                        cout <<setw(20)<<"Offset : " <<current->var.Offset;
						cout<<setw(20)<<"Var. Decl:";
						if(current->varDec == 0) cout<<"Global";
						else if(current->varDec == 1) cout<<"Params";
						else if(current->varDec == 2) cout<<"Local";
                        current = current->next; // get next
						cout<<endl;
					}
                    else if (current->TypeOfEntry == constEntry) //constEntry
                    { 
                        cout <<setw(20)<< "Lexeme : "  <<setw(20)<< current->Lexeme ;
						cout <<setw(20)<<"Depth : " << current->depth;
                        cout <<setw(40)<<"TypeOfEntry : constEntry ";
						
                        if(current->constant.TypeOfConstant == intType){
                            cout << setw(20)<<"Type : "<<setw(10)<<"integer";
							cout <<setw(20)<<"Value : ";
							cout << current->constant.Value;
                        } //intType
                        if(current->constant.TypeOfConstant == floatType){
                            cout <<setw(20) <<"Type : "<<setw(10)<<"float";
							cout << setw(20)<<"ValueR : ";
							cout << current->constant.ValueR;
                        } //floatType
                        current = current->next; // get next
						cout<<endl;
                    }
                    else if (current->TypeOfEntry == functionEntry) // functionEntry
                    {
                        cout <<setw(20)<<"Lexeme : " <<setw(20)<< current->Lexeme;
						cout <<setw(20)<<"Depth : " << current->depth;
                        cout <<setw(40)<<"TypeOfEntry : functionEntry ";
						cout<<setw(20)<<"Return type : ";
						int res = current->function.ReturnType;
						cout<<setw(10)<<getTypeFromId(res)<<endl; // get type from id
						cout<<setw(20)<<" [ Size of locals : "<<current->function.SizeOfLocal <<" Bytes"; // size of locals
						cout<<"\t , No. of Parameters : " <<current->function.NumberOfParameters; //number of parameters
						cout<<"\t, Size of parameters : "<<current->function.SizeOfParameters<< " Bytes";
                        cout<<"\t , Parameters - types : ";
						
                        ParamPtr tempPtr = current->function.ParamList;
						cout<<"{ ";
						while(tempPtr)
						{// move forward until last parameter
							int id = tempPtr->typeOfParameter;
							cout<<"( "<<tempPtr->nameOfParameter<<" - "<<getTypeFromId(id)<<" ) ";
							tempPtr = tempPtr->Next;
						}
						cout<<" } ]";
						cout<<endl<<endl;
						current = current->next; // get next
                    }
					cout<<endl;
				}
			}
		
	}

cout<<"------------------------------------------------------------------------------------------------------------------------------------------------------------"<<endl<<endl;	
} //writeTable

/*** function <hash>	  *********************************************************
*** DESCRIPTION: <finding the location for the input lexeme>					***
*** INPUT ARGS:  < lexeme  >													***
*** OUTPUT ARGS:																***	
*** IN/OUT ARGS:																***
*** RETURN: <>	********************************************					**/
int hash(char* lexeme)
{
	char *P;
	unsigned h=0,g;
	for (P=lexeme; *P!=EOS; P=P+1)
	{
		h=(h<<4)+(*P);
		if(g==(h&0xf0000000))
		{
			h=h^(g>>24);
			h=h^g;
		}
	}
	return h%TableSize;
} // hash


/*** function <getType>	  *********************************************************
*** DESCRIPTION: <finding appropriate type for a given token>					***
*** INPUT ARGS:  < t  >															***
*** OUTPUT ARGS:																***	
*** IN/OUT ARGS:																***
*** RETURN: <VarType>	********************************************			**/
VarType getType(char t[])
{
	if(strcmp(t,"int")==0) return intType;
	else if(strcmp(t, "float") == 0) return floatType;
	else if(strcmp(t, "char") == 0) return charType;
}

//Function to check integer or float for given token
VarType checkValueType (char val[])
{
	VarType temp = intType;
	int k=0;
	while(val[k] != '\0')
	{
		if(val[k] == '.')
		{
			temp = floatType;
			break;
		}
	k++;
	}
return temp;
}
//return type name from its id
string getTypeFromId(int id)
{
	if(id == 0)
		return "char";
	else if(id == 1)
		return "integer";
	else if(id == 2)
		return "float";
	else if(id == 3)
		return "literal";
}
