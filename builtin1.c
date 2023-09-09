#include "shell.h"

/**
 * unset_alias - sets alias to string
 *
 * @infs: parameter
 * @str: the alias
 *
 * Return: Always 0 (success), 1 (error)
 */
int unset_alias(infs_t *infs, char *str)
{
	char *pt, c;
	int ret;

	pt = _advstrchr(str, '=');
	if (!pt)
		return (1);
	c = *pt;
	*pt = 0;
	ret = deleting_nodeatindex(&(infs->alias),
		getting_nodeatindex(infs->alias, node_beggwith(infs->alias, str, -1)));
	*pt = c;
	return (ret);
}

/**
 * set_alias - sets alias
 *
 * @infs: parameter
 * @str: the alias
 *
 * Return: Always 0 (success), 1 (error)
 */
int set_alias(infs_t *infs, char *str)
{
	char *pt;

	pt = _advstrchr(str, '=');
	if (!pt)
		return (1);
	if (!*++pt)
		return (unset_alias(infs, str));

	unset_alias(infs, str);
	return (adding_nodeatend(&(infs->alias), str, 0) == NULL);
}

/**
 * print_alias - prints an alias string
 *
 * @node: the aliases node
 *
 * Return: Always 0 (success), 1 (error)
 */
int print_alias(list_t *node)
{
	char *pt = NULL, *a = NULL;

	if (node)
	{
		pt = _advstrchr(node->str, '=');
		a = node->str;
		while (a <= pt)
		{
			_advputchar(*a);
			a++;
		}
		_advputchar('\'');
		_advputs(pt + 1);
		_advputs("'\n");

		return (0);
	}
	return (1);
}

/**
 * get_alias - man alias mimic
 *
 * @infs: potential arguments
 *
 * Return: Always 0 (Success)
 */
int get_alias(infs_t *infs)
{
	int i = 0;
	char *pt = NULL;
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
		pt = _advstrchr(infs->argv[i], '=');
		if (pt)
			set_alias(infs, infs->argv[i]);
		else
			print_alias(node_beggwith(infs->alias, infs->argv[i], '='));
	}

	return (0);
}