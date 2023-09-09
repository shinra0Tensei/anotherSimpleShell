#include "shell.h"

/**
 * chan_dir - changes the current directory
 *
 * @infs: potential arguments.
 *
 * Return: Always 0 (Success)
 */
int chan_dir(infs_t *infs)
{
	char *s, *dir, buff[1024];
	int ret_dir;

	s = getcwd(buff, 1024);
	if (!s)
		_advputs("TODO: >>getcwd failure emsg here<<\n");
	if (!infs->argv[1])
	{
		dir = _advgetenvir(infs, "HOME=");
		if (!dir)
			ret_dir =
				chdir((dir = _advgetenvir(infs, "PWD=")) ? dir : "/");
		else
			ret_dir = chdir(dir);
	}
	else if (_advstrcmp(infs->argv[1], "-") == 0)
	{
		if (!_advgetenvir(infs, "OLDPWD="))
		{
			_advputs(s);
			_advputchar('\n');
			return (1);
		}
		_advputs(_advgetenvir(infs, "OLDPWD=")), _advputchar('\n');
		ret_dir = /* TODO: what should this be? */
			chdir((dir = _advgetenvir(infs, "OLDPWD=")) ? dir : "/");
	}
	else
		ret_dir = chdir(infs->argv[1]);
	if (ret_dir == -1)
	{
		error_printer(infs, "can't cd to ");
		_prputs(infs->argv[1]), _prputchar('\n');
	}
	else
	{
		init_setenvir(infs, "OLDPWD", _advgetenvir(infs, "PWD="));
		init_setenvir(infs, "PWD", getcwd(buff, 1024));
	}
	return (0);
}

/**
 * exit_shell - quits the shell
 *
 * @infs: potential arguments,
 *
 * Return: exits with a given exit status
 */
int exit_shell(infs_t *infs)
{
	int exch;

	if (infs->argv[1])  /* If there is an exit arguement */
	{
		exch = _adverratoi(infs->argv[1]);
		if (exch == -1)
		{
			infs->status = 2;
			error_printer(infs, "Illegal number: ");
			_prputs(infs->argv[1]);
			_prputchar('\n');
			return (1);
		}
		infs->err_n = _adverratoi(infs->argv[1]);
		return (-2);
	}
	infs->err_n = -1;
	return (-2);
}

/**
 * get_help - changes the current directory of the process
 *
 * @infs: potential arguments
 *
 * Return: Always 0 (Success)
 */
int get_help(infs_t *infs)
{
	char **argarr;

	argarr = infs->argv;
	_advputs("help call works. Function not yet implemented \n");
	if (0)
		_advputs(*argarr); /* temp att_unused workaround */
	return (0);
}

/**
 * get_history - displays the history list.
 *
 * @infs: potential arguments
 *
 * Return: Always 0 (Success)
 */
int get_history(infs_t *infs)
{
	list_printing(infs->history);
	return (0);
}