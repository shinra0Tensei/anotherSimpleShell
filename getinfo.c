#include "shell.h"

/**
 * infos_clearer - initializes infs_t struct
 * @infs: struct address
 */
void infos_clearer(infs_t *infs)
{
	infs->arg = NULL;
	infs->argv = NULL;
	infs->path = NULL;
	infs->argc = 0;
}

/**
 * info_setter - initializes infs_t struct
 * @infs: struct address
 * @av: argument vector
 */
void info_setter(infs_t *infs, char **av)
{
	int i = 0;

	infs->file_name = av[0];
	if (infs->arg)
	{
		infs->argv = strtowadv_A(infs->arg, " \t");
		if (!infs->argv)
		{

			infs->argv = malloc(sizeof(char *) * 2);
			if (infs->argv)
			{
				infs->argv[0] = _advstrdup(infs->arg);
				infs->argv[1] = NULL;
			}
		}
		for (i = 0; infs->argv && infs->argv[i]; i++)
			;
		infs->argc = i;

		alias_replacing(infs);
		vars_replacing(infs);
	}
}

/**
 * info_freer - frees infs_t struct fields
 * @infs: struct address
 * @all: true if freeing all fields
 */
void info_freer(infs_t *infs, int all)
{
	str_free(infs->argv);
	infs->argv = NULL;
	infs->path = NULL;
	if (all)
	{
		if (!infs->cmd_buffer)
			free(infs->arg);
		if (infs->envi)
			freing_list(&(infs->envi));
		if (infs->history)
			freing_list(&(infs->history));
		if (infs->alias)
			freing_list(&(infs->alias));
		str_free(infs->environ);
			infs->environ = NULL;
		buff_ffree((void **)infs->cmd_buffer);
		if (infs->read_fd > 2)
			close(infs->read_fd);
		_advputchar(FLUSH_BUFFER);
	}
}
