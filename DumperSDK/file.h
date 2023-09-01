#ifndef FILE_H
#define FILE_H

#include "base.h"



class File
{
public:
	File(const char* FileName);

	~File();

	int fprintf(string str);

	int fprintf(const char* str);

	FILE* GetFile();

private:
	FILE* file;
};

#endif // !FILE_H
