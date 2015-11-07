#include "includes/malloc.h"

int main()
{
	char *addr;

	write(1, "start\n", 7);
	addr = malloc(100);
	write(1, "end\n", 5);
	show_alloc_mem();
	return (0);
}