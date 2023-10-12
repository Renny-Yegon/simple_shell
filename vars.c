#include "shell.h"

/**
 * is_chain_delimiter - test if the current character in the buffer is a chain delimiter
 * @info: the parameter struct
 * @buffer: the character buffer
 * @position: address of the current position in the buffer
 *
 * Return: 1 if a chain delimiter, 0 otherwise
 */
int is_chain_delimiter(info_t *info, char *buffer, size_t *position)
{
    size_t j = *position;

    if (buffer[j] == '|' && buffer[j + 1] == '|')
    {
        buffer[j] = 0;
        j++;
        info->command_buffer_type = CMD_OR;
    }
    else if (buffer[j] == '&' && buffer[j + 1] == '&')
    {
        buffer[j] = 0;
        j++;
        info->command_buffer_type = CMD_AND;
    }
    else if (buffer[j] == ';') /* found end of this command */
    {
        buffer[j] = 0; /* replace semicolon with null */
        info->command_buffer_type = CMD_CHAIN;
    }
    else
        return 0;

    *position = j;
    return 1;
}

/**
 * check_chain_continuation - checks if we should continue chaining based on the last status
 * @info: the parameter struct
 * @buffer: the character buffer
 * @position: address of the current position in the buffer
 * @start: starting position in the buffer
 * @length: length of the buffer
 *
 * Return: Void
 */
void check_chain_continuation(info_t *info, char *buffer, size_t *position, size_t start, size_t length)
{
    size_t j = *position;

    if (info->command_buffer_type == CMD_AND)
    {
        if (info->status)
        {
            buffer[start] = 0;
            j = length;
        }
    }
    if (info->command_buffer_type == CMD_OR)
    {
        if (!info->status)
        {
            buffer[start] = 0;
            j = length;
        }
    }

    *position = j;
}

/**
 * replace_aliases - replaces aliases in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_aliases(info_t *info)
{
    int i;
    list_t *node;
    char *p;

    for (i = 0; i < 10; i++)
    {
        node = node_starts_with(info->alias_list, info->arguments[0], '=');
        if (!node)
            return 0;
        free(info->arguments[0]);
        p = _strchr(node->string, '=');
        if (!p)
            return 0;
        p = _strdup(p + 1);
        if (!p)
            return 0;
        info->arguments[0] = p;
    }
    return 1;
}

/**
 * replace_variables - replaces variables in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_variables(info_t *info)
{
    int i = 0;
    list_t *node;

    for (i = 0; info->arguments[i]; i++)
    {
        if (info->arguments[i][0] != '$' || !info->arguments[i][1])
            continue;

        if (!_strcmp(info->arguments[i], "$?"))
        {
            replace_string(&(info->arguments[i]), _strdup(convert_number(info->status, 10, 0)));
            continue;
        }
        if (!_strcmp(info->arguments[i], "$$"))
        {
            replace_string(&(info->arguments[i]), _strdup(convert_number(getpid(), 10, 0)));
            continue;
        }
        node = node_starts_with(info->environment, &info->arguments[i][1], '=');
        if (node)
        {
            replace_string(&(info->arguments[i]), _strdup(_strchr(node->string, '=') + 1));
            continue;
        }
        replace_string(&info->arguments[i], _strdup(""));
    }
    return 0;
}

/**
 * replace_string - replaces a string
 * @old_string: address of the old string
 * @new_string: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_string(char **old_string, char *new_string)
{
    free(*old_string);
    *old_string = new_string;
    return 1;
}








