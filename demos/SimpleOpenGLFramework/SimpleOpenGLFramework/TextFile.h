#pragma once
//����shader�ļ��Ķ�д
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h> 

char *textFileRead(const char *fn);
int textFileWrite(char *fn, char *s);
unsigned char *readDataFromFile(char *fn);

