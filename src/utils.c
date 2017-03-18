#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

void throw_error(char *a)
{
	fprint(stderr,"%s\n",a);
	exit(-1);
}