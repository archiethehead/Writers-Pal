////////////////////
// debug_output.h //
////////////////////

#pragma once

#include <stdio.h>

#ifdef _MSC_VER
#include <windows.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif


	void dbouts(const char* string);
	void dbouti(int num);
	void dboutf(float num);


#ifdef __cplusplus
}
#endif