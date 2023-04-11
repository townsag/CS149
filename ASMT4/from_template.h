#ifndef FROM_TEMPLATE_H
#define FROM_TEMPLATE_H


void PUSH_TRACE(char* p);
void POP_TRACE();
char* PRINT_TRACE();

void* REALLOC(void* p,int t,char* file,int line);
void* MALLOC(int t,char* file,int line);
void FREE(void* p,char* file,int line);

#define realloc(a,b) REALLOC(a,b,__FILE__,__LINE__)
#define malloc(a) MALLOC(a,__FILE__,__LINE__)
#define free(a) FREE(a,__FILE__,__LINE__)

#endif

