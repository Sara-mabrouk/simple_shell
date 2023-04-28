#include "main.h"

/**
 * input_buffer - buffers chained commands
 * @info: parameter struct
 * @buffer: address of buffer
 * @len: address of len var
 *
 * Return: bytes read
 */
ssize_t input_buffer(info_t *info, char **buffer, size_t *len)
{
	ssize_t bu_ff = 0;
	size_t buff_len = 0;

	if (!*len) /* if nothing left in the buffer, fill it */
	{
		/*bfree((void **)info->cmd_buf);*/
		free(*buffer);
		*buffer = NULL;
		signal(SIGINT, sigintHandler);
#if USE_GETLINE
		bu_ff = getline(buffer, &buff_len, stdin);
#else
		bu_ff = _getline(info, buffer, &buff_len);
#endif
		if (bu_ff > 0)
		{
			if ((*buffer)[bu_ff - 1] == '\n')
			{
				(*buffer)[bu_ff - 1] = '\0'; /* remove trailing newline */
				bu_ff--;
			}
			info->count_flag = 1;
			remove_comments(*buffer);
			_buildhistory(info, *buffer, info->history_count++);
			/* if (_strchr(*buf, ';')) is this a command chain? */
			{
				*len = bu_ff;
				info->command_buffer = buffer;
			}
		}
	}
	return (bu_ff);
}

/**
 * _getinput - gets a line minus the newline
 * @info: parameter struct
 *
 * Return: bytes read
 */
ssize_t _getinput(info_t *info)
{
	static char *buffer; /* the ';' command chain buffer */
	static size_t i, j, len;
	ssize_t bu_ff = 0;
	char **buffer_pointer = &(info->arg), *s;

	_putchar(BUFFER_FLUSH);
	bu_ff = input_buffer(info, &buffer, &len);
	if (bu_ff == -1) /* EOF */
		return (-1);
	if (len)	/* we have commands left in the chain buffer */
	{
		j = i; /* init new iterator to current buf position */
		s = buffer + i; /* get pointer for return */

		check_chain(info, buffer, &j, i, len);
		while (j < len)
		{
			if (test_chain(info, buffer, &j))
				break;
			j++;
		}

		i = j + 1;
		if (i >= len)
		{
			i = len = 0;
			info->command_buffer_type = COMMAND_NORM;
		}

		*buffer_pointer = s;
		return (_strlen(s));
	}

	*buffer_pointer = buffer;
	return (bu_ff);
}

/**
 * _readbuffer - reads a buffer
 * @info: parameter struct
 * @buffer: buffer
 * @len: size
 *
 * Return: bu_ff
 */
ssize_t _readbuffer(info_t *info, char *buffer, size_t *len)
{
	ssize_t bu_ff = 0;

	if (*len)
		return (0);
	bu_ff = read(info->read_file_descriptor, buffer, READ_BUFFER);
	if (bu_ff >= 0)
		*len = bu_ff;
	return (bu_ff);
}

/**
 * _getline - gets the next line of input from STDIN
 * @info: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: s
 */
int _getline(info_t *info, char **ptr, size_t *length)
{
	static char buffer[READ_BUFFER];
	static size_t i, len;
	size_t j;
	ssize_t bu_ff = 0, k = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		k = *length;
	if (i == len)
		i = len = 0;

	bu_ff = _readbuffer(info, buffer, &len);
	if (bu_ff == -1 || (bu_ff == 0 && len == 0))
		return (-1);

	c = _strchr(buffer + i, '\n');
	j = c ? 1 + (unsigned int)(c - buffer) : len;
	new_p = realloc_mem(p, k, k ? k + j : j + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (k)
		_strncat(new_p, buffer + i, j - i);
	else
		_strncpy(new_p, buffer + i, j - i + 1);

	k += j - i;
	i = j;
	p = new_p;

	if (length)
		*length = k;
	*ptr = p;
	return (k);
}

/**
 * sigintHandler - blocks ctrl-C
 * @signal_number: the signal number
 *
 * Return: void
 */
void sigintHandler(__attribute__((unused))int signal_number)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUFFER_FLUSH);
}
