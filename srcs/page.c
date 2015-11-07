/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   page.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcosnefr <fcosnefr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/08 14:37:59 by fcosnefr          #+#    #+#             */
/*   Updated: 2015/10/08 14:38:00 by fcosnefr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

static char	has_memory_available(size_t size)
{
	char			*mem;
	size_t			total;
	struct rlimit	rlp;

	total = 0;
	getrlimit(RLIMIT_AS, &rlp);
	mem = get_page_list(0, (void *)-1);
	while (mem)
	{
		total += *PAGE_SIZE(mem);
		mem = *PAGE_NEXT(mem);
	}
	if (total + get_alloc_size(size) <= rlp.rlim_cur)
		return (1);
	return (0);
}

char		*get_new_page(size_t size)
{
	char	*mem;
	size_t	alloc_size;

	mem = NULL;
	if (!has_memory_available(size))
		return (NULL);
	alloc_size = get_alloc_size(size);
	if ((mem = (char *)mmap(0, alloc_size, FLAGS, -1, 0)) == ((void *)-1))
		return (NULL);
	if (alloc_size != ROUND_PAGE(size + PAGE_META + DATA_META))
		bzero(mem, alloc_size);
	else
		*DATA_SIZE(mem, PAGE_META) = 0;
	*PAGE_SIZE(mem) = alloc_size;
	*PAGE_NEXT(mem) = NULL;
	*PAGE_QUICK(mem) = PAGE_META;
	return (mem);
}

char		*get_page_list(size_t size, char *modify)
{
	static char *mem = NULL;

	if (mem == NULL && size != 0)
		mem = get_new_page(size);
	else if ((void *)modify != (void *)-1)
		mem = modify;
	return (mem);
}
