
#define MAX_LEXEME_SIZE 1024

class CLexer
{

public:	
	CLexer();
	~CLexer();
	int InitLogFile(char *);
	int InitLogFile(CLog *l);
	int GetNextToken();
	char *GetCurrentLexeme();
	char *GetLexeme(){return currLexeme;}
	
	int GetNumberLines() {return numberLines;}
	void SkipLine();
	int LoadASMFile(char*);
	char *GetSource(){return pSource;}
	void PrepFile();
	bool IsFileFinished() {return endOfFile;} 
	void AssignInstrTable (CInstrTable* table)  {instrTable = table;}
	void PrintToken(int);
	CInstrTable *instrTable;
	void  Reset();
	void MoveToNextLine();
	char *currPos;
private:
	
	CFile *fp;
	CLog *log;
	
	void SkipWhiteSpaces();
	void GetNextValidChar();
	void StripComments(char *,int);
	void TrimWhiteSpaces(char *,int);
	
	bool EndOfFile();
	
	
	char *pSource;
	char **ppSource;
	
	char *endPos;
	char *currLexeme;
	int currToken;
	bool endOfFile;
	
	int numInvalidTokens;
	int numberLines;
	int currentLine;
	int currLineLength;
	int index0;
	int index1;
	int lines;
	int len;
	char lexBuffer[MAX_LEXEME_SIZE];
	bool newLineFound;
	

};