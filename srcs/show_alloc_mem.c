/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcosnefr <fcosnefr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/08 14:38:17 by fcosnefr          #+#    #+#             */
/*   Updated: 2015/10/08 14:38:19 by fcosnefr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

static char		*alloc_size_to_string(size_t size)
{
	if (size == GET_PAGE_SIZE(TINY))
		return ("TINY");
	if (size == GET_PAGE_SIZE(SMALL))
		return ("SMALL");
	return ("LARGE");
}

static size_t	print_page_content(char *mem, size_t page_size)
{
	size_t	i;
	size_t	size;
	size_t	total;

	total = 0;
	i = PAGE_META;
	while (mem + i < mem + page_size - PAGE_META)
	{
		size = *DATA_SIZE(mem, i);
		if (size > 0)
			printf("%p - %p : %lu octets\n",
				mem + i + DATA_META, mem + i + DATA_META + size - 1, size);
		total += size;
		i += size + DATA_META;
	}
	return (total);
}

static size_t	print_page(char *mem)
{
	size_t	page_size;

	page_size = *PAGE_SIZE(mem);
	printf("%s : %p\n", alloc_size_to_string(page_size), mem);
	return (print_page_content(mem, page_size));
}

void			show_alloc_mem(void)
{
	size_t	total;
	char	*mem;

	total = 0;
	mem = get_page_list(0, (void *)-1);
	while (mem)
	{
		total += print_page(mem);
		mem = *PAGE_NEXT(mem);
	}
	printf("Total : %lu octets\n", total);
}
