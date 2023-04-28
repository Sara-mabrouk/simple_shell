#include "main.h"
/**
 * main - checks code
 * @arg_c: counter
 * @arg_v: A pointer to arg vector
 *
 * Return: 0
 */
int main(int arg_c, char **arg_v)
{
	info_t info[] = { INFO_INIT};
	int file_descriptor = 2;

	asm ("mov %1, %0\n\t"
		"add $3, %0"
		: "=r" (file_descriptor)
		: "r" (file_descriptor));
	if (arg_c == 2)
	{
		file_descriptor = open(arg_v[1], O_RDONLY);
		if (file_descriptor == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				error_str(arg_v[0]);
				error_str(": 0: Not accessible ");
				error_str(arg_v[1]);
				error_char('\n');
				error_char(BUFFER_FLUSH);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		info->read_file_descriptor = file_descriptor;
	}
	env_list(info);
	_readhistory(info);
	shell_loop(info, arg_v);
	return (EXIT_SUCCESS);
}
