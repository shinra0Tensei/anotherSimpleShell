#include "shell.h"

/**
 * _advatoi - converts a string to integer
 *
 * @str: the string to be converted
 *
 * Return: 0 if the string doesn't have number, converted number otherwise
 */
int _advatoi(char *str)
{
    int i = 0, sign = 1, f = 0;
    int number;
    unsigned int res = 0;

    while (str[i] != '\0' && f != 2)
    {
        if (str[i] == '-')
            sign *= -1;

        if (str[i] >= '0' && str[i] <= '9')
        {
            f = 1;
            res *= 10;
            res += (str[i] - '0');
        }
        else if (f == 1)
            f = 2;
        i++;
    }
    if (sign == -1)
        number = -res;
    else
        number = res;

    return (number);
}
/**
 * intrac_checker - returns true if the shell is in intractive mode
 *
 * @infs: struct address
 *
 * Return: 1 if intractive mode, 0 if not
 */
int intrac_checker(infs_t *infs)
{
	return (isatty(STDIN_FILENO) && infs->read_fd <= 2);
}

/**
 * delimiter_check - checks if a delimeter
 *
 * @c: character to check
 * @delim: the delimeter
 *
 * Return: 1 in cas true, 0 if false
 */
int delimiter_check(char ch, char *delimeter)
{
	while (*delimeter)
		if (*delimeter++ == ch)
			return (1);
	return (0);
}

/**
 * _advisalpha - checks for characters
 *
 * @c: the input charcter
 *
 * Return: 1 if c is alpha, 0 is not
 */
int _advisalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	else
		return (0);
}