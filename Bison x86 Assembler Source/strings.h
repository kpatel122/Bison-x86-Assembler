


class CStringTable
{
public:
	CStringTable(){numStrings = 0;}
	~CStringTable(){}
	int AddString(char*);
    char *GetStringByName(char *);
	char *GetStringByIndex(int);
	int DoesStringExist(char *);
	int GetNumberStrings(){return numStrings;}

private:
	int numStrings;
	std::vector<char *>strings;
};