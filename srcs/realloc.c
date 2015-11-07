/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcosnefr <fcosnefr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/08 14:37:50 by fcosnefr          #+#    #+#             */
/*   Updated: 2015/10/08 14:37:52 by fcosnefr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

static char	*find_ptrs_page(char *ptr)
{
	char	*mem;

	mem = get_page_list(0, (void *)-1);
	while (mem)
	{
		if (ptr >= mem + PAGE_META && ptr < mem + *PAGE_SIZE(mem))
			return (mem);
		mem = *PAGE_NEXT(mem);
	}
	return (NULL);
}

static char	*copy_ptr_content(char *ptr, size_t size)
{
	char *res;

	if (!(res = malloc(size)))
		return (NULL);
	memcpy(res, ptr, *DATA_SIZE(ptr, -DATA_META));
	free(ptr);
	return (res);
}

static char	*extend_memory(char *ptr, char *mem, size_t size)
{
	size_t	page_size;
	size_t	*data_size;

	page_size = *PAGE_SIZE(mem);
	data_size = DATA_SIZE(ptr, -DATA_META);
	if (size > *data_size)
	{
		if ((ptr - mem) + (size - *data_size) < page_size &&
			!memory_is_set(ptr + *data_size, (size - *data_size)))
		{
			*data_size = size;
			if (*PAGE_QUICK(mem) >= (size_t)((ptr - mem)) &&
				*PAGE_QUICK(mem) < (ptr - mem) + size)
				*PAGE_QUICK(mem) = (ptr - mem + size);
			return (ptr);
		}
	}
	else if (get_alloc_size(size) == page_size)
	{
		bzero(ptr + *data_size - (*data_size - size), (*data_size - size));
		*data_size = size;
		*PAGE_QUICK(mem) = (ptr - mem + size);
		return (ptr);
	}
	return (copy_ptr_content(ptr, size));
}

void		*realloc(void *ptr, size_t size)
{
	char	*mem;

	if (!ptr)
		return (malloc(size));
	if ((size_t)ptr % 4 != 0)
		return (NULL); 
	if (ptr && size == 0)
	{
		free(ptr);
		return (malloc(1));
	}
	if (!(mem = find_ptrs_page(ptr - DATA_META)))
		return (NULL);
	if (ROUND4(size) == *DATA_SIZE(ptr, -DATA_META))
		return (ptr);
	return (extend_memory(ptr, mem, ROUND4(size)));
}
