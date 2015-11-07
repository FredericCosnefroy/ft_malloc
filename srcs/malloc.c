/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcosnefr <fcosnefr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/08 14:42:18 by fcosnefr          #+#    #+#             */
/*   Updated: 2015/10/08 14:42:31 by fcosnefr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

static char	*book_large(char *mem, size_t size)
{
	if (*DATA_SIZE(mem, PAGE_META) == 0)
	{
		*DATA_SIZE(mem, PAGE_META) = size;
		return (mem + PAGE_META + DATA_META);
	}
	return (NULL);
}

static char	*book_memory(char *mem, size_t size)
{
	size_t	i;
	size_t	page_size;

	i = *PAGE_QUICK(mem);
	page_size = *PAGE_SIZE(mem);
	if (ROUND_PAGE(size + PAGE_META + DATA_META) == page_size)
		return (book_large(mem, size));
	while (i < page_size - PAGE_META - size)
	{
		if (!memory_is_set(mem + i, DATA_META + size))
		{
			*DATA_SIZE(mem, i) = size;
			*PAGE_QUICK(mem) = i + DATA_META + size;
			return (mem + i + DATA_META);
		}
		i += *DATA_SIZE(mem, i) + DATA_META;
	}
	return (NULL);
}

static char	*find_memory(size_t size)
{
	char	*mem;
	char	**next;
	char	*res;

	if (!(mem = get_page_list(size, (void *)-1)))
		return (NULL);
	while (mem)
	{
		if (*PAGE_SIZE(mem) == get_alloc_size(size))
		{
			if ((res = book_memory(mem, size)))
				return (res);
		}
		next = PAGE_NEXT(mem);
		if (*next == NULL)
			*next = get_new_page(size);
		mem = *next;
	}
	return (NULL);
}

void		*malloc(size_t size)
{
	char	*mem;

	if (size == 0)
		return (NULL);
	mem = find_memory(ROUND4(size));
	return (mem);
}
