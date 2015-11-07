/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcosnefr <fcosnefr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/08 14:37:27 by fcosnefr          #+#    #+#             */
/*   Updated: 2015/10/08 14:37:32 by fcosnefr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALLOC_H
# define FT_MALLOC_H

#include <stdio.h>

#include <stddef.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <strings.h>

#define FLAGS					PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE

#define ROUND(value, round)		((value + (round - 1)) & ~(round - 1))

#define ROUND4(value)			ROUND(value, 4)

#define ROUND_PAGE(value)		ROUND(value, getpagesize())

#define PAGE_META				(sizeof(size_t) + sizeof(char *) + sizeof(size_t))

#define DATA_META				(sizeof(size_t))

#define PAGE_SIZE(page)			((size_t *)(page))

#define PAGE_NEXT(page)			((char **)(page + sizeof(size_t)))

#define PAGE_QUICK(page)		((size_t *)(page + sizeof(size_t) + sizeof(char *)))

#define DATA_SIZE(page, index)	((size_t *)(page + (index)))


#define TINY					ROUND4(1024)

#define SMALL					ROUND4(1024 * 32)

#define GET_PAGE_SIZE(elem_size)	ROUND_PAGE(((DATA_META + elem_size) * 100) + PAGE_META)

char	*get_page_list(size_t size, char *modify);
char	*get_new_page(size_t size);

void	*malloc(size_t size);

void	free(void *ptr);

void	*realloc(void *ptr, size_t size);

void	show_alloc_mem(void);

char	memory_is_set(char *mem, size_t size);
size_t 	get_alloc_size(size_t size);

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
