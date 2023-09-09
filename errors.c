#include "shell.h"

/**
 *_prputs - prints an input string
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void _prputs(char *str)
{
	int i = 0;

	if (!str)
		return;
	while (str[i] != '\0')
	{
		_prputchar(str[i]);
		i++;
	}
}

/**
 * _prputchar - writes the character c to stderr
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _prputchar(char c)
{
	static int i;
	static char buf[WR_BUFFER_SIZE];

	if (c == FLUSH_BUFFER || i >= WR_BUFFER_SIZE)
	{
		write(2, buf, i);
		i = 0;
	}
	if (c != FLUSH_BUFFER)
		buf[i++] = c;
	return (1);
}

/**
 * _prput_fd - writes the character c to given fd
 * @c: The character to print
 * @fd: The filedescriptor to write to
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _prput_fd(char c, int fd)
{
	static int i;
	static char buf[WR_BUFFER_SIZE];

	if (c == FLUSH_BUFFER || i >= WR_BUFFER_SIZE)
	{
		write(fd, buf, i);
		i = 0;
	}
	if (c != FLUSH_BUFFER)
		buf[i++] = c;
	return (1);
}

/**
 *_prputs_fd - prints an input string
 * @str: the string to be printed
 * @fd: the filedescriptor to write to
 *
 * Return: the number of chars put
 */
int _prputs_fd(char *str, int fd)
{
	int i = 0;

	if (!str)
		return (0);
	while (*str)
	{
		i += _prput_fd(*str++, fd);
	}
	return (i);
}
