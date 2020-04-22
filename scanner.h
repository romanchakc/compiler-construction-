/***********************************************************************
*** NAME: Romancha Khatri											 ***
*** CLASS:  CSC 446													 ***
*** ASSIGNMENT:	#1 													 ***	
*** DUE DATE: 01/31/2017											 ***
*** INSTRUCTOR:	G. HAMER											 ***
***********************************************************************/
#ifndef _SCANNER
#define _SCANNER

token checkSymbols(char[]);
bool checkValidId(char[]);
bool checkKeyword(char[]);
bool checkDigits(char []);
token processWordToken(char input[]);
bool checkSeparator(char t);
void matchId();
void getToken();
void errorReport(string);
bool Separator(char t);
void processCommentToken(bool &comment);
token processToken();


/*** FUNCTION <getToken>		***************
*** DESCRIPTION: <getting token>			***
*** INPUT ARGS:  <   >						***
*** OUTPUT ARGS: <>							***	
*** IN/OUT ARGS: <global var. buffer>		***
*** RETURN: <>								**/
void getToken()
{
	int i=0;
	char chk,tmp;
	bool comment =false;
	strcpy(previous,buffer);
	//buffer[i] ='\0';
	while(in)
	{
		chk = in.get(); //reading a character from a file
		if(chk == '/') 
		{
			processCommentToken(comment);
			if(comment)
			chk = in.get();
		}
				
		if(isblank(chk) >=1 || chk == '\0' || chk == '\n' ||Separator(chk) == true)  // check blank characters (tab, white spaces)
		{
				if(i > 0)
				{
				   buffer[i] = '\0';	
				   i=0;
				   if(Separator(chk)) in.seekg(-1,ios::cur);
				   return;
				   
				}
				else if(Separator(chk))
				{
					if(chk == '&') // &&
					{
						tmp = in.get();
						if(tmp == '&')
						{
							buffer[i] = tmp;
							i++;
						}
						else
						in.seekg(-1,ios::cur);
					}
					else if(chk == '|') // ||
					{
						tmp = in.get();
						if(tmp == '|')
						{
							buffer[i] = tmp;
							i++;
						}
						else
						in.seekg(-1,ios::cur);
					}
					else if(chk == '>')
					{
						tmp = in.get();
						if(tmp == '>')
						{
							buffer[i] = tmp;
							i++;
						}
						else
						in.seekg(-1,ios::cur);
					}
					else if(chk == '<')
					{
						tmp = in.get();
						if(tmp == '<')
						{
							buffer[i] = tmp;
							i++;
						}
						else
						in.seekg(-1,ios::cur);
					}
					
					buffer[i] = chk;
					buffer[++i] = '\0';
					i=0;
					return;
				}
				
				if(chk == '\0' || chk == '\n')
					line++;
		}

		else
		{
			buffer[i++] = chk;
		}
	}
	if(in.eof()) //check if end of file token
	{
		if(i>1)
		{
		buffer[--i] = '\0';
		return;
		}
		else
		{
		buffer[i] = '\0';
		return;
		}
	}

} // getToken

void processCommentToken(bool &comment)
{
	comment = false;
	char temp;
	temp= in.get();
		if(temp == '*')
				{
			label:	temp = in.get();
					while(temp != '*' && !in.eof())
						temp = in.get();	
							if(!in)
							{
								cout<<"Missing end of comment */ "<<endl<<endl;
								return;
							}
							temp = in.get();
							if(temp == '/') //check if end of comment
							{
							comment = true;
							return;
							}
								else
								goto label;	
				}
				else
					in.seekg(-1, ios::cur);
}

/*** FUNCTION <errorReport>		***************
*** DESCRIPTION: <printing error message>	***
*** INPUT ARGS:  <string msg >				***
*** OUTPUT ARGS:							***	
*** IN/OUT ARGS:							***
*** RETURN: <>								**/

void errorReport(string msg)
{
	error++;
	if(in.eof())
		cout<<"\nAt line "<<line<<"," <<msg<<" after \'"<<previous<<"\' but found \'eof() token\' "<<endl<<endl;
	else
		cout<<"\nAt line "<<line<<"," <<msg<<" after \'"<<previous<<"\' but found \'"<<buffer<<"\'"<<endl<<endl;
	exit(0);
} // errorReport

//error print
void errorMsg(string msg)
{
	cout<<"\nAt line "<<line<<" ,"<<msg<<endl<<endl;
	exit(0);
}


