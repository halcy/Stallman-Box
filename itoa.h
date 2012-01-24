#ifndef ITOA_H
#define ITOA_H

#include "tools.h"

/**
 * Ansi C "itoa" based on Kernighan & Ritchie's "Ansi C"
 * with slight modification to optimize for specific architecture:
 */
void strreverse(char* begin, char* end);
void itoa(int value, char* str, int base);

#endif