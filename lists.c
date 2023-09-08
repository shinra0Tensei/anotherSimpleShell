#include "shell.h"

/**
 * add_node - adds a node to the start of the list
 * @head: address of pointer to head node
 * @str: str field of node
 * @num: node index used by history
 *
 * Return: size of list
 */
ls_t *add_node(ls_t **head, const char *str, int num)
{
	ls_t *new_head;

	if (!head)
		return (NULL);
	new_head = malloc(sizeof(ls_t));
	if (!new_head)
		return (NULL);
	_memset((void *)new_head, 0, sizeof(ls_t));
	new_head->num = num;
	if (str)
	{
		new_head->str = _strdup(str);
		if (!new_head->str)
		{
			free(new_head);
			return (NULL);
		}
	}
	new_head->next = *head;
	*head = new_head;
	return (new_head);
}

/**
 * add_nd_end - adds a node to the end of the list
 * @head: address of pointer to head node
 * @str: str field of node
 * @num: node index used by history
 *
 * Return: size of list
 */
ls_t *add_nd_end(ls_t **head, const char *str, int num)
{
	ls_t *new_node, *node;

	if (!head)
		return (NULL);

	node = *head;
	new_node = malloc(sizeof(ls_t));
	if (!new_node)
		return (NULL);
	_memset((void *)new_node, 0, sizeof(ls_t));
	new_node->num = num;
	if (str)
	{
		new_node->str = _strdup(str);
		if (!new_node->str)
		{
			free(new_node);
			return (NULL);
		}
	}
	if (node)
	{
		while (node->next)
			node = node->next;
		node->next = new_node;
	}
	else
		*head = new_node;
	return (new_node);
}

/**
 * prt_lst_str - prints only the str element of a ls_t linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t prt_lst_str(const ls_t *h)
{
	size_t i = 0;

	while (h)
	{
		is_puts(h->str ? h->str : "(nil)");
		is_puts("\n");
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * delt_nd_at_indx - deletes node at given index
 * @head: address of pointer to first node
 * @index: index of node to delete
 *
 * Return: 1 on success, 0 on failure
 */
int delt_nd_at_indx(ls_t **head, unsigned int index)
{
	ls_t *node, *prev_node;
	unsigned int i = 0;

	if (!head || !*head)
		return (0);

	if (!index)
	{
		node = *head;
		*head = (*head)->next;
		free(node->str);
		free(node);
		return (1);
	}
	node = *head;
	while (node)
	{
		if (i == index)
		{
			prev_node->next = node->next;
			free(node->str);
			free(node);
			return (1);
		}
		i++;
		prev_node = node;
		node = node->next;
	}
	return (0);
}

/**
 * free_list - frees all nodes of a list
 * @head_ptr: address of pointer to head node
 *
 * Return: void
 */
void free_list(ls_t **head_ptr)
{
	ls_t *node, *next_node, *head;

	if (!head_ptr || !*head_ptr)
		return;
	head = *head_ptr;
	node = head;
	while (node)
	{
		next_node = node->next;
		free(node->str);
		free(node);
		node = next_node;
	}
	*head_ptr = NULL;
}
