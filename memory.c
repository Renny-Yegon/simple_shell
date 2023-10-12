#include "shell.h"

/**
 * custom_free - Frees a pointer and sets its address to NULL
 * @pointer: Address of the pointer to free
 *
 * Return: 1 if freed, otherwise 0.
 */
int custom_free(void **pointer)
{
    if (pointer && *pointer)
    {
        free(*pointer);
        *pointer = NULL;
        return (1);
    }
    return (0);
}

