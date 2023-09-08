#include "shell.h"

/**
 * is_env - prints the current envnment
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int is_env(infs_t *info)
{
	prt_lst_str(info->env);
	return (0);
}

/**
 * _getenv - gets the value of an envn variable
 * @info: Structure containing potential arguments. Used to maintain
 * @name: env var name
 *
 * Return: the value
 */
char *is_getenv(infs_t *info, const char *name)
{
	ls_t *node = info->env;
	char *p;

	while (node)
	{
		p = starts_with(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * isis_setenv - Initialize a new envnment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int isis_setenv(infs_t *info)
{
	if (info->argc != 3)
	{
		is_eputs("Incorrect number of arguements\n");
		return (1);
	}
	if (is_setenv(info, info->argv[1], info->argv[2]))
		return (0);
	return (1);
}

/**
 * isis_myunsetenv - Remove an envnment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int isis_myunsetenv(infs_t *info)
{
	int i;

	if (info->argc == 1)
	{
		is_eputs("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= info->argc; i++)
		is_myunsetenv(info, info->argv[i]);

	return (0);
}

/**
 * is_poplt_env_list - populates env linked list
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int is_poplt_env_list(infs_t *info)
{
	ls_t *node = NULL;
	size_t i;

	for (i = 0; envn[i]; i++)
		add_nd_end(&node, envn[i], 0);
	info->env = node;
	return (0);
}
