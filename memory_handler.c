#include "main.h"
/**
 * _memset - Fills memory with a constant byte
 * @s: buffer array
 * @b: constant byte
 * @n: number of bytes of memory area to fill
 * Return: Pointer to memory area `s`
 */
char *_memset(char *s, char b, unsigned int n)
{
	int i;

	i = 0;
	while (n > 0)
	{
		s[i] = b;
		i++;
		n--;
	}

	return (s);
}
/**
 * free_strarr - frees a string of strings
 * @arr: string of strings
 */
void free_strarr(char **arr)
{
	char **s = arr;

	if (!arr)
		return;
	while (*arr)
		free(*arr++);
	free(s);
}

/**
 * realloc_mem - reallocates a block of memory
 * @ptr: pointer to previous malloc'ated block
 * @prev: byte size of previous block
 * @new: byte size of new block
 *
 * Return: pointer to str.
 */
void *realloc_mem(void *ptr, unsigned int prev, unsigned int new)
{
	char *str;

	if (!ptr)
		return (malloc(new));
	if (!new)
		return (free(ptr), NULL);
	if (new == prev)
		return (ptr);

	str = malloc(new);
	if (!str)
		return (NULL);

	prev = prev < new ? prev : new;
	while (prev--)
		str[prev] = ((char *)ptr)[prev];
	free(ptr);
	return (str);
}

/**
 * safe_free - frees a pointer and NULLs the address
 * @ptr: address of the pointer to free
 *
 * Return: 1 on success.
 */
int safe_free(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
		return (1);
	}
	return (0);
}
