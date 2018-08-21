
#define BISON_FILE FILE

enum
{
	BISON_SUCCESS =1,
	BISON_FAILURE 

};


#define	BISON_FILE_READ "r"
#define	BISON__FILE_WRITE "w"

//#define	BISON_WRITE_BINARY
#define	BISON_FILE_READ_BINARY "b"





class CFile
{
public:
	int LoadFile(char *,char *);
	void ReadFile(void*,int,int);
	void Write(char *);
	int GetFileLen();
	BISON_FILE *GetFile(){return fp;}
	
private:	
	BISON_FILE *fp;
};