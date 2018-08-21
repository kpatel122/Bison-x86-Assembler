#include <stdlib.h>         
#include <stdio.h>
#include <malloc.h>
#include <string.h>

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

Instr *gl;

#define Error(x){ cout<<"Error: "<<x<<"\n"; numErrors++; \
log->Msg("Error : %s\n", x); return -1; }

CParser::CParser()
{
	 inFunction = script.globalSize = isMainDefined = stackSet  = 0;
	 instrIndex = script.stackSize  = numErrors = \
	 numLocalVar = numParam = currFuncIndex = currFuncStackSize = 0;
	 script.mainIndex =-1;
	 writeEnable = false;

}

void CParser::GetNextToken()
{
	int res = lexer->GetNextToken();
	tok = res;
	lex = lexer->GetLexeme();
	lexer->PrintToken(tok);
}
int CParser::InitLogFile(char *f)
{
	log = new CLog;
	log->Init(f);

	if(!log)
		return -1;

	return 1;
}

int CParser::InitLogFile(CLog *l)
{
	log = l;
	return 1;
}

CParser::~CParser()
{
	if(f!=NULL)
	{
		fclose(f);
	}
}

void CParser::SkipNewlines()
{
	int count =0;
while(tok == TOKEN_TYPE_NEWLINE)
{
		tok = lexer->GetNextToken();
		count++;
}
/*FOR DEBUG*/
if(count!=0)
{
	lexer->PrintToken(tok);
}


}

/*
int CParser::ParseStack()
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
*/


int CParser::ParseFunc()
{
	
	
	
	GetNextToken();
	SkipNewlines();

	if(inFunction == true)
	{
		Error("Cannot Function inside a function")
	}
	inFunction = true;

	if(tok!=TOKEN_TYPE_IDENT)
	{
		Error("Expected identifier")
		
	}

	currFuncIndex = script.funcTable.AddFunc(lex);
	
	if(strcmp(lex,FUNCTION_MAIN)==0)
	{
		if(isMainDefined == true)
		{
			Error("Main Already defined")
		}
		else
		{
			isMainDefined = true;
			script.mainIndex = currFuncIndex;
			
		}

	}



	if(isMainDefined != true)
	{
		
	}
	
	if(currFuncIndex == -1)
	{
		Error("Function Already Defined")
	}

	func = script.funcTable.GetFuncByIndex(currFuncIndex);
	func->numLocalVar=0;

/*	
	GetNextToken();
	SkipNewlines();
	
	if(tok!=TOKEN_TYPE_OPEN_BRACE)
	{
		Error("Expected {");
		SkipNewlines();
	}
*/
return 1;	
}


int CParser::ParseParam()
{
	int size = 1;

	if(inFunction!=true)
	{
		Error("Illegal to declare parameter in global scope")
	}

	GetNextToken();
	SkipNewlines();


	
	if(tok!=TOKEN_TYPE_IDENT)
	{
		Error("Expected identifier");
	}
	
	char funcName[64];
	strcpy(funcName,lex);
	
	GetNextToken();
	if(tok == TOKEN_TYPE_OPEN_BRACKET)
	{
		GetNextToken();
		SkipNewlines();
		if(tok!=TOKEN_TYPE_INT)
		{
			Error("Illegal referencing")
		}

		size = atoi(lex);
		
		if(size < 0)
		{
			Error("Array Size Invalid")
		}

	

		GetNextToken();
		SkipNewlines();
		if(tok!=TOKEN_TYPE_CLOSE_BRACKET)
		{
			Error("expexted ]")
		}

	}

	
	size = 1;
	

    //int s = - ((currFuncStackSize+1) + (numLocalVar) + 1);
	
	//-( pCurrFunc->iLocalDataSize + 2 + ( iCurrFuncParamCount + 1 ) );
	int s = -(func->numLocalVar+2 +(func->numParams)+1);
	if(script.symbolTable.AddSymbol(funcName,currFuncIndex,s,size) == -1)
	{
		Error("Symbol already defined")
	}


	func->numParams++;
	
	
	return 1;

}

