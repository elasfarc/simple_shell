#include "shell.h"

/**
 * _setenv - change or add an environment variable.
 * @name: key of the record tobe changed or added.
 * @value: the new value.
 *
 * Return: (1) on success; (0) otherwise.
 */
int _setenv(const char *name, const char *value)
{
	int is_modify_success, is_add_new_success;
	char **env = environ, **env_cpy;

	env_cpy = cpy_env();
	if (!env_cpy)
		return (0);

	is_modify_success = modify_record_in_env(name, value, env_cpy);

	if (!is_modify_success)
		is_add_new_success = add_new_record_to_env(name, value, &env_cpy);

	if (!is_modify_success && !is_add_new_success)
	{
		free_string_array(env_cpy, NULL);
		return (0);
	}

	free_string_array(env, NULL);
	environ = env_cpy;

	return (1);
}

/**
 * _unsetenv - deletes the variable name from the environment.
 *		If name does not exist in the environment,
 *		then the function succeeds,and the environment is unchanged.
 * @name: name of the enviroment variable to be deleted.
 *
 * Return: (1) on success, (0) otherwise.
 */
int _unsetenv(const char *name)
{
	char **env = environ, **new_env, *env_record, *key;
	int i, is_name_in_env = 0;

	for (i = 0; (env[i] != NULL && !is_name_in_env); i++)
	{
		env_record = _strdup(env[i]);
		key = _strtok(env_record, "=");
		if (_are_strs_eql(key, name))
			is_name_in_env = 1;
		safe_free(env_record);
	}

	if (!is_name_in_env)
		return (1);

	i = 0;
	while (env[i])
		i++;
	new_env = malloc((sizeof(char *) * (i - 1)) + sizeof(NULL));
	if (!new_env)
		return (0);
	for (i = 0; env[i]; i++)
	{
		env_record = _strdup(env[i]);
		key = _strtok(env_record, "=");
		if (!(_are_strs_eql(key, name)))
			new_env[i] = _strdup(env[i]);
		safe_free(env_record);
	}

	new_env[i - 1] = NULL;

	free_string_array(env, NULL);
	environ = new_env;

	return (1);
}

/**
 * handle_env_change - parses a command string with arguments
 *	and attempts to modify the environment variables
 *	based on the provided command.
 * @cmd_with_args: The command string with arguments to be processed.
 *
 * Description:
 * - The command string is expected to follow the format:
 *	"setenv VARIABLE_NAME VARIABLE_VALUE" for setting an environment variable,
 *	or "unsetenv VARIABLE_NAME" for unsetting an environment variable.
 * - If the provided command is not in the correct format or the number of
 *       arguments does not match the expected count for the specified command,
 *       an error message is printed to the standard error (stderr) stream.
 * - If the operation to set or unset the environment variable fails, an
 *       error message is also printed to stderr.
 * - The function does not free it's arg.
 *
 * TODO:
 * case when cmd_with_args is null
 * case when argv[0] is not SET or UNSET
 *
 * Return: void.
 */


int handle_env_change(char *cmd_with_args)
{
	#define CHNG_ENV_WRONG_ARGS (8)
	#define CHNG_ENV_FAIL (9)

	char **argv, *cmd, *error;
	unsigned short argc = 0, is_set, is_unset, is_success;

	argv = get_argv(cmd_with_args);
	cmd = argv[0];
	is_set = _are_strs_eql(cmd, "setenv");
	is_unset = _are_strs_eql(cmd, "unsetenv");

	while (argv[argc])
		argc++;

	if ((is_set && argc != 3) || (is_unset && argc != 2))
	{
		error = is_set
			? _strdup("Command syntax: setenv VARIABLE VALUE")
			: _strdup("Command syntax: unsetenv VARIABLE");
		handle_error(cmd, 1, CHNG_ENV_WRONG_ARGS, error);
		safe_free(error);
		free_string_array(argv, NULL);
		return (CHNG_ENV_WRONG_ARGS);
	}

	is_success = is_set
		? _setenv(argv[1], argv[2])
		: _unsetenv(argv[1]);
	if (!is_success)
	{
		error = _strcat(_strdup("ERROR: "), argv[1], " NOT",
				(is_set ? "set" : "unset"));
		handle_error(cmd, 1, CHNG_ENV_FAIL, error);
		safe_free(error);
	}
	free_string_array(argv, NULL);

	return (is_success ? 0 : 1);
}
