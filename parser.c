#include "shell.h"

/**
 * is_executable_command - determines if a file is an executable command
 * @info: Pointer to the info struct
 * @path: Path to the file
 *
 * Return: 1 if it's an executable command, 0 otherwise
 */
int is_executable_command(info_t *info, char *path)
{
    struct stat file_stat;

    (void)info;
    if (!path || stat(path, &file_stat) != 0)
        return (0);

    if (S_ISREG(file_stat.st_mode))
    {
        return (1);
    }
    return (0);
}

/**
 * duplicate_characters - duplicates characters from a string
 * @path_string: The PATH string
 * @start_index: Starting index
 * @stop_index: Stopping index
 *
 * Return: Pointer to a new buffer
 */
char *duplicate_characters(char *path_string, int start_index, int stop_index)
{
    static char buffer[1024];
    int i = 0, k = 0;

    for (k = 0, i = start_index; i < stop_index; i++)
        if (path_string[i] != ':')
            buffer[k++] = path_string[i];
    buffer[k] = '\0';
    return (buffer);
}

/**
 * find_command_path - finds the full path of a command in the PATH string
 * @info: Pointer to the info struct
 * @path_string: The PATH string
 * @command: The command to find
 *
 * Return: Full path of the command if found, or NULL
 */
char *find_command_path(info_t *info, char *path_string, char *command)
{
    int i = 0, current_position = 0;
    char *path;

    if (!path_string)
        return (NULL);
    if ((_strlen(command) > 2) && starts_with(command, "./"))
    {
        if (is_executable_command(info, command))
            return (command);
    }
    while (1)
    {
        if (!path_string[i] || path_string[i] == ':')
        {
            path = duplicate_characters(path_string, current_position, i);
            if (!*path)
                _strcat(path, command);
            else
            {
                _strcat(path, "/");
                _strcat(path, command);
            }
            if (is_executable_command(info, path))
                return (path);
            if (!path_string[i])
                break;
            current_position = i;
        }
        i++;
    }
    return (NULL);
}
