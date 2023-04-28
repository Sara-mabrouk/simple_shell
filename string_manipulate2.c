#include "main.h"
/**
 **_strcpy - copies the string pointed to by src
 *@dest: buffer
 *@src: string
 *Description: copies the string pointed to by src
 *Return: pointer to dest
 */
char *_strcpy(char *dest, char *src)
{
	int i;

	for (i = 0; src[i] != '\0'; i++)
	{
		dest[i] = src[i];
	}
	dest[i] = '\0';

	return (dest);
}

/**
 * _putchar - writes the character c to stdout
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _putchar(char c)
{
	return (write(1, &c, 1));
}

/**
 *_puts - prints a value of a variable
 *@str: pointer with the string value to be printed
 *Description: prints out the value of*str
 */
void _puts(char *str)
{
	int i;

	for (i = 0; str[i] != '\0'; i++)
	{
		_putchar(str[i]);
	}
	_putchar('\n');
}

/**
 *_strdup - A function that takes in the string as a parameter
 *@str: A pointer that points the memory address
 *that holds the string
 *Return: A pointer to a duplicated string or rturns NULL if
 *there was insufficient memory.
 */
char *_strdup(char *str)
{
	unsigned int a, i;
	char *p;

	if (str == NULL)
	{
		return (NULL);
	}
	a = 0;
	while (str[a] != '\0')
	{
		a++;
	}
	a++;
	p = malloc((a) * sizeof(char));
	if (p == NULL)
	{
		return (NULL);
	}
	i = 0;
	while (str[i] != '\0')
	{
		p[i] = str[i];
		i++;
	}
	p[i] = '\0';
	return (p);
}
