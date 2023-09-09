#include "shell.h"

/**
 * cmd_check - determines if a file is an executable command
 * @infs: the infs struct
 * @path: path to the file
 *
 * Return: 1 if true, 0 otherwise
 */
int cmd_check(infs_t *infs, char *path)
{
	struct stat st;

	(void)infs;
	if (!path || stat(path, &st))
		return (0);

	if (st.st_mode & S_IFREG)
	{
		return (1);
	}
	return (0);
}

/**
 * char_dupli - duplicates characters
 * @pathstr: the PATH string
 * @start: starting index
 * @stop: stopping index
 *
 * Return: pointer to new buffer
 */
char *char_dupli(char *pathstr, int start, int stop)
{
	static char buf[1024];
	int i = 0, k = 0;

	for (k = 0, i = start; i < stop; i++)
		if (pathstr[i] != ':')
			buf[k++] = pathstr[i];
	buf[k] = 0;
	return (buf);
}

/**
 * path_finder - finds this cmd in the PATH string
 * @infs: the infs struct
 * @pathstr: the PATH string
 * @cmd: the cmd to find
 *
 * Return: full path of cmd if found or NULL
 */
char *path_finder(infs_t *infs, char *pathstr, char *cmd)
{
	int i = 0, curr_pos = 0;
	char *path;

	if (!pathstr)
		return (NULL);
	if ((_advstrlen(cmd) > 2) && begg_with(cmd, "./"))
	{
		if (cmd_check(infs, cmd))
			return (cmd);
	}
	while (1)
	{
		if (!pathstr[i] || pathstr[i] == ':')
		{
			path = char_dupli(pathstr, curr_pos, i);
			if (!*path)
				_advstrcat(path, cmd);
			else
			{
				_advstrcat(path, "/");
				_advstrcat(path, cmd);
			}
			if (cmd_check(infs, path))
				return (path);
			if (!pathstr[i])
				break;
			curr_pos = i;
		}
		i++;
	}
	return (NULL);
}
