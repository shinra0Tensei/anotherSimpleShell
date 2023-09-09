#include "shell.h"

/**
 * show_envir - prints the current environment
 * @infs: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int show_envir(infs_t *infs)
{
	str_list_printing(infs->envi);
	return (0);
}

/**
 * _advgetenvir - gets the value of an environ variable
 * @infs: Structure containing potential arguments. Used to maintain
 * @name: envi var name
 *
 * Return: the value
 */
char *_advgetenvir(infs_t *infs, const char *name)
{
	list_t *node = infs->envi;
	char *p;

	while (node)
	{
		p = begg_with(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * set_envir - Initialize a new environment variable,
 *             or modify an existing one
 * @infs: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int set_envir(infs_t *infs)
{
	if (infs->argc != 3)
	{
		_prputs("Incorrect number of arguements\n");
		return (1);
	}
	if (init_setenvir(infs, infs->argv[1], infs->argv[2]))
		return (0);
	return (1);
}

/**
 * unset_envir - Remove an environment variable
 * @infs: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int unset_envir(infs_t *infs)
{
	int i;

	if (infs->argc == 1)
	{
		_prputs("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= infs->argc; i++)
		rem_unsetenvir(infs, infs->argv[i]);

	return (0);
}

/**
 * list_envir - populates envi linked list
 * @infs: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int list_envir(infs_t *infs)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		adding_nodeatend(&node, environ[i], 0);
	infs->envi = node;
	return (0);
}
