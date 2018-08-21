
#define Error(x){ cout<<"Error: "<<x<<"\n"; numErrors++; \
log->Msg("Error : %s\n", x); return -1; }

class ASMParser : public CParser
{
	
	int ParseStack();
	int ParseInstr();
	int ParseLineLabel();
public:	
	int Parse();
};