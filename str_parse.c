#include "main.h"

/**
 * check_command - determines if a file is an executable command
 * @info: the info struct
 * @path: path to the file
 *
 * Return: 1 if true, 0 otherwise
 */
int check_command(info_t *info, char *path)
{
	struct stat status;

	(void)info;
	if (!path || stat(path, &status))
		return (0);

	if (status.st_mode & S_IFREG)
	{
		return (1);
	}
	return (0);
}

/**
 * str_dup - duplicates characters
 * @pathstr: the PATH string
 * @start: starting index
 * @stop: stopping index
 *
 * Return: pointer to new buffer
 */
char *str_dup(char *pathstr, int start, int stop)
{
	static char buffer[1024];
	int i = 0, j = 0;

	for (j = 0, i = start; i < stop; i++)
		if (pathstr[i] != ':')
			buffer[j++] = pathstr[i];
	buffer[j] = 0;
	return (buffer);
}

/**
 * check_path - finds this cmd in the PATH string
 * @info: the info struct
 * @pathstr: the PATH string
 * @cmd: the cmd to find
 *
 * Return: full path of cmd if found or NULL
 */
char *check_path(info_t *info, char *pathstr, char *cmd)
{
	int i = 0, current = 0;
	char *path;

	if (!pathstr)
		return (NULL);
	if ((_strlen(cmd) > 2) && _strstr(cmd, "./"))
	{
		if (check_command(info, cmd))
			return (cmd);
	}
	while (1)
	{
		if (!pathstr[i] || pathstr[i] == ':')
		{
			path = str_dup(pathstr, current, i);
			if (!*path)
				_strcat(path, cmd);
			else
			{
				_strcat(path, "/");
				_strcat(path, cmd);
			}
			if (check_command(info, path))
				return (path);
			if (!pathstr[i])
				break;
			current = i;
		}
		i++;
	}
	return (NULL);
}
