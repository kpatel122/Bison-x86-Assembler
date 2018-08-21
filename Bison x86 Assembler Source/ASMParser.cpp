
#include <iostream.h>
#include "error.h"
#include "instructions.h"
#include "token.h"
#include "file.h"
#include "log.h"
#include "String.h"
#include "lexer.h"
#include "functions.h"
#include "symbol.h"
#include "strings.h"
#include "label.h"
#include "script.h"
#include "parser.h"
#include "ASMParser.h"



int ASMParser::ParseStack()
{
	if(stackSet == true)
	{
		Error("Stack Already Set")
	}
	if(inFunction == true)
	{
		Error("Cannot Declare Stack inside a function")
	}
			
	stackSet = true;

	SkipNewlines();
	GetNextToken();
	if(tok!= TOKEN_TYPE_INT)
	{
		Error(ERROR_EXPECTED_INT);
				
	}
			
	GetLexeme();
	script.stackSize = atoi(lex);
	
//	GetNextToken();
	return 1;
}

int ASMParser::ParseInstr()
{
		InstrLookup *instr;
	
	instr = lexer->instrTable->GetInstrByName(lex);

	if(!instr)
	{
		return 1;
	}

	if(inFunction!=true)
	{
		Error("Instruction outside of Function");

	}
	Func *f;
	f = func;//script.funcTable.GetFuncByIndex(currFuncIndex);
	
	if(f->entryIndex == -1)
	{
		f->entryIndex = instrIndex;
	}

	int params = instr->numParam;
	
	GetNextToken();
	SkipNewlines();
	

	Instr *inst = (Instr*)malloc(sizeof(Instr));
	inst->numberParams = params;
	inst->opCode = instr->token;
	
	inst->opList = (Operand*)malloc(sizeof(Operand)*params);
	script.instrStream.push_back(inst);



	for(int i=0;i<params;i++)
	{
		if((ValidateParam(tok,&instr->opList[i],&inst->opList[i]) )==-1)
		{
			Error("Invalid Parameter")
		}
		Instr *ti;
		ti = script.instrStream[instrIndex];

		
		
		if(tok == TOKEN_TYPE_NEWLINE)
		{
			Error("Not enough parameters")
		}

		if( (i+1) >= (params) )
		{
			continue;
		}
		else
		{
			GetNextToken();
			if(tok != TOKEN_TYPE_COMMA)
			{
				Error("Expected ','")
			}
			else
			{
			GetNextToken();
			}

		}
	}

	GetNextToken();

	if(tok!=TOKEN_TYPE_NEWLINE)
	{
		Error("To many instructions in constant");
	}


	//Instr *i = 
	instrIndex ++;
	return 1;
}
int ASMParser::ParseLineLabel()
{
	char labelName[64];
	strcpy(labelName,lex);
	
	GetNextToken();

	if(tok!=TOKEN_TYPE_COLON)
	{
		return-1;
		//Error("Missin ':' after line label")
	}

	if(script.labelTable.GetLabelByName(labelName,currFuncIndex)!=NULL)
	{
		Error("Labal already defined")
	}
	
	if((script.labelTable.AddLabel(labelName,currFuncIndex,instrIndex))==-1)
	{
		Error("Could not add lablel")
	}


	return 1;
}
int ASMParser::Parse()
{
	//gCurrPos = lexer->currPos;
	char & gCurrPos = *lexer->currPos;
while(!(lexer->IsFileFinished()))
{
	

	GetNextToken();
	SkipNewlines();


	switch(tok)
	{
	case TOKEN_TYPE_SETSTACKSIZE:
		{
			
			ParseStack();
			
		}break;
	case TOKEN_TYPE_FUNC:
		{
			ParseFunc();
			GetNextToken();
			
			SkipNewlines();
	
			if(tok!=TOKEN_TYPE_OPEN_BRACE)
			{
			Error("Expected {");
			SkipNewlines();
			}
		}break;

	case TOKEN_TYPE_PARAM:
		{
			lexer->MoveToNextLine();
			//ParseParam();
		}break;
	case TOKEN_TYPE_VAR:
		{
			ParseVar();
		}break;
	case TOKEN_TYPE_INSTR:
		{
		
		lexer->MoveToNextLine();
		}break;
	case TOKEN_TYPE_CLOSE_BRACE:
		{
		numLocalVar = numParam = 0;
		
		if(!inFunction){
			Error("invalid brace\n")}

		inFunction = false;
		lexer->MoveToNextLine();
		//	ParseCloseFune();
		}break;
	case TOKEN_TYPE_IDENT:
		{
			//lexer->MoveToNextLine();
			ParseLineLabel();
		}break;



	}
	
}		
	
lexer->Reset();

PRINT("Commencing Second Pass\n");

while(!(lexer->IsFileFinished()))
{
	


	GetNextToken();
	SkipNewlines();

	
	
	switch(tok)
	{
	case TOKEN_TYPE_INSTR:
		{
			ParseInstr();
		}break;
   case TOKEN_TYPE_FUNC:
		{
			inFunction = true;
			GetNextToken();
			SkipNewlines();
			func= this->script.funcTable.GetFuncByName(lex);
			currFuncIndex =this->script.funcTable.GetFuncIndex(lex);
		
			


		}break;
	case TOKEN_TYPE_CLOSE_BRACE:
		{
				ParseCloseFune();
			inFunction=false;
			currFuncIndex=-1;
		}break;

	case TOKEN_TYPE_PARAM:
		{
			ParseParam();
			func->stackSize = func->numLocalVar+func->numParams;
		}break;

	case TOKEN_TYPE_IDENT:
		{
		//	ParseLineLabel();
		}break;
	
		
	
	
	}

}
	
	
	return 1;
}