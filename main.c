#include "ft_malloc.h"

#include <stdio.h>

int main()
{
	char	*str1;
	char	*str2;
	char	*str3;
	char	*str4;
	char 	*str5;
	char	*str6;
	
	str1 = ft_malloc(512);
	str2 = ft_malloc(512);
	str3 = ft_malloc(512);
	str4 = ft_malloc(512);
	str5 = ft_malloc(512);

	show_alloc_mem();
	getchar();

	str2 = ft_realloc(str2, 256);

	show_alloc_mem();
	getchar();

	str6 = ft_malloc(248);


	show_alloc_mem();
	getchar();

	memcpy(str6, "J'aime la bite", 14);
	str6 = ft_realloc(str6, 2048);

	show_alloc_mem();
	printf("\n%s\n", str6);
	getchar();

	str5 = ft_realloc(str5, 1024);
	printf("%p\n", str5);
	show_alloc_mem();
	getchar();

	ft_malloc(512);
		show_alloc_mem();
	getchar();
	ft_free(str2);
		show_alloc_mem();
	getchar();
	ft_malloc(128);
		show_alloc_mem();
	getchar();
	//ft_free(str4);
	//ft_free(str1);
	//show_alloc_mem();
	//getchar();
	//ft_free(str5);
	//show_alloc_mem();
	return (0);
}