#include "shell.h"

/**
 * input_buf - buffers chained commands
 * @infs: parameter struct
 * @buf: address of buffer
 * @len: address of len var
 *
 * Return: bytes read
 */
ssize_t input_buf(infs_t *infs, char **buf, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len) /* if nothing left in the buffer, fill it */
	{
		/*buff_ffree((void **)infs->cmd_buffer);*/
		free(*buf);
		*buf = NULL;
		signal(SIGINT, tohandler);
#if GETLINE_USING
		r = getline(buf, &len_p, stdin);
#else
		r = _advgetline(infs, buf, &len_p);
#endif
		if (r > 0)
		{
			if ((*buf)[r - 1] == '\n')
			{
				(*buf)[r - 1] = '\0'; /* remove trailing newline */
				r--;
			}
			infs->line_count_flag = 1;
			comment_deleter(*buf);
			list_historyfile(infs, *buf, infs->histcount++);
			/* if (_advstrchr(*buf, ';')) is this a command chain? */
			{
				*len = r;
				infs->cmd_buffer = buf;
			}
		}
	}
	return (r);
}

/**
 * input_req - gets a line minus the newline
 * @infs: parameter struct
 *
 * Return: bytes read
 */
ssize_t input_req(infs_t *infs)
{
	static char *buf; /* the ';' command chain buffer */
	static size_t i, j, len;
	ssize_t r = 0;
	char **buf_p = &(infs->arg), *p;

	_advputchar(FLUSH_BUFFER);
	r = input_buf(infs, &buf, &len);
	if (r == -1) /* EOF */
		return (-1);
	if (len)	/* we have commands left in the chain buffer */
	{
		j = i; /* init new iterator to current buf position */
		p = buf + i; /* get pointer for return */

		checking_chain(infs, buf, &j, i, len);
		while (j < len) /* iterate to semicolon or end */
		{
			if (chain_checker(infs, buf, &j))
				break;
			j++;
		}

		i = j + 1; /* increment past nulled ';'' */
		if (i >= len) /* reached end of buffer? */
		{
			i = len = 0; /* reset position and length */
			infs->cmd_buffer_type = CMD_NORM;
		}

		*buf_p = p; /* pass back pointer to current command position */
		return (_advstrlen(p)); /* return length of current command */
	}

	*buf_p = buf; /* else not a chain, pass back buffer from _advgetline() */
	return (r); /* return length of buffer from _advgetline() */
}

/**
 * read_buf - reads a buffer
 * @infs: parameter struct
 * @buf: buffer
 * @i: size
 *
 * Return: r
 */
ssize_t read_buf(infs_t *infs, char *buf, size_t *i)
{
	ssize_t r = 0;

	if (*i)
		return (0);
	r = read(infs->read_fd, buf, RD_BUFFER_SIZE);
	if (r >= 0)
		*i = r;
	return (r);
}

/**
 * _advgetline - gets the next line of input from STDIN
 * @infs: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: s
 */
int _advgetline(infs_t *infs, char **ptr, size_t *length)
{
	static char buf[RD_BUFFER_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (i == len)
		i = len = 0;

	r = read_buf(infs, buf, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = _advstrchr(buf + i, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = _advrealloctor(p, s, s ? s + k : k + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (s)
		_advstrncat(new_p, buf + i, k - i);
	else
		_advstrncpy(new_p, buf + i, k - i + 1);

	s += k - i;
	i = k;
	p = new_p;

	if (length)
		*length = s;
	*ptr = p;
	return (s);
}

/**
 * tohandler - blocks ctrl-C
 * @sig_num: the signal number
 *
 * Return: void
 */
void tohandler(__attribute__((unused))int sig_num)
{
	_advputs("\n");
	_advputs("$ ");
	_advputchar(FLUSH_BUFFER);
}
