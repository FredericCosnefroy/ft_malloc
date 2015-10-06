#ifndef FT_MALLOC_H
# define FT_MALLOC_H

#include <stddef.h>
#include <unistd.h>
#include <sys/mman.h>
#include <strings.h>

#define FLAGS					PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE

#define ROUND(value, round)		((value + (round - 1)) & ~(round - 1))

#define ROUND8(value)			ROUND(value, 8)

#define ROUND_PAGE(value)		ROUND(value, getpagesize())

#define PAGE_META				(sizeof(size_t) + sizeof(char *) + sizeof(size_t))

#define DATA_META				(sizeof(size_t))

#define PAGE_SIZE(page)			((size_t *)(page))

#define PAGE_NEXT(page)			((char **)(page + sizeof(size_t)))

#define PAGE_QUICK(page)		((size_t *)(page + sizeof(size_t) + sizeof(char *)))

#define DATA_SIZE(page, index)	((size_t *)(page + (index)))


#define TINY					1024

#define SMALL					2048

#define GET_PAGE_SIZE(elem_size)	ROUND_PAGE(((DATA_META + elem_size) * 100) + PAGE_META)

void	*ft_malloc(size_t size);

void	ft_free(void *ptr);

void	*ft_realloc(void *ptr, size_t size);

/*\
 *
 *	PAGE ANATOMY
 *
 *
 *  | S | S | S | S | S | S | S | S | N | N | N | N | N | N | N | N | Q | Q | Q | Q | Q | Q | Q | Q | 0 | 0 | 0 | 0 | ...
 *																									^
 *																							   data starts
 *
 *  S : Page size 					(size_t)
 *	N : Next page 					(char *)
 *	Q : Most recent modified index 	(size_t)
 *	0 : Free bits
 *
 *
 *	DATA ANATOMY
 *
 *	| s | s | s | s | s | s | s | s | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | ...
 *									^
 *						  malloc return address
 *
 *	s : Data size 					(size_t)
 *	0 : Free bits
 *
\*/

#endif