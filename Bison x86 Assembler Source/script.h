struct Script
{


	std::vector<Instr *> instrStream;

	CFuncTable   funcTable;
	CSymbolTable symbolTable;
	CStringTable stringTable;
	CStringTable apiFuncTable;
	CLabelTable  labelTable;
	int globalSize;
	int stackSize;
	int mainIndex;

	FILE *fp;

};