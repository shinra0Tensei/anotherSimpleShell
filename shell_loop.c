#include "shell.h"

/**
 * hsh - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int hsh(infs_t *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		clear_info(info);
		if (is_intrctv(info))
			is_puts("$ ");
		is_erputchar(BUFFER_FLSH);
		r = is_get_input(info);
		if (r != -1)
		{
			set_info(info, av);
			builtin_ret = find_bltn(info);
			if (builtin_ret == -1)
				find_cmmd(info);
		}
		else if (is_intrctv(info))
			is_putchar('\n');
		free_info(info, 0);
	}
	wrt_history(info);
	free_info(info, 1);
	if (!is_intrctv(info) && info->status)
		exit(info->status);
	if (builtin_ret == -2)
	{
		if (info->err_num == -1)
			exit(info->status);
		exit(info->err_num);
	}
	return (builtin_ret);
}

/**
 * find_bltn - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int find_bltn(infs_t *info)
{
	int i, built_in_ret = -1;
	bltn_tbl builtintbl[] = {
		{"exit", is_exit},
		{"env", is_env},
		{"help", is_help},
		{"history", is_history},
		{"setenv", isis_setenv},
		{"unsetenv", isis_myunsetenv},
		{"cd", is_mycd},
		{"alias", is_myalias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (is_strcmp(info->argv[0], builtintbl[i].type) == 0)
		{
			info->line_count++;
			built_in_ret = builtintbl[i].func(info);
			break;
		}
	return (built_in_ret);
}

/**
 * find_cmmd - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void find_cmmd(infs_t *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag = 0;
	}
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!_delim(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = find_path(info, _getenv(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		forks_cmd(info);
	}
	else
	{
		if ((is_intrctv(info) || _getenv(info, "PATH=")
			|| info->argv[0][0] == '/') && is_cmmd(info, info->argv[0]))
			forks_cmd(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			is_print_error(info, "not found\n");
		}
	}
}

/**
 * forks_cmd - forks a an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void forks_cmd(infs_t *info)
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
		if (execve(info->path, info->argv, get_envn(info)) == -1)
		{
			free_info(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				is_print_error(info, "Permission denied\n");
		}
	}
}
