#include "main.h"

/**
 * _gethistory - gets the history file
 * @info: parameter struct
 *
 * Return: allocated string containg history file
 */

char *_gethistory(info_t *info)
{
	char *buffer, *directory;

	directory = _getenv(info, "HOME=");
	if (!directory)
		return (NULL);
	buffer = malloc(sizeof(char) * (_strlen(directory) +
			_strlen(HISTORY_FILE) + 2));
	if (!buffer)
		return (NULL);
	buffer[0] = 0;
	_strcpy(buffer, directory);
	_strcat(buffer, "/");
	_strcat(buffer, HISTORY_FILE);
	return (buffer);
}

/**
 * _writehistory - creates a file, or appends to an existing file
 * @info: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int _writehistory(info_t *info)
{
	ssize_t file_descriptor;
	char *filename = _gethistory(info);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	file_descriptor = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (file_descriptor == -1)
		return (-1);
	for (node = info->history; node; node = node->next)
	{
		putstr_fd(node->str, file_descriptor);
		putchar_fd('\n', file_descriptor);
	}
	putchar_fd(BUFFER_FLUSH, file_descriptor);
	close(file_descriptor);
	return (1);
}

/**
 * _readhistory - reads history from file
 * @info: the parameter struct
 *
 * Return: count on success, 0 otherwise
 */
int _readhistory(info_t *info)
{
	int i, last = 0, count = 0;
	ssize_t file_descriptor, read_length, f_size = 0;
	struct stat status;
	char *buffer = NULL, *filename = _gethistory(info);

	if (!filename)
		return (0);

	file_descriptor = open(filename, O_RDONLY);
	free(filename);
	if (file_descriptor == -1)
		return (0);
	if (!fstat(file_descriptor, &status))
		f_size = status.st_size;
	if (f_size < 2)
		return (0);
	buffer = malloc(sizeof(char) * (f_size + 1));
	if (!buffer)
		return (0);
	read_length = read(file_descriptor, buffer, f_size);
	buffer[f_size] = 0;
	if (read_length <= 0)
		return (free(buffer), 0);
	close(file_descriptor);
	for (i = 0; i < f_size; i++)
		if (buffer[i] == '\n')
		{
			buffer[i] = 0;
			_buildhistory(info, buffer + last, count++);
			last = i + 1;
		}
	if (last != i)
		_buildhistory(info, buffer + last, count++);
	free(buffer);
	info->history_count = count;
	while (info->history_count-- >= HISTORY_MAX)
		_deletenode(&(info->history), 0);
	_renumberhistory(info);
	return (info->history_count);
}

/**
 * _buildhistory - adds entry to a history linked list
 * @info: Structure containing potential arguments. Used to maintain
 * @buffer: buffer
 * @linecount: the history linecount, histcount
 *
 * Return: Always 0
 */
int _buildhistory(info_t *info, char *buffer, int count)
{
	list_t *node = NULL;

	if (info->history)
		node = info->history;
	_endnode(&node, buffer, count);

	if (!info->history)
		info->history = node;
	return (0);
}

/**
 * _renumberhistory - renumbers the history linked list after changes
 * @info: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histcount
 */
int _renumberhistory(info_t *info)
{
	list_t *node = info->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (info->history_count = i);
}
