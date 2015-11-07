/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcosnefr <fcosnefr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/08 14:38:26 by fcosnefr          #+#    #+#             */
/*   Updated: 2015/10/08 14:38:28 by fcosnefr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

char	memory_is_set(char *mem, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		if (mem[i] != 0)
			return (1);
		i++;
	}
	return (0);
}

size_t	get_alloc_size(size_t size)
{
	if (size <= TINY)
		return (GET_PAGE_SIZE(TINY));
	if (size <= SMALL)
		return (GET_PAGE_SIZE(SMALL));
	return (ROUND_PAGE(size + PAGE_META + DATA_META));
}
