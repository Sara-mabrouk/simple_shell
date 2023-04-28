#include "main.h"

/**
 *error_str - prints an input string
 * @str: the string to be printed
 */
void error_str(char *str)
{
	int i = 0;

	if (!str)
		return;
	while (str[i] != '\0')
	{
		error_char(str[i]);
		i++;
	}
}

/**
 * error_char - writes the character c to stderr
 * @c: The character to print
 *
 * Return: 1 on success.
 */
int error_char(char c)
{
	static int i;
	static char buffer[WRITE_BUFFER];

	if (c == BUFFER_FLUSH || i >= WRITE_BUFFER)
	{
		write(2, buffer, i);
		i = 0;
	}
	if (c != BUFFER_FLUSH)
		buffer[i++] = c;
	return (1);
}

/**
 * putchar_fd - writes the character c to given file descriptor
 * @c: The character to print
 * @file_descriptor: The filedescriptor to write to
 *
 * Return: On success 1.
 */
int putchar_fd(char c, int file_descriptor)
{
	static int i;
	static char buffer[WRITE_BUFFER];

	if (c == BUFFER_FLUSH || i >= WRITE_BUFFER)
	{
		write(file_descriptor, buffer, i);
		i = 0;
	}
	if (c != BUFFER_FLUSH)
		buffer[i++] = c;
	return (1);
}

/**
 * putstr_fd - prints an input string
 * @str: the string to be printed
 * @file_descriptor: the filedescriptor to write to
 *
 * Return: the number of chars put
 */
int putstr_fd(char *str, int file_descriptor)
{
	int i = 0;

	if (!str)
		return (0);
	while (*str)
	{
		i += putchar_fd(*str++, file_descriptor);
	}
	return (i);
}
