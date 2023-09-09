#include "shell.h"

/**
 * get_history - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @infs: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int get_history(infs_t *infs)
{
	list_printing(infs->history);
	return (0);
}

/**
 * unset_alias - sets alias to string
 * @infs: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int unset_alias(infs_t *infs, char *str)
{
	char *p, c;
	int ret;

	p = _advstrchr(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = deleting_nodeatindex(&(infs->alias),
		getting_nodeatindex(infs->alias, node_beggwith(infs->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * set_alias - sets alias to string
 * @infs: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_alias(infs_t *infs, char *str)
{
	char *p;

	p = _advstrchr(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unset_alias(infs, str));

	unset_alias(infs, str);
	return (adding_nodeatend(&(infs->alias), str, 0) == NULL);
}

/**
 * print_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int print_alias(list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = _advstrchr(node->str, '=');
		for (a = node->str; a <= p; a++)
			_advputchar(*a);
		_advputchar('\'');
		_advputs(p + 1);
		_advputs("'\n");
		return (0);
	}
	return (1);
}

/**
 * get_alias - mimics the alias builtin (man alias)
 * @infs: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int get_alias(infs_t *infs)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (infs->argc == 1)
	{
		node = infs->alias;
		while (node)
		{
			print_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; infs->argv[i]; i++)
	{
		p = _advstrchr(infs->argv[i], '=');
		if (p)
			set_alias(infs, infs->argv[i]);
		else
			print_alias(node_beggwith(infs->alias, infs->argv[i], '='));
	}

	return (0);
}
