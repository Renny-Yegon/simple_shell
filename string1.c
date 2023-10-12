#include "shell.h"

/**
 * custom_strcpy - copies a string
 * @destination: The destination
 * @source: The source
 *
 * Return: Pointer to destination
 */
char *custom_strcpy(char *destination, char *source)
{
    int index = 0;

    if (destination == source || source == NULL)
        return destination;

    while (source[index])
    {
        destination[index] = source[index];
        index++;
    }

    destination[index] = '\0';
    return destination;
}

/**
 * custom_strdup - duplicates a string
 * @str: The string to duplicate
 *
 * Return: Pointer to the duplicated string
 */
char *custom_strdup(const char *str)
{
    int length = 0;
    char *result;

    if (str == NULL)
        return NULL;

    while (*str++)
        length++;

    result = malloc(sizeof(char) * (length + 1));
    if (!result)
        return NULL;

    for (length++; length--;)
        result[length] = *--str;

    return result;
}

/**
 * custom_puts - prints an input string
 * @str: The string to be printed
 *
 * Return: Nothing
 */
void custom_puts(char *str)
{
    int index = 0;

    if (!str)
        return;

    while (str[index] != '\0')
    {
        custom_putchar(str[index]);
        index++;
    }
}

/**
 * custom_putchar - writes the character c to stdout
 * @character: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int custom_putchar(char character)
{
    static int buffer_index;
    static char buffer[WRITE_BUFFER_SIZE];

    if (character == BUFFER_FLUSH || buffer_index >= WRITE_BUFFER_SIZE)
    {
        write(1, buffer, buffer_index);
        buffer_index = 0;
    }

    if (character != BUFFER_FLUSH)
        buffer[buffer_index++] = character;

    return 1;
}

