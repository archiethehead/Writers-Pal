////////////////////
// debug_output.c //
////////////////////

#include "debug_output.h"

#ifdef _MSC_VER

void dbouts(const char* string) {

	int size = sizeof(string);
	char* stringNewLine = (char*)malloc(size + 2);

	if (!stringNewLine) { return; }

	strcpy(stringNewLine, string);
	strcat(stringNewLine, "\n");

	OutputDebugStringA(stringNewLine);

	free(stringNewLine);

}

void dbouti(int num) {

	int len = snprintf(NULL, 0, "%d", num);
	char* string = (char*)malloc(len + 3);

	snprintf(string, len + 1, "%d", num);

	if (!string) { return; }

	strcat(string, "\n");

	OutputDebugStringA(string);
	free(string);

}

void dboutf(float num) {

	int len = snprintf(NULL, 0, "%f", num);
	char* string = (char*)malloc(len + 3);

	snprintf(string, len + 1, "%f", num);

	if (!string) { return; }

	strcat(string, "\n");

	OutputDebugStringA(string);
	free(string);

}

#else

void dbouts(const char* string) {

	printf("%s", string);

}

void dbouti(int num) {

	prtinf("%d", num);

}

void dboutf(float num) {

	printf("%f", num);

}

#endif