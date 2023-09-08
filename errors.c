#include "shell.h"

/**
 *is_eputs - prints an input string
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void is_eputs(char *str)
{
	int i = 0;

	if (!str)
		return;
	while (str[i] != '\0')
	{
		is_erputchar(str[i]);
		i++;
	}
}

/**
 * is_erputchar - writes the character c to stderr
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int is_erputchar(char c)
{
	static int i;
	static char buf[WR_BUFF_SIZE];

	if (c == BUFFER_FLSH || i >= WR_BUFF_SIZE)
	{
		write(2, buf, i);
		i = 0;
	}
	if (c != BUFFER_FLSH)
		buf[i++] = c;
	return (1);
}

/**
 * is__erputfd - writes the character c to given fd
 * @c: The character to print
 * @fd: The filedescriptor to write to
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int is__erputfd(char c, int fd)
{
	static int i;
	static char buf[WR_BUFF_SIZE];

	if (c == BUFFER_FLSH || i >= WR_BUFF_SIZE)
	{
		write(fd, buf, i);
		i = 0;
	}
	if (c != BUFFER_FLSH)
		buf[i++] = c;
	return (1);
}

/**
 *is_eris_putsfd - prints an input string
 * @str: the string to be printed
 * @fd: the filedescriptor to write to
 *
 * Return: the number of chars put
 */
int is_eris_putsfd(char *str, int fd)
{
	int i = 0;

	if (!str)
		return (0);
	while (*str)
	{
		i += is__erputfd(*str++, fd);
	}
	return (i);
}