int CParser::ParseVar()
{
	bool local = false;
	int size = 1;
	int func;

	if(inFunction == true)
	{
		local = true;

		func = currFuncIndex;
	
	}
	else
	{
		func = -1;
	}
	
	
	
	

	GetNextToken();
	SkipNewlines();

	
	
	if(tok!=TOKEN_TYPE_IDENT)
	{
		Error("Expected identifier");
	}
	
	char varName[64];
	strcpy(varName,lex);
	
	GetNextToken();
	if(tok == TOKEN_TYPE_OPEN_BRACKET)
	{
		GetNextToken();
		SkipNewlines();
		if(tok != TOKEN_TYPE_INT)
		{
			Error("Illegal array referencing")
		}

		size = atoi(lex);
		
		if(size < 0)
		{
			Error("Array Size Invalid")
		}

	

		GetNextToken();
		SkipNewlines();
		if(tok!=TOKEN_TYPE_CLOSE_BRACKET)
		{
			Error("expexted ]")
		}

	}

	int s;
	if(inFunction == true)
	{
		s = - (currFuncStackSize+2);
	}
	else
	{
		s = script.globalSize;
	}

	if(script.symbolTable.AddSymbol(varName,func,s,size) == -1)
	{
		Error("Symbol already defined")
	}

	if(local == true)
	{
		currFuncStackSize+=size;
		this->func->numLocalVar+=size;
	}
	else
	{
		script.globalSize+=size;
	}

	
	
	return 1;
}

int CParser::ValidateParam(int tok,Operand* op,Operand* dest)
{

	int test,res,base,offset =0;
	res = -1;

	switch (tok)
	{
	case TOKEN_TYPE_IDENT:
		{
			if(OP_FLAG_TYPE_FUNC_NAME & op->type)
			{
				if(script.funcTable.GetFuncByName(lex)==NULL)
				{
					Error("Parsing instr, could not find func")
				}
				test = OP_FLAG_TYPE_FUNC_NAME; 
				res = ((op->type) & (test));
				if(res)
				{
					dest->type = OP_TYPE_FUNC_INDEX;
					dest->funcIndex = atoi(lex);
				}
			}
			else
			if(script.symbolTable.GetSymbolByName(lex,currFuncIndex)!=NULL)
			{
				/*NOT SURE ABOUT THIS
				CHANGE ME
				*/
				Symbol *s = script.symbolTable.GetSymbolByName(lex,currFuncIndex);
				if(s->size > 1)//check for array
				{
					base = s->stackIndex;
					offset = 0;
					GetNextToken();
					if(this->tok !=TOKEN_TYPE_OPEN_BRACKET)
					{
						Error("Expected '['")
					}
					GetNextToken();
					if(this->tok == TOKEN_TYPE_INT)
					{
						offset = ((base) + (atoi(lex)));
						dest->stackIndex = offset;
						dest->type = OP_TYPE_ABS_STACK_INDEX;
					}
					else
					if(this->tok == TOKEN_TYPE_IDENT)
					{
						Symbol *i =script.symbolTable.GetSymbolByName(lex,currFuncIndex);
						if((i!=NULL) )
						{
								
								
							offset = i->stackIndex;
								
							dest->OffsetIndex = offset;
							dest->stackIndex = base;
							dest->type = OP_TYPE_REL_STACK_INDEX;

						}
						else
						{
							Error("Undeclared array reference");
						}
					}
					else
					{
						Error("Illegal array referencing");
					}

					GetNextToken();
					if(this->tok != TOKEN_TYPE_CLOSE_BRACKET)
					{
						Error("Expected ']' ")
					}
					
				
				}
				else 
				{
					test = OP_FLAG_TYPE_MEM_REF;
					res = ((op->type) & (test));
					if(res)
					{/*Resolve stack index*/
						dest->type = OP_TYPE_ABS_STACK_INDEX;
					
						dest->stackIndex = s->stackIndex;
					}
				}
			res = 1;
			}
			else
			
			if( (test = OP_FLAG_TYPE_HOST_API_CALL) & op->type)
			{
				res = ((op->type) & (test));
				if(res)
				{
				
					dest->type = OP_TYPE_HOST_API_CALL_INDEX;
					
					int i = script.apiFuncTable.AddString(lex);
					if(i ==-1)
					{
						Error("Could not ADD API function")
					}
					dest->hostAPICallIndex= i;
				}
			}
			else
			if( op->type & OP_FLAG_TYPE_LINE_LABEL )
			{
				if (script.labelTable.GetLabelByName(lex,currFuncIndex)==NULL)
				{
					Error("Parsing instr, could not find fund")
				}
				
				res = ((op->type) & (test));
				
				{/*Resolve stack index*/
				Label *l = script.labelTable.GetLabelByName(lex,currFuncIndex);
				dest->instrIndex = l->instrIndex; 
				dest->type = OP_TYPE_INSTR_INDEX;
				}
			}
			
			
		}break;

		case TOKEN_TYPE_INT:
		{
			test = OP_FLAG_TYPE_INT;
		
				res = ((op->type) & (test));
				if(res)
				{
					dest->type = OP_TYPE_INT;
					dest->Int = atoi(lex);
				}
		}break;
		case TOKEN_TYPE_FLOAT:
		{
			test = OP_FLAG_TYPE_FLOAT;
			
				res = ((op->type) & (test));
				if(res)
				{
					dest->type = OP_TYPE_FLOAT;
					dest->Float= atof(lex);
				}
		}break;
		case TOKEN_TYPE_STRING:
		{
			/*ADD STRING TABLE*/

			test = OP_FLAG_TYPE_STRING;
			
				res = ((op->type) & (test));
				if(res)
				{
					dest->type = OP_TYPE_STRING_INDEX;
					char buffer[1024];
					strcpy(buffer,lex);
					CStringInfo::RemoveStringQuotes(buffer);
					int i = script.stringTable.AddString(buffer);
					dest->stringTableIndex = i;
				}
				
		}break;

		case TOKEN_TYPE_REG_RETVAL:
		{
			test = OP_FLAG_TYPE_REG;
				res = ((op->type) & (test));
				if(res)
				{
					dest->type = OP_TYPE_REG;
					dest->Reg = atoi(lex);
				}

		}break;

		
	}
	
	
	/*
 TOKEN_TYPE_INT,              
        TOKEN_TYPE_FLOAT,                      
        TOKEN_TYPE_STRING,                   
                           
        TOKEN_TYPE_IDENT,
		*/
/*
#define OP_FLAG_TYPE_INT        1           // Integer literal value
#define OP_FLAG_TYPE_FLOAT      2           // Floating-point literal value
#define OP_FLAG_TYPE_STRING     4           // Integer literal value
#define OP_FLAG_TYPE_MEM_REF    8           // Memory reference (variable or array
                                                        // index, both absolute and relative)
#define OP_FLAG_TYPE_LINE_LABEL 16          // Line label (used for jumps)
#define OP_FLAG_TYPE_FUNC_NAME  32          // Function table index (used for Call)
#define OP_FLAG_TYPE_HOST_API_CALL  64      // Host API Call table index (used for
                                                        // CallHost)
#define OP_FLAG_TYPE_REG        128         // Register
*/
//res = ((op->type) & (test));	
return res;
	
}


