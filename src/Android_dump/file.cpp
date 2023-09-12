#include "file.h"



File::File(const char* FileName)
{
    fopen(FileName, "w+");
}

File::~File()
{
	if (file)
		fclose(file);
}

int File::fprintf(string str)
{
	return std::fprintf(file, "%s", str.c_str());
}

int File::fprintf(const char* str)
{
	return std::fprintf(file, "%s", str);
}

FILE* File::GetFile()
{
	return file;
}