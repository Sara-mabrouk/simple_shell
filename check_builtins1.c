#include "main.h"

/**
 * ex_it - exits the shell
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: exits with a given exit status
 *         (0) if info.argv[0] != "exit"
 */
int ex_it(info_t *info)
{
	int check;

	if (info->argv[1])  /* If there is an exit arguement */
	{
		check = str_to_int(info->argv[1]);
		if (check == -1)
		{
			info->status = 2;
			p_error(info, "Illegal number: ");
			error_str(info->argv[1]);
			error_char('\n');
			return (1);
		}
		info->error_number = str_to_int(info->argv[1]);
		return (-2);
	}
	info->error_number = -1;
	return (-2);
}

/**
 * _cd - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _cd(info_t *info)
{
	char *str, *directory, buffer[1024];
	int result;

	str = getcwd(buffer, 1024);
	if (!str)
		_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!info->argv[1])
	{
		directory = _getenv(info, "HOME=");
		if (!directory)
			result =
				chdir((directory = _getenv(info, "PWD=")) ? directory : "/");
		else
			result = chdir(directory);
	}
	else if (_strcmp(info->argv[1], "-") == 0)
	{
		if (!_getenv(info, "OLDPWD="))
		{
			_puts(str);
			_putchar('\n');
			return (1);
		}
		_puts(_getenv(info, "OLDPWD=")), _putchar('\n');
		chdir((directory = _getenv(info, "OLDPWD=")) ? directory : "/");
	}
	else
		result = chdir(info->argv[1]);
	if (result == -1)
	{
		p_error(info, "can't cd to ");
		error_str(info->argv[1]), error_char('\n');
	}
	else
	{
		_setenv(info, "OLDPWD", _getenv(info, "PWD="));
		_setenv(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * _help - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _help(info_t *info)
{
	char **arr;

	arr = info->argv;
	_puts("help call works. Function not yet implemented \n");
	if (0)
		_puts(*arr); /* temp att_unused workaround */
	return (0);
}
