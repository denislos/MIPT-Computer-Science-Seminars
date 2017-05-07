
#ifndef FILE_UTILS_H
#define FILE_UTILS_H


#include <stdio.h>




size_t filelength(FILE* file);

unsigned char* getInputBuffer(FILE* file, size_t* file_length);



#endif //FILE_UTILS_H