/*** FUNCTION <matchId>			***************
*** DESCRIPTION: <IDt validation>			***
*** INPUT ARGS:  <   >						***
*** OUTPUT ARGS:							***	
*** IN/OUT ARGS:							***
*** RETURN: <>								**/

void matchId() //Checking valid identifier
{
    int i=0,len,len_err=0,type_match=0;
	len = strlen(buffer);
	bool check = true;
	if(len >27 ) 
	{
		len_err = 1;
		check = false;
	}
	else if(checkKeyword(buffer)){ type_match = 1; check = false;}
	
	else if(buffer[i] == '_' || isdigit(buffer[i]) != 0) check = false;  
	else
	{
	while(buffer[i] != '\0' && check == true)
	{
			if(isalnum(buffer[i]) || buffer[i] == '_')
				check = true;
			else
				check = false;
		i++;
	}
	}
	if(check == false)
	{
		if(len_err == 1)
			errorReport(" The Length of idenfier exceeds 27 chacaters, Expected valid identifier ");
		if(type_match == 1)
			errorReport(" Identifier's name can not be a keyword, Expected valid identifier ");
		else
		errorReport(" Expected valid identifier ");
	}
} //matchId

// processing valid id token
token processToken() //Checking valid identifier
{
    int i=0,len,len_err=0,type_match=0;
	len = strlen(buffer);
	bool check = true;
	if(len >27 ) 
	{
		len_err = 1;
		check = false;
	}
	else if(checkKeyword(buffer)){ type_match = 1; check = false;}
	
	else if(buffer[i] == '_' || isdigit(buffer[i]) != 0) check = false;  
	else
	{
	while(buffer[i] != '\0' && check == true)
	{
			if(isalnum(buffer[i]) || buffer[i] == '_')
				check = true;
			else
				check = false;
		i++;
	}
	}
	if(check)
		return idt;
	else
		return invalidt;
} //matchId


/*** FUNCTION <checkSeparator>	***************
*** DESCRIPTION: <checking separator>		***
*** INPUT ARGS:  <char   >					***
*** OUTPUT ARGS:							***	
*** IN/OUT ARGS:							***
*** RETURN: <bool>							**/
bool checkSeparator(char t) //string separator code
{
	bool flag = false;
	for(int i=0;i<strlen(vs);i++)
	{
		if(t == vs[i])
		{
			flag = true;
			break;
		}
	}
	return flag;
} //checkSeparator



bool Separator(char t) //string separator code
{
	bool flag = false;
	for(int i=0;i<strlen(sp);i++)
	{
		if(t == sp[i])
		{
			flag = true;
			break;
		}
	}
	return flag;
}


/*******************************************
*** FUNCTION <checkKeyword> ****************
*** DESCRIPTION: <checks valid keywords> ***
*** INPUT ARGS:  <char s[] >			 ***
*** OUTPUT ARGS:						 ***	
*** IN/OUT ARGS:						 ***
*** RETURN: <bool>						 ***
*******************************************/

bool checkKeyword(char s[]) // checking valid keywords
{
	bool flag = false;
	for(int i=0;i<10;i++) 
		{
		if(strcmp(s,resWords[i])==0)
			{
			flag = true;	
			break;
			}
		}
	 return flag;
} //checkKeyword



/*******************************************
*** FUNCTION <checkDigits>			     ***
*** DESCRIPTION: <checks valid digits>   ***
*** INPUT ARGS:  <char s[] >			 ***
*** OUTPUT ARGS:						 ***	
*** IN/OUT ARGS:						 ***
*** RETURN: <bool>						 ***
*******************************************/

bool checkDigits(char s[]) // checking valid digits
{	
	char temp[100];
	int j=0,k=0,m=0,count_period = 0;
	int check = true;

	while(s[m] != '\0')
	{
		if(s[m] != '.' && !isdigit(s[m]))
			errorReport(" Expected valid num token ");
		m++;
	}
	if(isdigit(s[j]))  // checking if digits preceed in identifier
   {
	   while(isdigit(s[j]) || s[j] == '.')
	   {
		   if(s[j] == '.') count_period++;
		   temp[k++] = s[j];
		   j++;
	   }
		temp[k] = '\0';
		if(count_period > 1 || strcspn(temp,".") == 0 || temp[k-1] == '.' ) // counting & checking periods first, last occurence position
			errorReport(" Expected valid num token ");
		else
		return true;
	}	
   else
   errorReport(" Expected valid num token ");
return true;
} //checkDigits

