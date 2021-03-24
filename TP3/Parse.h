#ifndef _PARSE_H

#define _PARSE_H

#define ERROR			-1
#define NOPARAM			0
#define OK				1

typedef int(*parseCallback) (char*, char*, void*);

int parseCmdLine(int argc, char* argv[], void* params, parseCallback pCallback);

#endif // _PARSE_H