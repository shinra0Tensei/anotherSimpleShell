#ifndef _SHELL_H_
#define _SHELL_H_

/* ==================== LIBRARIES ==================== */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

/* ==================== CONSTANTS ==================== */
/* buffers */
#define RD_BUFFER_SIZE 1024
#define WR_BUFFER_SIZE 1024
#define FLUSH_BUFFER -1
/* convert */
#define LOWER_CONV	1
#define UNSIGNED_CONV	2
/* command */
#define CMD_NORM	0
#define CMD_AND		2
#define CMD_CHAIN	3
#define CMD_OR		1
/* history creating */
#define HISTORY_FILE	".simple_shell_history"
#define HISTORY_MAXI	4096
/* using getline */
#define GETLINE_USING 0
#define STRTOK_USING 0

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

extern char **environ;


/* ==================== STRUCTURES ==================== */
/**
 * struct list_str - singly linked list
 *
 * @num: the numbers
 * @str: the string
 * @next: next node
 */
typedef struct list_str
{
	int num;
	char *str;
	struct list_str *next;
} list_t;

/**
 * struct pass_info - Encompasses pseudo-arguments for passing into a function,
 * ensuring a consistent prototype for a function pointer structure.
 *
 * @file_name: the program filename
 * @envi: linked list of environ
 * @environ: copy of environ from LL envi
 * @history: the history node
 * @err_counter: the error counter
 * @err_n: the error code for exiting
 * @line_count_flag: if on count this line of input
 * @cmd_buffer: address of cmd_buffer
 * @cmd_buffer_type: type of cmd ||, &&, ;
 * @arg: a string containing arguements
 * @argv: an array of strings from arg
 * @path: the path of the current command -string-
 * @argc: the argument counter
 * @read_fd: the fd from which to read line input
 * @histcount: the history line number count
 * @alias: the alias
 * @new_env: in case of environ is changed
 * @status: status of the last command
 */
typedef struct pass_info
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int err_counter;
	int err_n;
	int line_count_flag;
	char *file_name;
	list_t *envi;
	list_t *history;
	list_t *alias;
	char **environ;
	int new_env;
	int status;
	char **cmd_buffer;
	int cmd_buffer_type;
	int read_fd;
	int histcount;
} infs_t;

/**
 * struct built_ins - contains the builtins and other functions
 *
 * @type: the builtin flag of the command
 * @func: the function to be executed
 */
typedef struct built_ins
{
	char *type;
	int (*func)(infs_t *);
} builtins_t;


/* ==================== FUNCTIONS ==================== */
/* printing functions*/
void _advputs(char *);
int _advputchar(char);
int _advstrlen(char *);
int _advstrcmp(char *, char *);
char *begg_with(const char *, const char *);
char *_advstrcat(char *, char *);
char *_advstrcpy(char *, char *);
char *_advstrdup(const char *);
int _prput_fd(char c, int fd);
int _prputs_fd(char *str, int fd);
void _prputs(char *);
int _prputchar(char);
char **strtowadv_A(char *, char *);
char **strtowadv_B(char *, char);
char *_advstrncpy(char *, char *, int);
char *_advstrncat(char *, char *, int);
char *_advstrchr(char *, char);

/* memory handlers */
char *_advmemseter(char *, char, unsigned int);
void str_free(char **);
void *_advrealloctor(void *, unsigned int, unsigned int);
int buff_ffree(void **);

/* ASCII to Integer */
int intrac_checker(infs_t *);
int delimiter_check(char, char *);
int _advisalpha(int);
int _advatoi(char *);

/* built ins handlers */
int exit_shell(infs_t *);
int chan_dir(infs_t *);
int get_history(infs_t *);
int get_alias(infs_t *);
int get_help(infs_t *);

/* hostory handler */
char *show_historyfile(infs_t *infs);
int renum_historyfile(infs_t *infs);
int wr_historyfile(infs_t *infs);
int rd_historyfile(infs_t *infs);
int list_historyfile(infs_t *infs, char *buf, int linecount);

/* informations */
void infos_clearer(infs_t *);
void info_setter(infs_t *, char **);
ssize_t input_req(infs_t *);
int _advgetline(infs_t *, char **, size_t *);
void tohandler(int);
void info_freer(infs_t *, int);

/* enviroments handlers */
char *_advgetenvir(infs_t *, const char *);
int show_envir(infs_t *);
int set_envir(infs_t *);
int unset_envir(infs_t *);
char **get_envir(infs_t *);
int rem_unsetenvir(infs_t *, char *);
int init_setenvir(infs_t *, char *, char *);
int list_envir(infs_t *);

/* parsers */
int cmd_check(infs_t *, char *);
char *char_dupli(char *, int, int);
char *path_finder(infs_t *, char *, char *);
/* shell loops */
int hsh(infs_t *, char **);
int builtin_finder(infs_t *);
void cmd_finder(infs_t *);
void cmd_fork(infs_t *);
int loophsh(char **);

/* error handler */
int _adverratoi(char *);
void error_printer(infs_t *, char *);
int decim_printer(int, int);
char *number_converter(long int, int, int);
void comment_deleter(char *);

/* vars handlers */
int chain_checker(infs_t *, char *, size_t *);
void checking_chain(infs_t *, char *, size_t *, size_t, size_t);
int alias_replacing(infs_t *);
int vars_replacing(infs_t *);
int string_replacing(char **, char *);

/* lists handler */
list_t *adding_node(list_t **, const char *, int);
list_t *adding_nodeatend(list_t **, const char *, int);
size_t str_list_printing(const list_t *);
size_t list_printing(const list_t *);
list_t *node_beggwith(list_t *, char *, char);
ssize_t getting_nodeatindex(list_t *, list_t *);
int deleting_nodeatindex(list_t **, unsigned int);
void freing_list(list_t **);
size_t list_length(const list_t *);
char **lists_2_strings(list_t *);

#endif