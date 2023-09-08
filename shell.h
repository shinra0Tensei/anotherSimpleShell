#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define RD_BUFF_SIZE 1024
#define WR_BUFF_SIZE 1024
#define BUFFER_FLSH -1

/* for command chaining */
#define CMD_NR	0
#define CMD_ORG	1
#define CMD_ND	2
#define CMD_CHN 3

/* for convert_number() */
#define CON_LOW	1
#define CON_UNSD	2

/* 1 if using system getline() */
#define USING_GTLN 0
#define USE_STRTOK 0

#define HST_FILE	".simple_shell_history"
#define HST_MX	4096

extern char **envn;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} ls_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *		allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of envn
 *@envn: custom modified copy of envn from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if envn was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	ls_t *env;
	ls_t *history;
	ls_t *alias;
	char **envn;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} infs_t;

#define INFS_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(infs_t *);
} bltn_tbl;


/* toem_shloop.c */
int hsh(infs_t *, char **);
int find_bltn(infs_t *);
void find_cmmd(infs_t *);
void forks_cmd(infs_t *);

/* toem_parser.c */
int is_cmmd(infs_t *, char *);
char *dups_char(char *, int, int);
char *find_path(infs_t *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* toem_errors.c */
void is_eputs(char *);
int is_erputchar(char);
int is__erputfd(char c, int fd);
int is_eris_putsfd(char *str, int fd);

/* toem_string.c */
int is_strlen(char *);
int is_strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *is_strcat(char *, char *);

/* toem_string1.c */
char *is_strcpy(char *, char *);
char *is_strdup(const char *);
void is_puts(char *);
int is_putchar(char);

/* toem_exits.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

/* toem_tokenizer.c */
char **strtow(char *, char *);
char **strtow2(char *, char);

/* toem_realloc.c */
char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);

/* toem_memory.c */
int bfree(void **);

/* toem_atoi.c */
int interactive(infs_t *);
int is_delim(char, char *);
int _isalpha(int);
int _atoi(char *);

/* toem_errors1.c */
int _erratoi(char *);
void print_error(infs_t *, char *);
int print_d(int, int);
char *convert_number(long int, int, int);
void remove_comments(char *);

/* toem_builtin.c */
int _myexit(infs_t *);
int _mycd(infs_t *);
int _myhelp(infs_t *);

/* toem_builtin1.c */
int _myhistory(infs_t *);
int _myalias(infs_t *);

/*toem_getline.c */
ssize_t get_input(infs_t *);
int _getline(infs_t *, char **, size_t *);
void sigintHandler(int);

/* toem_getinfo.c */
void clear_info(infs_t *);
void set_info(infs_t *, char **);
void free_info(infs_t *, int);

/* toem_envn.c */
char *_getenv(infs_t *, const char *);
int _myenv(infs_t *);
int _mysetenv(infs_t *);
int _myunsetenv(infs_t *);
int populate_env_list(infs_t *);

/* toem_getenv.c */
char **get_envn(infs_t *);
int _unsetenv(infs_t *, char *);
int _setenv(infs_t *, char *, char *);

/* toem_history.c */
char *get_history_file(infs_t *info);
int write_history(infs_t *info);
int read_history(infs_t *info);
int build_history_list(infs_t *info, char *buf, int linecount);
int renumber_history(infs_t *info);

/* toem_lists.c */
ls_t *add_node(ls_t **, const char *, int);
ls_t *add_node_end(ls_t **, const char *, int);
size_t print_list_str(const ls_t *);
int delete_node_at_index(ls_t **, unsigned int);
void free_list(ls_t **);

/* toem_lists1.c */
size_t list_len(const ls_t *);
char **ls_to_strings(ls_t *);
size_t print_list(const ls_t *);
ls_t *node_starts_with(ls_t *, char *, char);
ssize_t get_node_index(ls_t *, ls_t *);

/* toem_vars.c */
int is_chain(infs_t *, char *, size_t *);
void check_chain(infs_t *, char *, size_t *, size_t, size_t);
int replace_alias(infs_t *);
int replace_vars(infs_t *);
int replace_string(char **, char *);

#endif