/*********************************************
*** FUNCTION <processWordToken>			        ***
*** DESCRIPTION: <checks valid identifier> 		***
*** INPUT ARGS:  <char input[] >		   		***
*** OUTPUT ARGS:						   		***	
*** IN/OUT ARGS:						   		***
*** RETURN: <bool>						   		***
**********************************************/
token processWordToken(char input[]) //Checking valid identifier
{
	if(strcmp(input,"int") == 0) return intt;
	else if(strcmp(input,"float") == 0) return floatt;
	else if(strcmp(input,"char") == 0) return chart;
	else if(isdigit(input[0])) return NUMt; 
	else if(processToken() == idt) return idt;
	else invalidt;
} //processWordToken


/*******************************************	 		 **
*** FUNCTION <checkSymbols>		          	 			***
*** DESCRIPTION: <checking valid operators & symbols> 	***
*** INPUT ARGS:  <char c >							    ***
*** OUTPUT ARGS:						  			 	***	
*** IN/OUT ARGS:						   				***
*** RETURN:								   				***
*********************************************			*/

token checkSymbols(char c) //checking valid operators
{
	char temp;
	char temp1[500] = "";;
	int i=0;
	if(c == '(') return LParent;
	else if (c == ')') return RParent;
	else if (c == '{') return lCurlyt;	
	else if (c == '}') return rCurlyt;
	else if (c== '[' ) return lIndext;
	else if (c== ']' ) return rIndext;
	else if (c== ';' ) return semiColont;
	else if (c== ',') return commat;
		else if(c == '"') // string literal
		{
			c = in.get();
			while(c != '"' && c != '\n' )
			{
			temp1[i++] = c;
			c = in.get();
			}
				if(c == '\n' || c== '\0')
					return invalidt;
					
						else if(c == '"')
						{
							if(strlen(temp1) !=0)
							{
								temp1[i] = '\0';
								return stringLiteralt;
							}
							else
							return emptyStringLiteralt;
						}
		} // string literal
		
		else if(c == '\'') // single quote
		{
			return SingleQuotet;
		}
		
		else if(c == '+' || c=='-') //addopt
			{
				return AddOpt;
			}
			
		else if(c =='|')
			{
				char temp = in.get();
				if(temp == '|')
					return AddOpt;
				else
				{
					in.seekg(-1,ios::cur);
					return invalidt;
					
				}
			}	 // addopt
				
		else if(c=='*' || c=='/' ||c=='%') //mulopt
			{ 
				if(c == '*' || c == '%')
					return MulOpt;
				
				else if(c == '/') // code for checking comments
					{
					temp = in.get();
						if(temp == '*')
						{
				label:	temp = in.get();
						
						while(temp != '*' && !in.eof())
							temp = in.get();	
						
							if(!in)
							{
								cout<<setw(50)<<" "<<setw(70)<<"Missing end of comment */ "<<endl<<endl;
								return invalidt;
							}
							temp = in.get();
							if(temp == '/') //check if end of comment
							return endofcommentt;
								else
								goto label;	
						}
						else
						{
							in.seekg(-1, ios::cur);
							return MulOpt;
						}
					}
						
			} // mulopt and comment check
		
		else if(c == '&')  //mulopt
			{
				temp = in.get();
				if(temp == '&')
					return MulOpt;
				else
				{
					in.seekg(-1, ios::cur);
					return invalidt;
					
				}
			}	
			
		else if(c == '=') //asignopt
			{
				temp = in.get();
				if(temp == '=') //check for equality operator
					return RelOpt;
				else
				{
					in.seekg(-1,ios::cur); //repositioning 1 index back
					return AssignOpt;
				}
					
			}	

		else if(c == '>') // for >=, >>, >
			{
				temp = in.get();
				if(temp == '=')
					return RelOpt;
				else if(temp == '>')
					return RelOpt;
				else
				{
					in.seekg(-1,ios::cur);//repositioning 1 index back
					return RelOpt;
				}
			}	
		
		else if(c == '<')  // for <, <<, <=
			{
				temp = in.get();
				if(temp == '=')
					return RelOpt;
				else if(temp == '<')
					return RelOpt;
				else
				{
					in.seekg(-1,ios::cur); //repositioning 1 index back
					return RelOpt;
				}
			}	

		else if(c == '!')  // != 
				{
				temp = in.get();
						if(temp == '=')
						return RelOpt;
						else
						{
						in.seekg(-1,ios::cur); //repositioning 1 index back
						return invalidt;
						}
				}	
							
}
#endif