#include <stdio.h>
#include <stdarg.h>
#include "gds_globals.h"

bool verbose_output;

void v_printf(const char *fmt, ...)
{
	if(verbose_output){
		va_list va;
		va_start(va, fmt);
		vprintf(fmt, va);
		va_end(va);
	}
}

