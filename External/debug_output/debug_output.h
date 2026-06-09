////////////////////
// debug_output.h //
////////////////////

#pragma once

#ifdef _MSC_VER
#include <windows.h>
#endif

#ifdef __cplusplus

#include <iostream>

extern void dbout(const char* string);
extern void dbout(int num);
extern void dbout(float num);

#else

#include <stdio.h>

extern void dbouts(const char* string);
extern void dbouti(int num);
extern void dboutf(float num);


#endif