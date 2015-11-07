/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcosnefr <fcosnefr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/08 14:37:40 by fcosnefr          #+#    #+#             */
/*   Updated: 2015/10/08 14:37:44 by fcosnefr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

static char has_one_page_available()
{
	char 	*mem;
	char	av;

	av = 0;
	mem = get_page_list(0, (void *)-1);
	while (mem && av != 3)
	{
		if (*PAGE_SIZE(mem) == get_alloc_size(TINY))
			av |= 1;
		else if (*PAGE_SIZE(mem) == get_alloc_size(SMALL))
			av |= 2;
		mem = *PAGE_NEXT(mem);
	}
	return (av == 3);
}

static char	unmap_page(char *previous, char *mem, char *next)
{
	if (!has_one_page_available())
		return (0);
	if (previous)
		*PAGE_NEXT(previous) = next;
	else
		get_page_list(0, next);
	munmap((void *)mem, *PAGE_SIZE(mem));
	return (0);
}

static void	delete_memory(char *ptr, char *mem)
{
	size_t	*quickest;
	size_t	size;

	if ((size = *DATA_SIZE(ptr, 0)) == 0)
		return ;
	quickest = PAGE_QUICK(mem);
	if ((size_t)(ptr - mem) < *quickest)
		*quickest = (ptr - mem);
	bzero(ptr, DATA_META + size);
}

static char	evaluate_freeable(char *ptr, char *previous, char *mem, char *next)
{
	if (*PAGE_SIZE(mem) == ROUND_PAGE(*DATA_SIZE(ptr, 0) +
		PAGE_META + DATA_META))
		return (unmap_page(previous, mem, next));
	delete_memory(ptr, mem);
	if (!memory_is_set(mem + PAGE_META, *PAGE_SIZE(mem) - PAGE_META))
		return (unmap_page(previous, mem, next));
	return (0);
}

static char	delete_ptrs_page(char *ptr)
{
	char	*mem;
	char	*previous;
	char	*next;

	previous = NULL;
	mem = get_page_list(0, (void *)-1);
	while (mem)
	{
		next = *PAGE_NEXT(mem);
		if (ptr >= mem + PAGE_META && ptr < mem + *PAGE_SIZE(mem))
			return (evaluate_freeable(ptr, previous, mem, next));
		previous = mem;
		mem = next;
	}
	return (0);
}

void		free(void *ptr)
{
	if (!ptr || (size_t)ptr % 4 != 0)
		return ;
	delete_ptrs_page((char *)(ptr - DATA_META));
}
