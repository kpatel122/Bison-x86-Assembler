

class CLog
{
public:
	CLog() {writeEnable = false;}
	~CLog() {  if(fp!=NULL){fclose(fp);}}
	void Init(char*);
    void Msg(char *,...);
private:
	bool writeEnable; //do we have permission to write files?
	FILE *fp;
};