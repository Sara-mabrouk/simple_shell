#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUFFER 1024
#define WRITE_BUFFER 1024
#define BUFFER_FLUSH -1

/* for command chaining */
#define COMMAND_NORM	0
#define COMMAND_OR		1
#define COMMAND_AND		2
#define COMMAND_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HISTORY_FILE	".simple_shell_history"
#define HISTORY_MAX	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *		allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@error_number: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@command_buffer: address of pointer to cmd_buf, on if chaining
 *@command_buffer_type: CMD_type ||, &&, ;
 *@read_file_descriptor: the fd from which to read line input
 *@history_count: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int count;
	int error_number;
	int count_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **command_buffer; /* pointer to cmd ; chain buffer, for memory mangement */
	int command_buffer_type; /* CMD_type ||, &&, ; */
	int read_file_descriptor;
	int history_count;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *command;
	int (*ptr)(info_t *);
} builtin_table;


/* toem_shloop.c */
int shell_loop(info_t *, char **);
int check_builtin(info_t *);
void find_command(info_t *);
void fork_child(info_t *);

/* toem_parser.c */
int check_command(info_t *, char *);
char *str_dup(char *, int, int);
char *check_path(info_t *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* toem_errors.c */
void error_str(char *);
int error_char(char);
int putchar_fd(char c, int file_descriptor);
int putstr_fd(char *str, int file_descriptor);

/* toem_string.c */
int _strlen(char *);
int _strcmp(char *, char *);
char *_strstr (char *haystack, char *needle);
char *_strcat(char *, char *);

/* toem_string1.c */
char *_strcpy(char *, char *);
char *_strdup(char *s);
void _puts(char *);
int _putchar(char);

/* toem_exits.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

/* toem_tokenizer.c */
char **tokenizer(char *, char *);
char **str_tokenizer(char *, char);

/* toem_realloc.c */
char *_memset(char *, char, unsigned int);
void free_strarr(char **);
void *realloc_mem(void *, unsigned int, unsigned int);

/* toem_memory.c */
int safe_free(void **);

/* toem_atoi.c */
int check_interactive(info_t *);
int check_delim(char, char *);
int _isalpha(int);
int _atoi(char *);

/* toem_errors1.c */
int str_to_int(char *);
void p_error(info_t *, char *);
int print_decimal(int, int);
char *convert_number(long int, int, int);
void remove_comments(char *);

/* toem_builtin.c */
int ex_it(info_t *);
int _cd(info_t *);
int _help(info_t *);

/* toem_builtin1.c */
int _history(info_t *);
int _alias(info_t *);

/*toem_getline.c */
ssize_t get_input(info_t *);
int _getline(info_t *, char **, size_t *);
void sigintHandler(int);

/* toem_getinfo.c */
void clr_info(info_t *);
void set_info(info_t *, char **);
void free_info(info_t *, int);

/* toem_environ.c */
char *_getenv(info_t *, const char *);
int _myenv(info_t *);
int _mysetenv(info_t *);
int _myunsetenv(info_t *);
int env_list(info_t *);

/* toem_getenv.c */
char **get_environ(info_t *);
int _unsetenv(info_t *, char *);
int _setenv(info_t *, char *, char *);

/* toem_history.c */
char *_gethistory(info_t *info);
int _writehistory(info_t *info);
int _readhistory(info_t *info);
int _buildhistory(info_t *info, char *buffer, int count);
int _renumberhistory(info_t *info);

/* toem_lists.c */
list_t *_startnode(list_t **, const char *, int);
list_t *_endnode(list_t **, const char *, int);
size_t str_list(const list_t *);
int _deletenode(list_t **, unsigned int);
void _freenodes(list_t **);

/* toem_lists1.c */
size_t _listlength(const list_t *);
char **list_str(list_t *);
size_t _listelements(const list_t *);
list_t *_prefixnode(list_t *, char *, char);
ssize_t _nodeindex(list_t *, list_t *);

/* toem_vars.c */
int test_chain(info_t *, char *, size_t *);
void check_chain(info_t *, char *, size_t *, size_t, size_t);
int resolve_alias(info_t *);
int resolve_vars(info_t *);
int resolve_string(char **, char *);

#endif
