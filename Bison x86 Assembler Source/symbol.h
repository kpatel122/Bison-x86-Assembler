
#define MAX_SYMBOL_NAME 64

struct Symbol
{
	char name[MAX_SYMBOL_NAME];
	int stackIndex;
	int size;
	int funcIndex;

};

class CSymbolTable
{
public:
	CSymbolTable();
	~CSymbolTable();
	AddSymbol(char *,int,int,int);
	Symbol *GetSymbolByName(char *,int);
	Symbol *GetSymbolByIndex(int);
	int GetNumSymbols(){return numSymbols;}

private:
	int numSymbols;
	std::vector<Symbol* > symbols;
};