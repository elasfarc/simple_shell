#include "shell.h"

/**
 * get_cmd_status - Get the status of a command based on its path.
 * @path: The path to the command.
 *
 * Description: checks whether a command is executable,
 * a directory, or not executable.
 *
 * Return: An enum CmdExecStatus indicating the status of the command.
 */
enum CmdExecStatus get_cmd_status(char *path)
{
	struct stat st;

	if (access(path, X_OK) == 0)
	{
		if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
			return (IS_EXEC_DIRECTORY);

		else
			return (IS_EXECUTABLE);
	}
	return (IS_NOT_EXECUTABLE);
}

/**
 * resolve_cmd_in_path - Resolve the full path of a command using
 * PATH environment variable.
 * @cmd: The command to resolve.
 *
 * Return: A dynamically allocated string containing
 * the full path of the command if found, or NULL if not found.
 */
char *resolve_cmd_in_path(char *cmd)
{
	char **paths, *path;
	int i, is_path = 0;

	path = _strcat(_get_env("PWD"), "/", cmd, NULL);
	if (access(path, X_OK) == 0)
		return (path);
	safe_free(path);

	paths = get_env_paths();
	for (i = 0; (paths[i] != NULL && !is_path); i++)
	{
		paths[i] = _strcat(paths[i], "/", cmd, NULL);
		if (access(paths[i], X_OK) == 0)
			is_path = 1;
	}

	path = is_path ? _strdup(paths[i - 1]) : NULL;
	free_string_array(paths, NULL);
	return (path);
}

/**
 * get_path - Get the full path of a command based on its name.
 * @cmd: The command to find the path for.
 *
 * Return: A dynamically allocated string containing the full path of
 * the command if it's executable, or NULL on error.
 *
 * Description:
 * - determines the full path of a command based on its name
 * and whether it should be looked up in the PATH variable.
 * - In case of an error, errno will be set.
 */
char *get_path(char *cmd)
{
	char *path;
	enum CmdExecStatus status;
	int skip_path_lookup = (cmd[0] == '/' || cmd[0] == '.');

	path = skip_path_lookup
		? _strdup(cmd)
		: resolve_cmd_in_path(cmd);

	if (!path)
		path = _strdup(cmd);

	status = get_cmd_status(path);
	if (status == IS_EXECUTABLE)
		return (path);

	safe_free(path);
	if (status == IS_EXEC_DIRECTORY)
	{
		handle_error(cmd, 1, "Is a directory");
		errno = 126;
	}
	else if (status == IS_NOT_EXECUTABLE && errno == ENOENT)
	{
		if (!charInString('/', cmd))
			handle_error(cmd, 1, "command not found");
		else
			handle_error(cmd, 0);
		errno = 127;
	}
	else
	{
		handle_error(cmd, 0);
		errno = 126;
	}


	return (NULL);
}