/*
int CParser::ParseInstr()
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
	f = script.funcTable.GetFuncByIndex(currFuncIndex);
	
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

	if(instrIndex==0)
	{
			gl = script.instrStream[0];
	}

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
*/
int CParser::ParseCloseFune()
{
	if(tok!= TOKEN_TYPE_CLOSE_BRACE)
	{
		Error("Expected '}'");
	}
	

	Func *f;
	f = script.funcTable.GetFuncByIndex(currFuncIndex);
	
//	f->numLocalVar = numLocalVar;
//	f->numParams = numParam;
//	f->stackSize = (currFuncStackSize+1);
	
	numLocalVar = numParam = 0;

	inFunction = false;
	
	
	if(currFuncIndex!=script.mainIndex)
	{
		Instr *inst = (Instr*)malloc(sizeof(Instr));
		inst->numberParams = 1;
		inst->opCode = INSTR_EXIT;
	
		inst->opList = (Operand*)malloc(sizeof(Operand));
		inst->opList[0].Int = 0;
		inst->opList[0].type = OP_TYPE_INT;
		script.instrStream.push_back(inst);
	

	
		instrIndex++;
	}

	return 1;
	
}
/*
int CParser::ParseLineLabel()
{
	
	char labelName[64];
	strcpy(labelName,lex);
	
	GetNextToken();

	if(tok!=TOKEN_TYPE_COLON)
	{
		Error("Missin ':' after line label")
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
*/

