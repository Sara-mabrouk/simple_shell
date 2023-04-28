#include "main.h"

/**
 * **tokenizer - splits a string into words. Repeat delimiters are ignored
 * @str: the input string
 * @delim: the delimeter string
 * Return: a pointer to an array of strings, or NULL on failure
 */

char **tokenizer(char *str, char *delim)
{
	int i, j, k, m, num = 0;
	char **arrstr;

	if (str == NULL || str[0] == 0)
		return (NULL);
	if (!delim)
		delim = " ";
	for (i = 0; str[i] != '\0'; i++)
		if (!check_delim(str[i], delim) &&
		(check_delim(str[i + 1], delim) || !str[i + 1]))
			num++;

	if (num == 0)
		return (NULL);
	arrstr = malloc((1 + num) * sizeof(char *));
	if (!arrstr)
		return (NULL);
	for (i = 0, j = 0; j < num; j++)
	{
		while (check_delim(str[i], delim))
			i++;
		k = 0;
		while (!check_delim(str[i + k], delim) && str[i + k])
			k++;
		arrstr[j] = malloc((k + 1) * sizeof(char));
		if (!arrstr[j])
		{
			for (k = 0; k < j; k++)
				free(arrstr[k]);
			free(arrstr);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			arrstr[j][m] = str[i++];
		arrstr[j][m] = 0;
	}
	arrstr[j] = NULL;
	return (arrstr);
}

/**
 * **str_tokenizer - splits a string into words
 * @str: the input string
 * @delim: the delimeter
 * Return: a pointer to an array of strings
 */
char **str_tokenizer(char *str, char delim)
{
	int i, j, k, m, num = 0;
	char **arrstr;

	if (str == NULL || str[0] == 0)
		return (NULL);
	for (i = 0; str[i] != '\0'; i++)
		if ((str[i] != delim && str[i + 1] == delim) ||
		    (str[i] != delim && !str[i + 1]) || str[i + 1] == delim)
			num++;
	if (num == 0)
		return (NULL);
	arrstr = malloc((1 + num) * sizeof(char *));
	if (!arrstr)
		return (NULL);
	for (i = 0, j = 0; j < num; j++)
	{
		while (str[i] == delim && str[i] != delim)
			i++;
		k = 0;
		while (str[i + k] != delim && str[i + k] && str[i + k] != delim)
			k++;
		arrstr[j] = malloc((k + 1) * sizeof(char));
		if (!arrstr[j])
		{
			for (k = 0; k < j; k++)
				free(arrstr[k]);
			free(arrstr);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			arrstr[j][m] = str[i++];
		arrstr[j][m] = 0;
	}
	arrstr[j] = NULL;
	return (arrstr);
}
