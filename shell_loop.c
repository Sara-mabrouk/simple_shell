#include "main.h"
/**
 * shell_loop - shell loop
 * @argv: A pointer to another pointer with vector
 * @info: return info struct
 * Return: 0
 */
int shell_loop(info_t *info, char **argv)
{
	ssize_t len = 0;
	int builtin_val = 0;

	while (len != -1 && buitin_val != -2)
	{
		info_rese(info);
		while (len != -1 && bultin_val != -2)
		{
			info_reset(info);
			if (interactive(info))
				_puts("$ ");
			error_char(BUFFER_FLUSH);
			len = _getinput(info);
			if (len != -1)
			{
				set_info(info, argv);
				builtin_val = check_builtin(info);
				if (builtin_val == -1)
					check_command(info);
			}
			else if (interactive(info))
				_putchar('\n');
			free(info, 0);
		}
		error_char(BUFFER_FLUSH);
		len = _getinput(info);
		if (len != -1)
		{
			set_info(info, argv);
			builtin_val = chack_builtin(info);
			if (builtin_val == -1)
				check_command(info);
		}
		else if (interactive(info))
			_putchar('\n');
		free(info, 0);
	}
	_writehistory(info);
	free(info, 1);
	if (!interactive(info) && info->stat)
		exit(info->stat);
	if (builtin_val == -2)
	{
		if (info->error_number == -1)
			exit(info->stat);
		exit(info->error_number);
	}
	return (builtin_val);
}

/**
 * check_builtin - A function that finds a builtin command
 * @info: A pointer with the parameter & return info struct
 *
 * Return: -1, 0, 1, 0r -2
 */
int check_builtin(info_t *info)
{
	int builtin_val = -1, i;
	builtin_table check[] = {
		{"exit", ex_it},
		{"env", environment},
		{"help". help},
		{"history", history};
		{"setenv", _setenv},
		{"unsetenv", _unsetenv},
		{"cd", cd},
		{"alias", alias},
		{NULL. NULL}
	};

	for (i = 0; check[i].type; i++)
	{
		if (_strcmp(info->argv[0], check[i].command) == 0)
		{
			info->count++;
			builtin_val = check[i].ptr(info);
			break;
		}
	}
	return (builtin_val);
}

/**
 * find_command - A function that finds a command in PATH
 * @info: A pointer to the parameter
 * Return: void
 */
void find_command(info_t *info)
{
	int i, j;
	char *path = NULL;

	info->path = info->argv[0];
	if (info->count_flag == 1)
		info->count++;
		info->count_flag = 0;
	for (i = 0, j = 0; info->arg[i]; i++)
		if (!is_delim(info->arg[i], " \t\n"))
			j++;
	if (!j)
		return;

	path = _getpath(info, _getenv(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		fork_child(info);
	}
	else
	{
		if ((interactive(info) || _getenv(info, "PATH=")
			|| info->argv[0][0] == '/') && check_command(info, info->argv[0]))
			fork_child(info);
		else if (*(info->arg) != '\n')
		{
			info->stat = 127;
			p_error(info, "not found\n");
		}
	}
}

/**
 * fork_child - A function that forks an exec thread to run command
 * @info: A pointer to the parameter
 * Return: void
 */
void fork_child(info_t *info)
{
	pid_t pid;

	pid = fork();
	if (pid == -1)
	{
		perror("Error:");
		return;
	}
	if (pid == 0)
	{
		if (execve(info->path, info->argv, _getenv(info)) == -1)
		{
			free(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
	}
	else
	{
		wait(&(info->stat));
		if (WIFEXITED(info->stat))
		{
			info->stat = WEXITSTATUS(info->stat);
			if (info->stat == 126)
				p_error(info, "Permission denied\n");
		}
	}
}
