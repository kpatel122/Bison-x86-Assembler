#include <stdlib.h>         
#include <stdio.h>
#include <malloc.h>


#include "file.h"

int CFile::LoadFile(char *fileName, char *t)
{
	fp = fopen(fileName,t);
		if(!fp)
			return BISON_FAILURE;

	int len = GetFileLen();

	if(!len)
		return BISON_FAILURE;

	return BISON_SUCCESS;

}


int CFile::GetFileLen()
{
	/***********************
	Gets the length of a file
	note: OPTIMIZE ME!
	*************************/
	
	int len = 0;
	
	char tok;
	
	//get a char till we get to the end
	//of the file
	while(tok = fgetc(fp)!=EOF)
	{
		len++;
	
	}
	
	//reset the file pointer
	rewind(fp);
	
	//return the length
	return len;
}

void CFile::ReadFile(void *dest,int size, int number)
{
	fread(dest,size,number,fp);
}

void CFile::Write(char *c)
{
	fputs(c,fp);
}