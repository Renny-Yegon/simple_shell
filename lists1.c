#include "shell.h"

/**
 * list_len - linked list length of determiner 
 * @r: first node pointer
 *
 * Return: size of list
 */
size_t list_len(const list_t *r)
{
	size_t n = 0;

	while (r)
	{
		r = r->next;
		n++;
	}
	return (n);
}

/**
 * list_to_strings - returns an array of strings of the list->str
 * @head: pointer to first node
 *
 * Return: array of strings
 */
char **list_to_strings(list_t *head)
{
	list_t *node = head;
	size_t r = list_len(head), n;
	char **strs;
	char *str;

	if (!head || !r)
		return (NULL);
	strs = malloc(sizeof(char *) * (r + 1));
	if (!strs)
		return (NULL);
	for (r = 0; node; node = node->next, r++)
	{
		str = malloc(_strlen(node->str) + 1);
		if (!str)
		{
			for (n = 0; n < r; n++)
				free(strs[n]);
			free(strs);
			return (NULL);
		}

		str = _strcpy(str, node->str);
		strs[r] = str;
	}
	strs[r] = NULL;
	return (strs);
}

/**
 * print_linked_list - prints all elements of a linked list_t list
 * @head: pointer to the first node
 *
 * Return: number of nodes in the list
 */
size_t print_linked_list(const list_t *head)
{
	size_t count = 0;

	while (head)
	{
		_puts(convert_to_string(head->value, 10, 0));
		_putchar(':');
		_putchar(' ');
		_puts(head->data ? head->data : "(nil)");
		_puts("\n");
		head = head->next;
		count++;
	}
	return (count);
}



/**
 * find_node_with_prefix - returns a node whose string starts with a given prefix
 * @start_node: pointer to the list head
 * @target_prefix: string to match as a prefix
 * @next_char: the next character after the prefix to match
 *
 * Return: the matching node or NULL if not found
 */
list_t *find_node_with_prefix(list_t *start_node, char *target_prefix, char next_char)
{
	char *prefix_check = NULL;

	while (start_node)
	{
		prefix_check = starts_with(start_node->str, target_prefix);
		if (prefix_check && ((next_char == -1) || (*prefix_check == next_char)))
			return (start_node);
		start_node = start_node->next;
	}
	return (NULL);
}


/**
 * find_node_index - finds the index of a specific node
 * @start_node: pointer to the list head
 * @target_node: pointer to the node to find
 *
 * Return: index of the node or -1 if not found
 */
ssize_t find_node_index(list_t *start_node, list_t *target_node)
{
	size_t index = 0;

	while (start_node)
	{
		if (start_node == target_node)
			return (index);
		start_node = start_node->next;
		index++;
	}
	return (-1);
}
