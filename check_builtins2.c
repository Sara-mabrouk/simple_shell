#include "main.h"

/**
 * _history - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _history(info_t *info)
{
	str_list(info->history);
	return (0);
}

/**
 * remove_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int remove_alias(info_t *info, char *str)
{
	char *s, c;
	int result;

	s = _strchr(str, '=');
	if (!s)
		return (1);
	c = *s;
	*s = 0;
	result = _deletenode(&(info->alias), _nodeindex(info->alias,
		_prefixnode(info->alias, str, -1)));
	*s = c;
	return (result);
}

/**
 * add_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int add_alias(info_t *info, char *str)
{
	char *s;

	s = _strchr(str, '=');
	if (!s)
		return (1);
	if (!*++s)
		return (remove_alias(info, str));

	remove_alias(info, str);
	return (_endnode(&(info->alias), str, 0) == NULL);
}

/**
 * print_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int print_alias(list_t *node)
{
	char *s = NULL, *a = NULL;

	if (node)
	{
		s = _strchr(node->str, '=');
		for (a = node->str; a <= s; a++)
			_putchar(*a);
		_putchar('\'');
		_puts(s + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * _alias - mimics the alias builtin (man alias)
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _alias(info_t *info)
{
	int i = 0;
	char *s = NULL;
	list_t *node = NULL;

	if (info->argc == 1)
	{
		node = info->alias;
		while (node)
		{
			print_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; info->argv[i]; i++)
	{
		s = _strchr(info->argv[i], '=');
		if (s)
			add_alias(info, info->argv[i]);
		else
			print_alias(_prefixnode(info->alias, info->argv[i], '='));
	}

	return (0);
}
