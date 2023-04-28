#include "main.h"

/**
 * clr_info - initializes info_t struct
 * @info: struct address
 */
void clr_info(info_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * set_info - initializes info_t struct
 * @info: struct address
 * @arg_v: argument vector
 */
void set_info(info_t *info, char **arg_v)
{
	int i = 0;

	info->fname = arg_v[0];
	if (info->arg)
	{
		info->argv = tokenizer(info->arg, " \t");
		if (!info->argv)
		{

			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = _strdup(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			;
		info->argc = i;

		resolve_alias(info);
		resolve_vars(info);
	}
}

/**
 * free_info - frees info_t struct fields
 * @info: struct address
 * @tr_ue: true if freeing all fields
 */
void free_info(info_t *info, int tr_ue)
{
	free_strarr(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (tr_ue)
	{
		if (!info->command_buffer)
			free(info->arg);
		if (info->env)
			_freenodes(&(info->env));
		if (info->history)
			_freenodes(&(info->history));
		if (info->alias)
			_freenodes(&(info->alias));
		free_strarr(info->environ);
			info->environ = NULL;
		safe_free((void **)info->command_buffer);
		if (info->read_file_descriptor > 2)
			close(info->read_file_descriptor);
		_putchar(BUFFER_FLUSH);
	}
}
