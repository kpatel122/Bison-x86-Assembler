#include <stdlib.h>         
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include <iostream.h>
#include "instructions.h"
#include "token.h"
#include "file.h"
#include "String.h"
#include "log.h"
#include "lexer.h"


#define PRINT(x) cout<<x<<""; log->Msg(x);

CLexer::CLexer()
{
	index0 = index1 =currentLine = numberLines = numInvalidTokens= 0;
	endOfFile = newLineFound = false;

		ppSource = new char * [1024];
	
}



CLexer::~CLexer()
{
	
}


int CLexer::InitLogFile(CLog *l)
{
	log = l;
	return 1;
}

int CLexer::InitLogFile(char *f)
{
	log = new CLog;
	log->Init(f);

	if(!log)
		return -1;

	return 1;
}

int CLexer::LoadASMFile(char *fileName)
{
	fp = new CFile;

	if(fp->LoadFile(fileName,BISON_FILE_READ)!=BISON_SUCCESS)
		return -1;

	len = fp->GetFileLen();

	if(!len)
		return -1;

	pSource = new char[len+1];
	
	

	fp->ReadFile(pSource,sizeof(char),len+1);
	
	pSource[len] = '\0';

	currPos = pSource;
	endPos = &pSource[len];

	if(!pSource)
		return -1;

	return 1;
}

void CLexer::SkipLine()
{
	while(*currPos!='\n')
	{
		currPos++;
		index0++;
	}

	currPos++;
	index0++;
}

void CLexer::StripComments(char *str,int line)
{
	int counter = 0;
	bool inStr = false;
	int lastLine = index0;
	


	while(1)
	{
		


		if(*currPos == '"')
		{
		  if ((counter < 1))
				inStr = !(inStr);

			else
			{
				if ( *(((currPos)-1)) != '\\')
				{
					inStr = !(inStr);
				}
			}
		}
		
		if ( !((currPos) < (endPos)) )
		{
			pSource[index0] = '\0';
			ppSource[line] = &pSource[lastLine];
			
			
			
			lastLine = index0;

			

			return;
		}

		if ( (*currPos == '\n') && (inStr == false) )
		{
			pSource[index0] = '\0';
			ppSource[line] = &pSource[lastLine];
			
			currPos+=1;
			index0+=1;
			
			lastLine = index0;

			

			return;
		}

		if( (*currPos == ';')  && (inStr == false) )
		{
			pSource[index0] = '\0';
			ppSource[line] = &pSource[lastLine];

			SkipLine();
			
			//currPos+=2;
			//index0+=2;
			
			lastLine = index0;

			return;
		}
		
		currPos++;
		index0++;
		counter++;
		
	}
}

void CLexer::Reset()
{ 
	index0 = index1 = 0;

	currentLine = 0;
	currPos = ppSource[currentLine];
	currLineLength = strlen(ppSource[currentLine]);
	GetNextValidChar();
	endOfFile=false;

}

void CLexer::PrepFile()
{
	index0 = 0;
	static int line = 0;
	char *str;
	while(currPos < endPos)
	{
		str = currPos;
		
		StripComments(str,line);
		line++;
		
	

	}
numberLines = line;
Reset();
	
}


bool CLexer::EndOfFile()
{
	return ( !((currPos) < (endPos)) );
}


void CLexer::SkipWhiteSpaces()
{ 
	
	
	
	while(*currPos ==' ' || *currPos =='\t')
	{
	
		currPos++;
		index1++;
		index0++;

		
	}

	
}

void CLexer::MoveToNextLine()
{
	index0 = index1 = 0;
	currentLine++;
	if(currentLine>=numberLines)
	{
		endOfFile =true;
	return;
	}
	currPos = ppSource[currentLine];
	currLineLength = strlen(ppSource[currentLine]);
	newLineFound = true;

}

void CLexer::GetNextValidChar()
{

	
	if(!(index0 < currLineLength))
	{
		if(currentLine >= (numberLines-1))
		{
			
			endOfFile = true;
			return;
		}
		
		MoveToNextLine();

	
		
	
	}
	
	
	if(CStringInfo::IsCharWhiteSpace(*currPos))
	{
		SkipWhiteSpaces();
	}
	if((*currPos == '\n') || ((*currPos == '\0')))
	{
		MoveToNextLine();
	}
	
}

char *CLexer::GetCurrentLexeme()
{
	bool inStr = false;
	int counter = 0;
	
	
	if(newLineFound)
	{
		newLineFound = false;
		lexBuffer[0]= '\n';
		lexBuffer[1]= '\0';
		GetNextValidChar();
		return lexBuffer;
	}
	

	while(1)
	{
		lexBuffer[counter] = ppSource[currentLine][index0];
		
	
		
		if(*currPos == '"')
		{
			if ((counter < 1))
				inStr = !(inStr);

			else
			{
				if ( *(((currPos)-1)) != '\\')
				{
					inStr = !(inStr);
				}
			}

			if(inStr == false)
			{
				lexBuffer[counter+1] = '\0';

				currPos++;
				index0++;
				counter++;

				GetNextValidChar();
				return lexBuffer;
			}
		}
		
		
		if((CStringInfo::IsCharDelimiter(*currPos)) && (inStr == false) )
		{
			if(!counter)
			{
				currPos++;
				index0++;
				counter++;
			}

			lexBuffer[counter] = '\0';
		//	CStringInfo::MakeStringLowerCase(lexBuffer);
			
			GetNextValidChar();
			return lexBuffer;
		}
		if ((*currPos == ' ') && (inStr == false))
		{
			lexBuffer[counter] = '\0';
		//	CStringInfo::MakeStringLowerCase(lexBuffer);
			
			currPos++;
			index0++;
			counter++;
			
			GetNextValidChar();
			return lexBuffer;


		}

		currPos++;
		index0++;
		counter++;
		
	}
	
}

