#include "shell.h"

/**
 * get_envir - returns the string array copy of our environ
 * @infs: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **get_envir(infs_t *infs)
{
	if (!infs->environ || infs->new_env)
	{
		infs->environ = lists_2_strings(infs->envi);
		infs->new_env = 0;
	}

	return (infs->environ);
}

/**
 * rem_unsetenvir - Remove an environment variable
 * @infs: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: 1 on delete, 0 otherwise
 * @var: the string envi var property
 */
int rem_unsetenvir(infs_t *infs, char *var)
{
	list_t *node = infs->envi;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = begg_with(node->str, var);
		if (p && *p == '=')
		{
			infs->new_env = deleting_nodeatindex(&(infs->envi), i);
			i = 0;
			node = infs->envi;
			continue;
		}
		node = node->next;
		i++;
	}
	return (infs->new_env);
}

/**
 * init_setenvir - Initialize a new environment variable,
 *             or modify an existing one
 * @infs: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string envi var property
 * @value: the string envi var value
 *  Return: Always 0
 */
int init_setenvir(infs_t *infs, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(_advstrlen(var) + _advstrlen(value) + 2);
	if (!buf)
		return (1);
	_advstrcpy(buf, var);
	_advstrcat(buf, "=");
	_advstrcat(buf, value);
	node = infs->envi;
	while (node)
	{
		p = begg_with(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			infs->new_env = 1;
			return (0);
		}
		node = node->next;
	}
	adding_nodeatend(&(infs->envi), buf, 0);
	free(buf);
	infs->new_env = 1;
	return (0);
}
