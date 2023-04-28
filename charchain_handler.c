#include "main.h"

/**
 * _chaintest - test if current char in buffer is a chain delimeter
 * @info: the parameter struct
 * @buffer: the char buffer
 * @ptr: address of current position in buf
 *
 * Return: 1 if chain delimeter, 0 otherwise
 */
int _chaintest(info_t *info, char *buffer, size_t *ptr)
{
	size_t i = *ptr;

	if (buffer[i] == '|' && buffer[i + 1] == '|')
	{
		buffer[i] = 0;
		i++;
		info->command_buffer_type = COMMAND_OR;
	}
	else if (buffer[i] == '&' && buffer[i + 1] == '&')
	{
		buffer[i] = 0;
		i++;
		info->command_buffer_type = COMMAND_AND;
	}
	else if (buffer[i] == ';') /* found end of this command */
	{
		buffer[i] = 0; /* replace semicolon with null */
		info->command_buffer_type = COMMAND_CHAIN;
	}
	else
		return (0);
	*ptr = i;
	return (1);
}

/**
 * check_chain - checks we should continue chaining based on last status
 * @info: the parameter struct
 * @buffer: the char buffer
 * @ptr: address of current position in buf
 * @i: starting position in buf
 * @len: length of buf
 *
 * Return: Void
 */
void check_chain(info_t *info, char *buffer, size_t *ptr, size_t i, size_t len)
{
	size_t j = *ptr;

	if (info->command_buffer_type == COMMAND_AND)
	{
		if (info->status)
		{
			buffer[i] = 0;
			j = len;
		}
	}
	if (info->command_buffer_type == COMMAND_OR)
	{
		if (!info->status)
		{
			buffer[i] = 0;
			j = len;
		}
	}

	*ptr = j;
}

/**
 * resolve_alias - replaces an aliases in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int resolve_alias(info_t *info)
{
	int i;
	list_t *node;
	char *ptr;

	for (i = 0; i < 10; i++)
	{
		node = _prefixnode(info->alias, info->argv[0], '=');
		if (!node)
			return (0);
		free(info->argv[0]);
		ptr = _strchr(node->str, '=');
		if (!ptr)
			return (0);
		ptr = _strdup(ptr + 1);
		if (!ptr)
			return (0);
		info->argv[0] = ptr;
	}
	return (1);
}

/**
 * replace_vars - replaces vars in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_vars(info_t *info)
{
	int i = 0;
	list_t *node;

	for (i = 0; info->argv[i]; i++)
	{
		if (info->argv[i][0] != '$' || !info->argv[i][1])
			continue;

		if (!_strcmp(info->argv[i], "$?"))
		{
			resolve_string(&(info->argv[i]),
				_strdup(convert_number(info->status, 10, 0)));
			continue;
		}
		if (!_strcmp(info->argv[i], "$$"))
		{
			resolve_string(&(info->argv[i]),
				_strdup(convert_number(getpid(), 10, 0)));
			continue;
		}
		node = _prefixnode(info->env, &info->argv[i][1], '=');
		if (node)
		{
			resolve_string(&(info->argv[i]),
				_strdup(_strchr(node->str, '=') + 1));
			continue;
		}
		resolve_string(&info->argv[i], _strdup(""));

	}
	return (0);
}

/**
 * resolve_string - replaces string
 * @prev: address of old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int resolve_string(char **prev, char *new)
{
	free(*prev);
	*prev = new;
	return (1);
}
