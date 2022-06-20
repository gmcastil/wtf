#include "squares.h"

#include <stdio.h>
#include <stdint.h>

int main()
{
	uint32_t x = 42;
	uint32_t y;

	y = squares(x);
	printf("The square of %d is %d\n", x, y);
	return 0;
}

