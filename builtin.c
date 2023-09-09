#include "shell.h"

/**
 * exit_shell - quits the shell
 * @infs: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: exits with a given exit status
 *         (0) if infs.argv[0] != "exit"
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
 * chan_dir - changes the current directory of the process
 * @infs: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int chan_dir(infs_t *infs)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		_advputs("TODO: >>getcwd failure emsg here<<\n");
	if (!infs->argv[1])
	{
		dir = _advgetenvir(infs, "HOME=");
		if (!dir)
			chdir_ret = /* TODO: what should this be? */
				chdir((dir = _advgetenvir(infs, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
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
		chdir_ret = /* TODO: what should this be? */
			chdir((dir = _advgetenvir(infs, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(infs->argv[1]);
	if (chdir_ret == -1)
	{
		error_printer(infs, "can't cd to ");
		_prputs(infs->argv[1]), _prputchar('\n');
	}
	else
	{
		init_setenvir(infs, "OLDPWD", _advgetenvir(infs, "PWD="));
		init_setenvir(infs, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * get_help - changes the current directory of the process
 * @infs: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int get_help(infs_t *infs)
{
	char **arg_array;

	arg_array = infs->argv;
	_advputs("help call works. Function not yet implemented \n");
	if (0)
		_advputs(*arg_array); /* temp att_unused workaround */
	return (0);
}
