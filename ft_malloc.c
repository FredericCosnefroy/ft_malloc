
#include "ft_malloc.h"

char	memory_is_set(char *mem, int size)
{
	size_t 	i;

	i = 0;
	while (i < size)
	{
		if (*((size_t *)(mem + i)) != 0)
			return (1);
		i += 8;
	}
	return (0);
}

size_t 	get_alloc_size(size_t size)
{
	if (size <= TINY)
		return (GET_PAGE_SIZE(TINY));
	if (size <= SMALL)
		return (GET_PAGE_SIZE(SMALL));
	return (ROUND_PAGE(size));
}

char	*get_new_page(size_t size)
{
	char 	*mem;
	size_t 	alloc_size;

	alloc_size = get_alloc_size(size);
	mem = (char *)mmap(0, alloc_size, FLAGS, -1, 0);
	bzero(mem, alloc_size);
	*PAGE_SIZE(mem) = alloc_size;
	*PAGE_NEXT(mem) = NULL;
	*PAGE_QUICK(mem) = PAGE_META;
	return (mem);
}

char	*get_page_list(size_t size, char *modify)
{
	static char *mem = NULL;

	if (mem == NULL && size != 0)
		mem = get_new_page(size);
	else if ((void *)modify != (void *)-1)
		mem = modify;
	return (mem);
}

char				*book_memory(char *mem, size_t size)
{
	size_t			i;
	size_t 			page_size;

	i = *PAGE_QUICK(mem);
	page_size = *PAGE_SIZE(mem);
	while (i < page_size - size)
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

char	*find_memory(size_t size)
{
	char 	*mem;
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

void	*ft_malloc(size_t size)
{
	char 	*mem;

	if (size == 0)
		return (NULL);
	mem = find_memory(ROUND8(size));
	return (mem);
}

void	unmap_page(char *previous, char *mem, char *next)
{
	if (previous)
		*PAGE_NEXT(previous) = next;
	else
		get_page_list(0, next);
	munmap((void *)mem, *PAGE_SIZE(mem));
}

void	delete_memory(char *ptr, char *mem)
{
	size_t 	*quickest;
	size_t 	size;

	if ((size = *DATA_SIZE(ptr, 0)) == 0)
		return ;
	quickest = PAGE_QUICK(mem);
	if (ptr - mem < *quickest)
		*quickest = (ptr - mem);
	bzero(ptr, DATA_META + size);
}

void	delete_ptrs_page(char *ptr)
{
	size_t 	page_size;
	char 	*mem;
	char	*previous;
	char 	*next;

	previous = NULL;
	mem = get_page_list(0, (void *)-1);
	while (mem)
	{
		page_size = *PAGE_SIZE(mem);
		next = *PAGE_NEXT(mem);
		if (ptr >= mem + PAGE_META && ptr < mem + page_size)
		{
			delete_memory(ptr, mem);
			if (!memory_is_set(mem + PAGE_META, page_size - PAGE_META))
				unmap_page(previous, mem, next);
			return ;
		}
		previous = mem;
		mem = next;
	}
}

void	ft_free(void *ptr)
{
	char	*mem;

	if (!ptr)
		return ;
	delete_ptrs_page((char *)(ptr - DATA_META));
}

char	*alloc_size_to_string(size_t size)
{
	if (size == GET_PAGE_SIZE(TINY))
		return ("TINY");
	if (size == GET_PAGE_SIZE(SMALL))
		return ("SMALL");
	return ("LARGE");
}

size_t	print_page_content(char *mem, size_t page_size)
{
	size_t			i;
	size_t 			size;
	size_t 			total;

	total = 0;
	i = PAGE_META;
	while (mem + i < mem + page_size)
	{
		size = *DATA_SIZE(mem, i);
		if (size > 0)
			printf("%p - %p : %lu octets\n", mem + i + DATA_META, mem + i + DATA_META + size - 1, size >> 3);
		total += size;
		i += *DATA_SIZE(mem, i) + DATA_META;
	}
	return (total);
}

size_t	print_page(char *mem)
{
	size_t 	page_size;

	page_size = *PAGE_SIZE(mem);
	printf("%s : %p\n", alloc_size_to_string(page_size), mem);
	return (print_page_content(mem, page_size));
}

void	show_alloc_mem( void )
{
	size_t 	total;
	char 	*mem;

	total = 0;
	mem = get_page_list(0, (void *)-1);
	while (mem)
	{
		total += print_page(mem);
		mem = *PAGE_NEXT(mem);
	}	
	printf("Total : %lu octets\n", total >> 3);
}


char	*find_ptrs_page(char *ptr)
{
	char 	*mem;

	mem = get_page_list(0, (void *)-1);
	while (mem)
	{
		if (ptr >= mem + PAGE_META && ptr < mem + *PAGE_SIZE(mem))
			return (mem);
		mem = *PAGE_NEXT(mem);
	}
	return (NULL);
}

char	*copy_ptr_content(char *ptr, size_t size)
{
	char *res;

	if (!(res = ft_malloc(size)))
		return (NULL);
	memcpy(res, ptr, *DATA_SIZE(ptr , -DATA_META));
	ft_free(ptr);
	return (res);
}

char	*extend_memory(char *ptr, char *mem, size_t size)
{
	size_t 	page_size;
	size_t 	*data_size;

	page_size = *PAGE_SIZE(mem);
	data_size = DATA_SIZE(ptr, -DATA_META);
	if (size > *data_size)
	{
		if ((ptr - mem) + (size - *data_size) < page_size &&
			!memory_is_set(ptr + *data_size, (size - *data_size)))
		{
			*data_size = size;
			*PAGE_QUICK(mem) = (ptr - mem + size);
			return (ptr);
		}
	}
	else
	{
		bzero(ptr + *data_size - (*data_size - size), (*data_size - size));
		*data_size = size;
		*PAGE_QUICK(mem) = (ptr - mem + size);
		return (ptr);
	}
	return (copy_ptr_content(ptr, size));
}

void	*ft_realloc(void *ptr, size_t size)
{
	char 	*mem;

	if (!ptr)
		return (ft_malloc(size));
	if (ptr && size == 0)
	{
		ft_free(ptr);
		return (ft_malloc(1));
	}
	if (!(mem = find_ptrs_page(ptr - DATA_META)))
		return (NULL);
	if (ROUND8(size) == *DATA_SIZE(ptr, -DATA_META))
		return (ptr);
	return (extend_memory(ptr, mem, ROUND8(size)));
}