/*
int CParser::Parse()
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
		}break;

	case TOKEN_TYPE_PARAM:
		{
			ParseParam();
		}break;
	case TOKEN_TYPE_VAR:
		{
			ParseVar();
		}break;
	case TOKEN_TYPE_INSTR:
		{
			
			ParseInstr();
		}break;
	case TOKEN_TYPE_CLOSE_BRACE:
		{
			ParseCloseFune();
		}break;
	case TOKEN_TYPE_IDENT:
		{
			ParseLineLabel();
		}break;



	}
	
		
	

	
	
	return 1;
}
*/

void CParser::PrintStats()
{
	char buffer[80];
	sprintf(buffer,"\nnumber strings : %d\n",script.stringTable.GetNumberStrings());
	PRINT(buffer);

	sprintf(buffer,"number api callsd : %d\n",script.apiFuncTable.GetNumberStrings());
	PRINT(buffer);
	
	sprintf(buffer,"number functions : %d\n",script.funcTable.GetNumFunc());
	PRINT(buffer);
	
	sprintf(buffer,"number line lables : %d\n",script.labelTable.GetNumberLabels());
	PRINT(buffer);

	sprintf(buffer,"number symbols : %d\n",script.symbolTable.GetNumSymbols());
	PRINT(buffer);
	
	sprintf(buffer,"number instruction : %d\n",instrIndex);
	PRINT(buffer);
	
	sprintf(buffer,"number Global vars : %d\n",script.globalSize);
	PRINT(buffer);
	
	sprintf(buffer,"is main present : %d\n",isMainDefined);
	PRINT(buffer);
	
	


	
}



int CParser::WriteInstructions()
{
	Instr *instr;

	for(int i=0;i<instrIndex;i++)
	{
		instr = script.instrStream[i];

		if(writeEnable==true)
		{
			fwrite(&instr->opCode,sizeof(int),1,f);
			fwrite(&instr->numberParams,sizeof(int),1,f);
		}

	
		
		for(int j=0; j<instr->numberParams;j++)
		{
			if(writeEnable==true)
				fwrite(&instr->opList[j].type,sizeof(int),1,f);
			
			
			switch(instr->opList[j].type)
			{
			
			case OP_TYPE_INSTR_INDEX:
				{
					//fwrite(&instr->opList[j].Int,sizeof(int),1,f);
				}break;
			
			case OP_TYPE_INT:
				{
					if(writeEnable==true)
					fwrite(&instr->opList[j].Int,sizeof(int),1,f);
				}break;
			case OP_TYPE_FLOAT:
				{
					if(writeEnable==true)
					fwrite(&instr->opList[j].Float,sizeof(float),1,f);
				}break;
			case OP_TYPE_STRING_INDEX:
				{
					if(writeEnable==true)
					fwrite(&instr->opList[j].stringTableIndex,sizeof(int),1,f);
				}break;
			case OP_TYPE_REL_STACK_INDEX:
				{
					if(writeEnable==true)
					{
						fwrite(&instr->opList[j].stackIndex,sizeof(int),1,f);
						fwrite(&instr->opList[j].OffsetIndex,sizeof(int),1,f);
					}
				}break;
			case OP_TYPE_ABS_STACK_INDEX:
				{
					if(writeEnable==true)
					fwrite(&instr->opList[j].stackIndex,sizeof(int),1,f);
				}break;
			case OP_TYPE_FUNC_INDEX:
				{
					if(writeEnable==true)
					fwrite(&instr->opList[j].funcIndex,sizeof(int),1,f);
				}break;
			case OP_TYPE_HOST_API_CALL_INDEX:
				{
					if(writeEnable==true)
					fwrite(&instr->opList[j].hostAPICallIndex,sizeof(int),1,f);
				}break;
			case OP_TYPE_REG :
				{
					if(writeEnable==true)
					fwrite(&instr->opList[j].Reg,sizeof(int),1,f);
				}break;
			
			default:
				{
					cout<<"Unkown op type: "<<instr->opList[j].type<<endl;
						Error(instr->opList[j].type);
				}break;

			}
		}
		
	}
	return 1;
}

