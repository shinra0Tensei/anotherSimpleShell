#include "shell.h"

/**
 * hsh - main shell loop
 * @infs: the parameter & return infs struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int hsh(infs_t *infs, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		infos_clearer(infs);
		if (intrac_checker(infs))
			_advputs("$ ");
		_prputchar(FLUSH_BUFFER);
		r = input_req(infs);
		if (r != -1)
		{
			info_setter(infs, av);
			builtin_ret = builtin_finder(infs);
			if (builtin_ret == -1)
				cmd_finder(infs);
		}
		else if (intrac_checker(infs))
			_advputchar('\n');
		info_freer(infs, 0);
	}
	wr_historyfile(infs);
	info_freer(infs, 1);
	if (!intrac_checker(infs) && infs->status)
		exit(infs->status);
	if (builtin_ret == -2)
	{
		if (infs->err_n == -1)
			exit(infs->status);
		exit(infs->err_n);
	}
	return (builtin_ret);
}

/**
 * builtin_finder - finds a builtin command
 * @infs: the parameter & return infs struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int builtin_finder(infs_t *infs)
{
	int i, built_in_ret = -1;
	builtins_t builtintbl[] = {
		{"exit", exit_shell},
		{"envi", show_envir},
		{"help", get_help},
		{"history", get_history},
		{"setenv", set_envir},
		{"unsetenv", unset_envir},
		{"cd", chan_dir},
		{"alias", get_alias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_advstrcmp(infs->argv[0], builtintbl[i].type) == 0)
		{
			infs->err_counter++;
			built_in_ret = builtintbl[i].func(infs);
			break;
		}
	return (built_in_ret);
}

/**
 * cmd_finder - finds a command in PATH
 * @infs: the parameter & return infs struct
 *
 * Return: void
 */
void cmd_finder(infs_t *infs)
{
	char *path = NULL;
	int i, k;

	infs->path = infs->argv[0];
	if (infs->line_count_flag == 1)
	{
		infs->err_counter++;
		infs->line_count_flag = 0;
	}
	for (i = 0, k = 0; infs->arg[i]; i++)
		if (!delimiter_check(infs->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = path_finder(infs, _advgetenvir(infs, "PATH="), infs->argv[0]);
	if (path)
	{
		infs->path = path;
		cmd_fork(infs);
	}
	else
	{
		if ((intrac_checker(infs) || _advgetenvir(infs, "PATH=")
			|| infs->argv[0][0] == '/') && cmd_check(infs, infs->argv[0]))
			cmd_fork(infs);
		else if (*(infs->arg) != '\n')
		{
			infs->status = 127;
			error_printer(infs, "not found\n");
		}
	}
}

/**
 * cmd_fork - forks a an exec thread to run cmd
 * @infs: the parameter & return infs struct
 *
 * Return: void
 */
void cmd_fork(infs_t *infs)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(infs->path, infs->argv, get_envir(infs)) == -1)
		{
			info_freer(infs, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(infs->status));
		if (WIFEXITED(infs->status))
		{
			infs->status = WEXITSTATUS(infs->status);
			if (infs->status == 126)
				error_printer(infs, "Permission denied\n");
		}
	}
}
