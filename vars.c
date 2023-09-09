#include "shell.h"

/**
 * chain_checker - test if current char in buffer is a chain delimeter
 * @infs: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 *
 * Return: 1 if chain delimeter, 0 otherwise
 */
int chain_checker(infs_t *infs, char *buf, size_t *p)
{
	size_t j = *p;

	if (buf[j] == '|' && buf[j + 1] == '|')
	{
		buf[j] = 0;
		j++;
		infs->cmd_buffer_type = CMD_OR;
	}
	else if (buf[j] == '&' && buf[j + 1] == '&')
	{
		buf[j] = 0;
		j++;
		infs->cmd_buffer_type = CMD_AND;
	}
	else if (buf[j] == ';') /* found end of this command */
	{
		buf[j] = 0; /* replace semicolon with null */
		infs->cmd_buffer_type = CMD_CHAIN;
	}
	else
		return (0);
	*p = j;
	return (1);
}

/**
 * checking_chain - checks we should continue chaining based on last status
 * @infs: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 * @i: starting position in buf
 * @len: length of buf
 *
 * Return: Void
 */
void checking_chain(infs_t *infs, char *buf, size_t *p, size_t i, size_t len)
{
	size_t j = *p;

	if (infs->cmd_buffer_type == CMD_AND)
	{
		if (infs->status)
		{
			buf[i] = 0;
			j = len;
		}
	}
	if (infs->cmd_buffer_type == CMD_OR)
	{
		if (!infs->status)
		{
			buf[i] = 0;
			j = len;
		}
	}

	*p = j;
}

/**
 * alias_replacing - replaces an aliases in the tokenized string
 * @infs: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int alias_replacing(infs_t *infs)
{
	int i;
	list_t *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = node_beggwith(infs->alias, infs->argv[0], '=');
		if (!node)
			return (0);
		free(infs->argv[0]);
		p = _advstrchr(node->str, '=');
		if (!p)
			return (0);
		p = _advstrdup(p + 1);
		if (!p)
			return (0);
		infs->argv[0] = p;
	}
	return (1);
}

/**
 * vars_replacing - replaces vars in the tokenized string
 * @infs: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int vars_replacing(infs_t *infs)
{
	int i = 0;
	list_t *node;

	for (i = 0; infs->argv[i]; i++)
	{
		if (infs->argv[i][0] != '$' || !infs->argv[i][1])
			continue;

		if (!_advstrcmp(infs->argv[i], "$?"))
		{
			string_replacing(&(infs->argv[i]),
				_advstrdup(number_converter(infs->status, 10, 0)));
			continue;
		}
		if (!_advstrcmp(infs->argv[i], "$$"))
		{
			string_replacing(&(infs->argv[i]),
				_advstrdup(number_converter(getpid(), 10, 0)));
			continue;
		}
		node = node_beggwith(infs->envi, &infs->argv[i][1], '=');
		if (node)
		{
			string_replacing(&(infs->argv[i]),
				_advstrdup(_advstrchr(node->str, '=') + 1));
			continue;
		}
		string_replacing(&infs->argv[i], _advstrdup(""));

	}
	return (0);
}

/**
 * string_replacing - replaces string
 * @old: address of old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int string_replacing(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}
