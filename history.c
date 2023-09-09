#include "shell.h"

/**
 * show_historyfile - gets the history file
 * @infs: parameter struct
 *
 * Return: allocated string containg history file
 */

char *show_historyfile(infs_t *infs)
{
	char *buf, *dir;

	dir = _advgetenvir(infs, "HOME=");
	if (!dir)
		return (NULL);
	buf = malloc(sizeof(char) * (_advstrlen(dir) + _advstrlen(HISTORY_FILE) + 2));
	if (!buf)
		return (NULL);
	buf[0] = 0;
	_advstrcpy(buf, dir);
	_advstrcat(buf, "/");
	_advstrcat(buf, HISTORY_FILE);
	return (buf);
}

/**
 * wr_historyfile - creates a file, or appends to an existing file
 * @infs: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int wr_historyfile(infs_t *infs)
{
	ssize_t fd;
	char *filename = show_historyfile(infs);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = infs->history; node; node = node->next)
	{
		_prputs_fd(node->str, fd);
		_prput_fd('\n', fd);
	}
	_prput_fd(FLUSH_BUFFER, fd);
	close(fd);
	return (1);
}

/**
 * rd_historyfile - reads history from file
 * @infs: the parameter struct
 *
 * Return: histcount on success, 0 otherwise
 */
int rd_historyfile(infs_t *infs)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = show_historyfile(infs);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buf = malloc(sizeof(char) * (fsize + 1));
	if (!buf)
		return (0);
	rdlen = read(fd, buf, fsize);
	buf[fsize] = 0;
	if (rdlen <= 0)
		return (free(buf), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			list_historyfile(infs, buf + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		list_historyfile(infs, buf + last, linecount++);
	free(buf);
	infs->histcount = linecount;
	while (infs->histcount-- >= HISTORY_MAXI)
		deleting_nodeatindex(&(infs->history), 0);
	renum_historyfile(infs);
	return (infs->histcount);
}

/**
 * list_historyfile - adds entry to a history linked list
 * @infs: Structure containing potential arguments. Used to maintain
 * @buf: buffer
 * @linecount: the history linecount, histcount
 *
 * Return: Always 0
 */
int list_historyfile(infs_t *infs, char *buf, int linecount)
{
	list_t *node = NULL;

	if (infs->history)
		node = infs->history;
	adding_nodeatend(&node, buf, linecount);

	if (!infs->history)
		infs->history = node;
	return (0);
}

/**
 * renum_historyfile - renumbers the history linked list after changes
 * @infs: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histcount
 */
int renum_historyfile(infs_t *infs)
{
	list_t *node = infs->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (infs->histcount = i);
}
