#include <iostream.h>
#include <stdlib.h>         
#include <stdio.h>
#include <malloc.h>
#include <string.h>


#include "token.h"
#include "instructions.h"
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

#define ASM_FILE_IN  "bspLoad.basm"
#define BXE_FILE_OUT "exes\\bspLoad.bxe"


CInstrTable instrTable;

void AddAllInstructions();
void PrintToken(int tok);

int numInvalidTokens = 0;

void main()
{
	int instrIndex =0;
	
	AddAllInstructions();
	
	CLog log;
	log.Init("log.txt");
	
/*
char *t = "\"   abc   \"";
char *k = (char*)malloc((sizeof(char)) * strlen(t));

strcpy(k,t);


cout<<"k before: "<<k<<"\n";
CStringInfo::RemoveStringQuotes(k);
cout<<"k After: "<<k<<"\n";
*/


	CLexer lexer;
	ASMParser parser;

	lexer.InitLogFile(&log);
	parser.InitLogFile(&log);

	lexer.LoadASMFile(ASM_FILE_IN);
	lexer.PrepFile();
	lexer.AssignInstrTable(&instrTable);

	parser.AssignLexer(&lexer);

	char *lex;
	int tok;


/*
	cout<<"LEXEMES" <<"\n\n";
	while(!(lexer.IsFileFinished()))
	{
		
		tok = lexer.GetNextToken();
		lex = (char*)lexer.GetLexeme();

		if(tok!=TOKEN_TYPE_NEWLINE)
		{
			cout <<"Lexeme: " <<lex<< " Token: "; PrintToken(tok); cout<<"\n";
		}
		else
		{
			cout<<"Token: "; PrintToken(tok); cout<<"\n";
		}

	}

*/

	

	parser.Parse();

cout <<"\nlex errors: "<<numInvalidTokens<<" parse errors: "<<parser.GetErrors()<<endl;
parser.log->Msg("\nlex errors: %d  parse errors: %d ",numInvalidTokens,parser.GetErrors() );


parser.PrintStats();

if (parser.GetErrors()==0)
{

	parser.WriteASMFile(BXE_FILE_OUT);
}




cout<<"\npress any key then enter to quit."<<endl;

/*
to stop this demo from being just a flash on the screen in release mode
get some dummy input when everythings finished
*/

char dummy[10];
cin>>dummy;


}

/*
TOKEN_TYPE_QUOTE,                     
TOKEN_TYPE_NEWLINE,      
*/
void PrintToken(int tok)
{
		switch (tok)
		{
			case TOKEN_TYPE_NEWLINE:
			{
				cout<<"TOKEN_TYPE_NEWLINE";
			}break;
			case TOKEN_TYPE_INT:
			{
				cout<<"TOKEN_TYPE_INT";
			}break;
			case TOKEN_TYPE_FLOAT:
			{
				cout<<"TOKEN_TYPE_FLOAT";
			}break;
			case TOKEN_TYPE_STRING:
			{
				cout<<"TOKEN_TYPE_STRING";
			}break;
			case TOKEN_TYPE_IDENT:
			{
				cout<<"TOKEN_TYPE_IDENT";
			}break;
			case TOKEN_TYPE_COLON:
			{
				cout<<"TOKEN_TYPE_COLON";
			}break;
			case TOKEN_TYPE_OPEN_BRACKET:
			{
				cout<<"TOKEN_TYPE_OPEN_BRACKET";
			}break;
			case TOKEN_TYPE_CLOSE_BRACKET:
			{
				cout<<"TOKEN_TYPE_CLOSE_BRACKET";
			}break;
			case TOKEN_TYPE_COMMA:
			{
				cout<<"TOKEN_TYPE_COMMA";
			}break;
			case TOKEN_TYPE_OPEN_BRACE:
			{
				cout<<"TOKEN_TYPE_OPEN_BRACE";
			}break;
			case TOKEN_TYPE_CLOSE_BRACE:
			{
				cout<<"TOKEN_TYPE_CLOSE_BRACE";
			}break;


		case TOKEN_TYPE_INSTR:
			{
				cout<<"TOKEN_TYPE_INSTR";
			}break;

		case TOKEN_TYPE_SETSTACKSIZE:
			{
				cout<<"TOKEN_TYPE_SETSTACKSIZE";
			}break;

		case TOKEN_TYPE_VAR:
			{
				cout<<"TOKEN_TYPE_VAR";
			}break;
		case TOKEN_TYPE_FUNC:
			{
				cout<<"TOKEN_TYPE_FUNC";
			}break;
		case TOKEN_TYPE_PARAM:
			{
				cout<<"TOKEN_TYPE_PARAM";
			}break;
		case TOKEN_TYPE_INVALID:
			{
				cout<<"*************Tis is bad************";
				numInvalidTokens++;
			}break;

			case TOKEN_TYPE_REG_RETVAL:
			{
				cout<<"TOKEN_TYPE_REG_RETVAL";
			}break;


		default:
			cout<<"This really shouldnt happen";
		}
}