int CParser::WriteFuncTable()
{
	Func * func;

	for(int i=0;i<script.funcTable.GetNumFunc();i++)
	{
		func = script.funcTable.GetFuncByIndex(i);

		if(!func)
		{
			Error("Invalid function index")
		}
		
		if(writeEnable==true)
		{
			fwrite(&func->entryIndex,sizeof(int),1,f);
			fwrite(&func->numLocalVar,sizeof(int),1,f);
			fwrite(&func->numParams,sizeof(int),1,f);
			fwrite(&func->stackSize,sizeof(int),1,f);
		}
	}
	return 1;
}
int CParser::WriteLabelTable()
{
	Label *label;
	
	for(int i=0;i<script.labelTable.GetNumberLabels();i++)
	{
		label = script.labelTable.GetLabelByIndex(i);

		if(!label)
		{
			Error("Invalid function index")
		}
		
		if(writeEnable==true)
		{
			fwrite(&label->funcIndex,sizeof(int),1,f);
			fwrite(&label->instrIndex,sizeof(int),1,f);
		}
	}
	return 1;
}

int CParser::WriteSymbolTable()
{
	Symbol * symbol;
	
	for(int i=0;i<script.symbolTable.GetNumSymbols();i++)
	{
		symbol = script.symbolTable.GetSymbolByIndex(i);

		if(!symbol)
		{
			Error("Invalid symbol index")
		}
		
		if(writeEnable==true)
		{
			fwrite(&symbol->funcIndex,sizeof(int),1,f);
			fwrite(&symbol->size,sizeof(int),1,f);
			fwrite(&symbol->stackIndex,sizeof(int),1,f);
		}
	}
	return 1;
}

int CParser::WriteStringTable()
{
	char *str;

	for(int i=0;i<script.stringTable.GetNumberStrings();i++)
	{
		str = script.stringTable.GetStringByIndex(i);

		if(!str)
		{
			Error("Invalid function index")
		}
		int len = strlen(str);

		if(writeEnable==true)
		{
			fwrite(&len,sizeof(int),1,f);
			fwrite(str,sizeof(char),len,f);
		}
	}
	return 1;
}
int CParser::WriteAPITable()
{
	char *apiFunc;

	for(int i=0;i<script.apiFuncTable.GetNumberStrings();i++)
	{
		apiFunc = script.apiFuncTable.GetStringByIndex(i);

		if(!apiFunc)
		{
			Error("Invalid function index")
		}

		int len = strlen(apiFunc);
		
		if(writeEnable==true)
		{
			fwrite(&len,sizeof(int),1,f);
			fwrite(apiFunc,len,sizeof(char),f);
		}
		
	}
	return 1;

}
int CParser::WriteHeader()
{
	
	BASM_Header header;

	header.id[0] = 'B';
	header.id[1] = 'A';
	header.id[2] = 'S';
	//header.id[3] = 'M';
	
	header.main = script.mainIndex;
	header.stackSize = script.stackSize;
	header.numStrings = script.stringTable.GetNumberStrings();
	header.numAPICalls = script.apiFuncTable.GetNumberStrings();
	header.numFunc = script.funcTable.GetNumFunc();
	header.numLineLable= script.labelTable.GetNumberLabels();
	header.numSymbols = script.symbolTable.GetNumSymbols();
	header.numInstr = instrIndex;
	header.globalSize = script.globalSize;


	if(writeEnable == true)
	fwrite(&header,sizeof(BASM_Header),1,f);
	
	return 1;

}
int CParser::WriteASMFile(char *filename)
{
	
	

	PRINT("Writing BXE file...\n");
	

	if(!strstr(filename,".bxe"))
	{
		Error("Invlid output file name");
	}

	f = fopen(filename,"wb");
	if(!f)
	{
		
		PRINT("Could not create file because the output file is read-only\n");
		PRINT("If this program is being run from the CD please transfer\n");
		PRINT("to the C drive and change the file attribute settings\n");
		Error("Could Not open output file");
	}
	else
		writeEnable=true;
	
WriteHeader();
WriteInstructions();
WriteFuncTable();
WriteLabelTable();
WriteSymbolTable();
WriteStringTable();
WriteAPITable();
	


	

	

	


	

	PRINT("File Created suceesfuly!\n");
	
	return 1;
}