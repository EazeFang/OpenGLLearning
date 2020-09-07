#pragma once
//用于shader文件的读写
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h> 

char *textFileRead(const char *fn);
int textFileWrite(char *fn, char *s);
unsigned char *readDataFromFile(char *fn);