void AddAllInstructions()
{
	int instrIndex = 0;
    instrIndex =	instrTable.AddInstr("MOV",INSTR_MOV,2);	
	instrTable.SetInstrOp(instrIndex,0,OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );
	
	instrTable.SetInstrOp(instrIndex,1,OP_FLAG_TYPE_INT |
                                    OP_FLAG_TYPE_FLOAT |
                                    OP_FLAG_TYPE_STRING |
                                    OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );
	
instrIndex = 	 instrTable.AddInstr("ADD", INSTR_ADD, 2 );
     instrTable.SetInstrOp(instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );
       instrTable.SetInstrOp(instrIndex, 1, OP_FLAG_TYPE_INT |
                                    OP_FLAG_TYPE_FLOAT |
                                    OP_FLAG_TYPE_STRING |
                                    OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );

        // Sub          Destination, Source

 instrIndex =      instrTable.AddInstr( "SUB", INSTR_SUB, 2 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );
        instrTable.SetInstrOp( instrIndex, 1, OP_FLAG_TYPE_INT |
                                    OP_FLAG_TYPE_FLOAT |
                                    OP_FLAG_TYPE_STRING |
                                    OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );

////////////////////////////////////////////////////////////////////////////////


		 // Mul          Destination, Source

        instrIndex = instrTable.AddInstr ( "MUL", INSTR_MUL, 2 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );
        instrTable.SetInstrOp( instrIndex, 1, OP_FLAG_TYPE_INT |
                                    OP_FLAG_TYPE_FLOAT |
                                    OP_FLAG_TYPE_STRING |
                                    OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );

        // Div          Destination, Source

        instrIndex = instrTable.AddInstr ( "DIV", INSTR_DIV, 2 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );
        instrTable.SetInstrOp( instrIndex, 1, OP_FLAG_TYPE_INT |
                                    OP_FLAG_TYPE_FLOAT |
                                    OP_FLAG_TYPE_STRING |
                                    OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );

        // Mod          Destination, Source

        instrIndex = instrTable.AddInstr ( "MOD", INSTR_MOD, 2 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );
        instrTable.SetInstrOp( instrIndex, 1, OP_FLAG_TYPE_INT |
                                    OP_FLAG_TYPE_FLOAT |
                                    OP_FLAG_TYPE_STRING |
                                    OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );

        // Exp          Destination, Source

        instrIndex = instrTable.AddInstr ( "EXP", INSTR_EXP, 2 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );
        instrTable.SetInstrOp( instrIndex, 1, OP_FLAG_TYPE_INT |
                                    OP_FLAG_TYPE_FLOAT |
                                    OP_FLAG_TYPE_STRING |
                                    OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );

        // Neg          Destination

        instrIndex = instrTable.AddInstr ( "NEG", INSTR_NEG, 1 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );

        // Inc          Destination

        instrIndex = instrTable.AddInstr ( "INC", INSTR_INC, 1 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );

        // Dec          Destination

        instrIndex = instrTable.AddInstr ( "DEC", INSTR_DEC, 1 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );

        // ---- Bitwise

        // And          Destination, Source

        instrIndex = instrTable.AddInstr ( "AND", INSTR_AND, 2 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );
        instrTable.SetInstrOp( instrIndex, 1, OP_FLAG_TYPE_INT |
                                    OP_FLAG_TYPE_FLOAT |
                                    OP_FLAG_TYPE_STRING |
                                    OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );

        // Or           Destination, Source

        instrIndex = instrTable.AddInstr ( "OR", INSTR_OR, 2 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );
        instrTable.SetInstrOp( instrIndex, 1, OP_FLAG_TYPE_INT |
                                    OP_FLAG_TYPE_FLOAT |
                                    OP_FLAG_TYPE_STRING |
                                    OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );

        // XOr          Destination, Source

        instrIndex = instrTable.AddInstr ( "XOR", INSTR_XOR, 2 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );
        instrTable.SetInstrOp( instrIndex, 1, OP_FLAG_TYPE_INT |
                                    OP_FLAG_TYPE_FLOAT |
                                    OP_FLAG_TYPE_STRING |
                                    OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );

        // Not          Destination

        instrIndex = instrTable.AddInstr ( "NOT", INSTR_NOT, 1 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );

        // ShL          Destination, Source

        instrIndex = instrTable.AddInstr ( "SHL", INSTR_SHL, 2 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );
        instrTable.SetInstrOp( instrIndex, 1, OP_FLAG_TYPE_INT |
                                    OP_FLAG_TYPE_FLOAT |
                                    OP_FLAG_TYPE_STRING |
                                    OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );

        // ShR          Destination, Source

        instrIndex = instrTable.AddInstr ( "SHR", INSTR_SHR, 2 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );
        instrTable.SetInstrOp( instrIndex, 1, OP_FLAG_TYPE_INT |
                                    OP_FLAG_TYPE_FLOAT |
                                    OP_FLAG_TYPE_STRING |
                                    OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );

        // ---- String Manipulation

        // Concat       String0, String1

        instrIndex = instrTable.AddInstr ( "CONCAT", INSTR_CONCAT, 2 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );
        instrTable.SetInstrOp( instrIndex, 1, OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG |
                                    OP_FLAG_TYPE_STRING );

       // GetChar      Destination, Source, Index

        instrIndex = instrTable.AddInstr ( "GETCHAR", INSTR_GETCHAR, 3 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );
        instrTable.SetInstrOp( instrIndex, 1, OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG |
                                    OP_FLAG_TYPE_STRING );
        instrTable.SetInstrOp( instrIndex, 2, OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG |
                                    OP_FLAG_TYPE_INT );

        // SetChar      Destination, Index, Source

        instrIndex = instrTable.AddInstr ( "SETCHAR", INSTR_SETCHAR, 3 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );
        instrTable.SetInstrOp( instrIndex, 1, OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG |
                                    OP_FLAG_TYPE_INT );
        instrTable.SetInstrOp( instrIndex, 2, OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG |
                                    OP_FLAG_TYPE_STRING );

        // ---- Conditional Branching

        // Jmp          Label

        instrIndex = instrTable.AddInstr ( "JMP", INSTR_JMP, 1 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_LINE_LABEL );

        // JE           Op0, Op1, Label

        instrIndex = instrTable.AddInstr ( "JE", INSTR_JE, 3 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_INT |
                                    OP_FLAG_TYPE_FLOAT |
                                    OP_FLAG_TYPE_STRING |
                                    OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );
        instrTable.SetInstrOp( instrIndex, 1, OP_FLAG_TYPE_INT |
                                    OP_FLAG_TYPE_FLOAT |
                                    OP_FLAG_TYPE_STRING |
                                    OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );
        instrTable.SetInstrOp( instrIndex, 2, OP_FLAG_TYPE_LINE_LABEL );

        // JNE          Op0, Op1, Label

        instrIndex = instrTable.AddInstr ( "JNE", INSTR_JNE, 3 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_INT |
                                    OP_FLAG_TYPE_FLOAT |
                                    OP_FLAG_TYPE_STRING |
                                    OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );
        instrTable.SetInstrOp( instrIndex, 1, OP_FLAG_TYPE_INT |
                                    OP_FLAG_TYPE_FLOAT |
                                    OP_FLAG_TYPE_STRING |
                                    OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );
        instrTable.SetInstrOp( instrIndex, 2, OP_FLAG_TYPE_LINE_LABEL );

        // JG           Op0, Op1, Label

        instrIndex = instrTable.AddInstr ( "JG", INSTR_JG, 3 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_INT |
                                    OP_FLAG_TYPE_FLOAT |
                                    OP_FLAG_TYPE_STRING |
                                    OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );
        instrTable.SetInstrOp( instrIndex, 1, OP_FLAG_TYPE_INT |
                                    OP_FLAG_TYPE_FLOAT |
                                    OP_FLAG_TYPE_STRING |
                                    OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );
        instrTable.SetInstrOp( instrIndex, 2, OP_FLAG_TYPE_LINE_LABEL );

        // JL           Op0, Op1, Label

        instrIndex = instrTable.AddInstr ( "JL", INSTR_JL, 3 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_INT |
                                    OP_FLAG_TYPE_FLOAT |
                                    OP_FLAG_TYPE_STRING |
                                    OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );
        instrTable.SetInstrOp( instrIndex, 1, OP_FLAG_TYPE_INT |
                                    OP_FLAG_TYPE_FLOAT |
                                    OP_FLAG_TYPE_STRING |
                                    OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );
        instrTable.SetInstrOp( instrIndex, 2, OP_FLAG_TYPE_LINE_LABEL );

        // JGE          Op0, Op1, Label

        instrIndex = instrTable.AddInstr ( "JGE", INSTR_JGE, 3 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_INT |
                                    OP_FLAG_TYPE_FLOAT |
                                    OP_FLAG_TYPE_STRING |
                                    OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );
        instrTable.SetInstrOp( instrIndex, 1, OP_FLAG_TYPE_INT |
                                    OP_FLAG_TYPE_FLOAT |
                                    OP_FLAG_TYPE_STRING |
                                    OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );
        instrTable.SetInstrOp( instrIndex, 2, OP_FLAG_TYPE_LINE_LABEL );

        // JLE           Op0, Op1, Label

        instrIndex = instrTable.AddInstr ( "JLE", INSTR_JLE, 3 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_INT |
                                    OP_FLAG_TYPE_FLOAT |
                                    OP_FLAG_TYPE_STRING |
                                    OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );
        instrTable.SetInstrOp( instrIndex, 1, OP_FLAG_TYPE_INT |
                                    OP_FLAG_TYPE_FLOAT |
                                    OP_FLAG_TYPE_STRING |
                                    OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );
        instrTable.SetInstrOp( instrIndex, 2, OP_FLAG_TYPE_LINE_LABEL );

        // ---- The Stack Interface

        // Push          Source

        instrIndex = instrTable.AddInstr ( "PUSH", INSTR_PUSH, 1 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_INT |
                                    OP_FLAG_TYPE_FLOAT |
                                    OP_FLAG_TYPE_STRING |
                                    OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );

        // Pop           Destination

        instrIndex = instrTable.AddInstr ( "POP", INSTR_POP, 1 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );

        // ---- The Function Interface

        // Call          FunctionName

        instrIndex = instrTable.AddInstr ( "CALL", INSTR_CALL, 1 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_FUNC_NAME );

        // Ret

        instrIndex = instrTable.AddInstr ( "RET", INSTR_RET, 0 );

        // CallHost      FunctionName

        instrIndex = instrTable.AddInstr ( "CALLHOST", INSTR_CALLHOST, 1 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_HOST_API_CALL );

        // ---- Miscellaneous

        // Pause        Duration

        instrIndex = instrTable.AddInstr ( "PAUSE", INSTR_PAUSE, 1 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_INT |
                                    OP_FLAG_TYPE_FLOAT |
                                    OP_FLAG_TYPE_STRING |
                                    OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );

        // Exit         Code

        instrIndex = instrTable.AddInstr ( "EXIT", INSTR_EXIT, 1 );
        instrTable.SetInstrOp( instrIndex, 0, OP_FLAG_TYPE_INT |
                                    OP_FLAG_TYPE_FLOAT |
                                    OP_FLAG_TYPE_STRING |
                                    OP_FLAG_TYPE_MEM_REF |
                                    OP_FLAG_TYPE_REG );


}