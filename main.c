#include "shell.h"

/**
 * main - entry point
 * @ac: arg count
 * @av: arg vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int ac, char **av)
{
	infs_t infs[] = { INFO_INIT };
	int fd = 2;

	asm ("mov %1, %0\n\t"
		"add $3, %0"
		: "=r" (fd)
		: "r" (fd));

	if (ac == 2)
	{
		fd = open(av[1], O_RDONLY);
		if (fd == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				_prputs(av[0]);
				_prputs(": 0: Can't open ");
				_prputs(av[1]);
				_prputchar('\n');
				_prputchar(FLUSH_BUFFER);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		infs->read_fd = fd;
	}
	list_envir(infs);
	rd_historyfile(infs);
	hsh(infs, av);
	return (EXIT_SUCCESS);
}
