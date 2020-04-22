#include<iostream>
#include<cstring>
#include<cctype>
#include<string>
#include<fstream>
#include<cstdlib>
#include<cstdio>
#include<iomanip>
#include<stack>
#include<vector>
#include<sstream>
using namespace std;

#include "global.h" // including header for global declarations
#include "SYM_H.h" // header for symbol table
#include "symbolTable.cpp"
#include "scanner.h" // include scanner.h
#include "parser.h" // include parser.h



/** FUNCTION		< main>		***********************************************
*** DESCRIPTION:	<reading a file and checking valid/invalid operations>	***
*** INPUT ARGS:		<int argc, char* argv[] >								***
*** OUTPUT ARGS:	<>														***	
*** IN/OUT ARGS:	<>														***
*** RETURN:			<>													    ***
******************************************************************************/

int main(int argc, char* argv[])
{
	parse p;  // object of class parse
	char fname1[100],fname2[100];
	int r=0;
	system("clear");
	switch (argc) {
		case 1: 
			cout<<"Please provide name of the file. "<<endl;
			exit(0);
			break;

		case 2: 	
			in.open(argv[1]);
			if(!in)
				{
				cout<<"Can not open the file. Please check name and/or type of that file." <<endl;
				exit(0);
				}
			break;

		default:
			cout<<"Please provide valid format"<<endl;
			exit(0);
				}
		strcpy(fname,argv[1]);
		while(fname[r] != '.')
		{
			fname1[r] = fname[r];
			r++;
		}
		strcpy(fname2,fname1);
		strcat(fname2,".asm");
		
		strcat(fname1,".tac");
		out.open (fname1,ios::out);
		out.clear(); // clear previous contents
		getToken();
		if(!in.eof())
		p.PROG();
		else
		cout<<"No any content to parse! "<<endl;
		if(parse_successful)
		{
		checkMain();
		writeTable(DEPTH); // view all lexeme at depth 1
		cout<<endl;
		out.close();
		//createTemplateASM(fname2);
		createTemplateASM(fname1,fname2);
		cout<<"The File \'"<<fname1<<"\' containing TAC (Three Address Code) has been successfully created !" <<endl<<endl;
		}
	return 0;
}