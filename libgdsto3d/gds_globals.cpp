#include <stdio.h>
#include <stdarg.h>

#include "gds_globals.h"

int verbose_output;

void v_printf(const int level, const char *fmt, ...)
{
	if(verbose_output>=level){
		va_list va;
		va_start(va, fmt);
		vprintf(fmt, va);
		va_end(va);
	}
}