int CLexer::GetNextToken()
{
	currLexeme = GetCurrentLexeme();
	
	if(CStringInfo::IsCharNewLine(*currLexeme))
		return TOKEN_TYPE_NEWLINE;

	if(CStringInfo::IsStringQuote(currLexeme))
		return TOKEN_TYPE_STRING;
	
	if(CStringInfo::IsStringInt(currLexeme))
		return TOKEN_TYPE_INT;

	if(CStringInfo::IsStringFloat(currLexeme))
		return TOKEN_TYPE_FLOAT;

	if(CStringInfo::IsStringIdent(currLexeme))
	{
		if(strcmp(KEYWORD_FUNCTION,currLexeme)==0)
		return TOKEN_TYPE_FUNC;

		if(strcmp(KEYWORD_VARIABLE,currLexeme)==0)
		return TOKEN_TYPE_VAR;

		if(strcmp(KEYWORD_PARAM,currLexeme)==0)
		return TOKEN_TYPE_PARAM;

		if(strcmp(KEYWORD_REG,currLexeme)==0)
		return TOKEN_TYPE_REG_RETVAL;

		if(strcmp(KEYWORD_STACK_SIZE,currLexeme)==0)
		return TOKEN_TYPE_SETSTACKSIZE;
		
		if(instrTable->IsValidInstr(currLexeme))
		return TOKEN_TYPE_INSTR;
		
		return TOKEN_TYPE_IDENT;
	
	}
	
	if(CStringInfo::IsCharDelimiter(*currLexeme))
	{
		if(strcmp(OPEN_BRACE,currLexeme)==0)
		return TOKEN_TYPE_OPEN_BRACE;

		if(strcmp(CLOSE_BRACE,currLexeme)==0)
		return TOKEN_TYPE_CLOSE_BRACE;

		if(strcmp(OPEN_BRACKET,currLexeme)==0)
		return TOKEN_TYPE_OPEN_BRACKET;

		if(strcmp(CLOSE_BRACKET,currLexeme)==0)
		return TOKEN_TYPE_CLOSE_BRACKET;

		if(strcmp(COLON,currLexeme)==0)
		return TOKEN_TYPE_COLON;

		if(strcmp(COMMA,currLexeme)==0)
		return TOKEN_TYPE_COMMA;

	
	}

	return TOKEN_TYPE_INVALID;
	
	

}

void CLexer::PrintToken(int tok)
{
	char *lex = GetLexeme();
	
	
	if(tok!=TOKEN_TYPE_NEWLINE)
	{
		cout<<"Lex: "<<lex<<" ";
		log->Msg("Lex: %s ",lex);
	}
	else
	{
		   cout<<"Lex: \\n ";
		log->Msg("Lex: \\n ",lex);
	}

	
	
	//this->"Lex: "<<lex<<" ";
	switch (tok)
		{
			case TOKEN_TYPE_NEWLINE:
			{
				PRINT("TOKEN_TYPE_NEWLINE");
			}break;
			case TOKEN_TYPE_INT:
			{
				PRINT("TOKEN_TYPE_INT");
			}break;
			case TOKEN_TYPE_FLOAT:
			{
				PRINT("TOKEN_TYPE_FLOAT");
			}break;
			case TOKEN_TYPE_STRING:
			{
				PRINT("TOKEN_TYPE_STRING");
			}break;
			case TOKEN_TYPE_IDENT:
			{
				PRINT("TOKEN_TYPE_IDENT");
			}break;
			case TOKEN_TYPE_COLON:
			{
				PRINT("TOKEN_TYPE_COLON");
			}break;
			case TOKEN_TYPE_OPEN_BRACKET:
			{
				PRINT("TOKEN_TYPE_OPEN_BRACKET");
			}break;
			case TOKEN_TYPE_CLOSE_BRACKET:
			{
				PRINT("TOKEN_TYPE_CLOSE_BRACKET");
			}break;
			case TOKEN_TYPE_COMMA:
			{
				PRINT("TOKEN_TYPE_COMMA");
			}break;
			case TOKEN_TYPE_OPEN_BRACE:
			{
				PRINT("TOKEN_TYPE_OPEN_BRACE");
			}break;
			case TOKEN_TYPE_CLOSE_BRACE:
			{
				PRINT("TOKEN_TYPE_CLOSE_BRACE");
			}break;


		case TOKEN_TYPE_INSTR:
			{
				PRINT("TOKEN_TYPE_INSTR");
			}break;

		case TOKEN_TYPE_SETSTACKSIZE:
			{
				PRINT("TOKEN_TYPE_SETSTACKSIZE");
			}break;

		case TOKEN_TYPE_VAR:
			{
				PRINT("TOKEN_TYPE_VAR");
			}break;
		case TOKEN_TYPE_FUNC:
			{
				PRINT("TOKEN_TYPE_FUNC");
			}break;
		case TOKEN_TYPE_PARAM:
			{
				PRINT("TOKEN_TYPE_PARAM");
			}break;
		case TOKEN_TYPE_INVALID:
			{
				PRINT("*************Tis is bad************");
				numInvalidTokens++;
			}break;

			case TOKEN_TYPE_REG_RETVAL:
			{
				PRINT("TOKEN_TYPE_REG_RETVAL");
			}break;


		default:
			PRINT("This really shouldnt happen");
		}
	PRINT(" \n");
	
}

/*
#define KEYWORD_FUNCTION "func"
#define KEYWORD_VARIABLE "var"
#define KEYWORD_PARAM "param"
#define KEYWORD_REG "reg0"
#define KEYWORD_STACK_SIZE "stacksize"
*/