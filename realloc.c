#include "shell.h"

/**
 * custom_memset - fills memory with a constant byte
 * @dest: The pointer to the memory area
 * @byte: The byte to fill @dest with
 * @count: The number of bytes to be filled
 *
 * Return: A pointer to the memory area @dest
 */
char *custom_memset(char *dest, char byte, unsigned int count)
{
    unsigned int i;

    for (i = 0; i < count; i++)
        dest[i] = byte;
    return (dest);
}

/**
 * free_string_array - frees an array of strings
 * @str_array: The array of strings
 */
void free_string_array(char **str_array)
{
    char **ptr = str_array;

    if (!str_array)
        return;
    while (*str_array)
        free(*str_array++);
    free(ptr);
}

/**
 * custom_realloc - reallocates a block of memory
 * @ptr: Pointer to the previous malloc'ed block
 * @old_size: Byte size of the previous block
 * @new_size: Byte size of the new block
 *
 * Return: Pointer to the reallocated block of memory
 */
void *custom_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
    char *new_ptr;

    if (!ptr)
        return (malloc(new_size));
    if (!new_size)
        return (free(ptr), NULL);
    if (new_size == old_size)
        return (ptr);

    new_ptr = malloc(new_size);
    if (!new_ptr)
        return (NULL);

    old_size = old_size < new_size ? old_size : new_size;
    while (old_size--)
        new_ptr[old_size] = ((char *)ptr)[old_size];
    free(ptr);
    return (new_ptr);
}

