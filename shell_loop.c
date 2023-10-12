#include "shell.h"

/**
 * shell_loop - Main shell loop
 * @info: Pointer to the parameter and return info struct
 * @args: Argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int shell_loop(info_t *info, char **args)
{
    ssize_t input_result = 0;
    int builtin_result = 0;

    while (input_result != -1 && builtin_result != -2)
    {
        clear_info(info);
        if (interactive(info))
            _puts("$ ");
        _eputchar(BUF_FLUSH);
        input_result = get_input(info);
        if (input_result != -1)
        {
            set_info(info, args);
            builtin_result = find_builtin_command(info);
            if (builtin_result == -1)
                find_executable_command(info);
        }
        else if (interactive(info))
            _putchar('\n');
        free_info(info, 0);
    }
    write_history(info);
    free_info(info, 1);
    if (!interactive(info) && info->status)
        exit(info->status);
    if (builtin_result == -2)
    {
        if (info->error_code == -1)
            exit(info->status);
        exit(info->error_code);
    }
    return (builtin_result);
}

/**
 * find_builtin_command - Finds a builtin command
 * @info: Pointer to the parameter and return info struct
 *
 * Return: -1 if builtin not found,
 *         0 if builtin executed successfully,
 *         1 if builtin found but not successful,
 *         2 if builtin signals exit()
 */
int find_builtin_command(info_t *info)
{
    int i, builtin_result = -1;
    builtin_table builtins[] = {
        {"exit", custom_exit},
        {"env", custom_env},
        {"help", custom_help},
        {"history", custom_history},
        {"setenv", custom_setenv},
        {"unsetenv", custom_unsetenv},
        {"cd", custom_cd},
        {"alias", custom_alias},
        {NULL, NULL}
    };

    for (i = 0; builtins[i].type; i++)
        if (_strcmp(info->argv[0], builtins[i].type) == 0)
        {
            info->line_count++;
            builtin_result = builtins[i].func(info);
            break;
        }
    return (builtin_result);
}

/**
 * find_executable_command - Finds an executable command in PATH
 * @info: Pointer to the parameter and return info struct
 *
 * Return: void
 */
void find_executable_command(info_t *info)
{
    char *path = NULL;
    int i, non_delim_count = 0;

    info->path = info->argv[0];
    if (info->linecount_flag == 1)
    {
        info->line_count++;
        info->linecount_flag = 0;
    }
    for (i = 0; info->arg[i]; i++)
        if (!is_delimiter(info->arg[i], " \t\n"))
            non_delim_count++;
    if (!non_delim_count)
        return;

    path = find_command_path(info, _getenv(info, "PATH="), info->argv[0]);
    if (path)
    {
        info->path = path;
        fork_command(info);
    }
    else
    {
        if ((interactive(info) || _getenv(info, "PATH=")
                    || info->argv[0][0] == '/') && is_executable_command(info, info->argv[0]))
            fork_command(info);
        else if (*(info->arg) != '\n')
        {
            info->status = 127;
            print_error(info, "not found\n");
        }
    }
}

/**
 * fork_command - Forks an exec thread to run a command
 * @info: Pointer to the parameter and return info struct
 *
 * Return: void
 */
void fork_command(info_t *info)
{
    pid_t child_pid;

    child_pid = fork();
    if (child_pid == -1)
    {
        perror("Error:");
        return;
    }
    if (child_pid == 0)
    {
        if (execve(info->path, info->argv, get_environment(info)) == -1)
        {
            free_info(info, 1);
            if (errno == EACCES)
                exit(126);
            exit(1);
        }
    }
    else
    {
        wait(&(info->status));
        if (WIFEXITED(info->status))
        {
            info->status = WEXITSTATUS(info->status);
            if (info->status == 126)
                print_error(info, "Permission denied\n");
        }
    }
}

