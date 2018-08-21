
#define MAX_LABAL_NAME 64

struct Label
{
	char name[MAX_LABAL_NAME];
	int funcIndex;
	int instrIndex;

};


class CLabelTable
{
public:
	CLabelTable(){numLabels=0;}
	~CLabelTable(){}
	int AddLabel(char*,int,int);
    Label *GetLabelByName(char *,int);
	Label *GetLabelByIndex(int);
	int DoesLabelExist(char *,int);
	int GetNumberLabels(){return numLabels;}

private:
	int numLabels;
	std::vector<Label *>labels;
};