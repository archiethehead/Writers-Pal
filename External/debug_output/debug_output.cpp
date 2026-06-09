//////////////////////
// debug_output.cpp //
//////////////////////

#ifdef __cplusplus

#include "debug_output/debug_output.h"

#ifdef _MSC_VER

void dbout(const char* string) {

	int size = sizeof(string);
	char* stringNewLine = (char*)malloc(size + 10);

	if (!stringNewLine) { return; }

	strcpy(stringNewLine, string);
	strcat(stringNewLine, "\n");

	OutputDebugStringA(stringNewLine);

	free(stringNewLine);

}

void dbout(int num) {

	int len = snprintf(NULL, 0, "%d", num);
	char* string = (char*)malloc(len + 3);

	snprintf(string, len + 1, "%d", num);

	if (!string) { return; }

	strcat(string, "\n");

	OutputDebugStringA(string);
	free(string);

}

void dbout(float num) {

	int len = snprintf(NULL, 0, "%f", num);
	char* string = (char*)malloc(len + 3);

	snprintf(string, len + 1, "%f", num);

	if (!string) { return; }

	strcat(string, "\n");

	OutputDebugStringA(string);
	free(string);

}

#else

void dbout(const char* string) {

	std::cout << string << std::endl;

}

void dbout(int num) {

	std::cout << num << std::endl;

}

void dbout(float num) {

	std::cout << num << std::endl;

}

#endif

#endif