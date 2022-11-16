#include "shell.h"

/**
 * path_finder - Acts as an interface for functions that will be able to
 * find the full path of a program.
 * @cmd: Represents a command. For example ls, echo, pwd, etc.
 * Return: Upon sucess a string with the full path of the program.
 * for example /bin/ls, /bin/echo, etc. Otherwise returns NULL.
 */
char *path_finder(char *cmd)
{
	char *str = "PATH";
	char *constrcd;
	char **path_tokens;
	int index;
	char *directory;

	index = find_path(str);
	path_tokens = tokenize_path(index, str);
	if (path_tokens == NULL)
		return (NULL);

	directory = search_directories(path_tokens, cmd);
	if (directory == NULL)
	{
		double_free(path_tokens);
		return (NULL);
	}

	constrcd = path_builder(directory, cmd);
	if (constrcd == NULL)
	{
		double_free(path_tokens);
		return (NULL);
	}

	double_free(path_tokens);

	return (constrcd);
}
/**
 * path_builder - Finds the index of an environmental variable.
 * @str: Environmental variable that needs to be found.
 * Return: Upon success returns the index of the environmental variable.
 * otherwise returns -1.
 */
int find_path(char *str)
{
	int i;
	int len;
	int j;

	len = str_len(str);
	i = 0;
	while (environ[i] != NULL)
	{
		for (j = 0; j < len; j++)
		{
			if (environ[i][j] != str[j])
				break;
		}
		if (j == len && environ[i][j] == '=')
			return (i);
		i++;
	}
	return (-1);
}

/**
 * tokenize_path - Separates a string representing paths as an array
 * of strings contining the path directories.
 * @index: Index of the path in the environment variables.
 * @str: string to separate and tokenize.
 * Return: Upon success a NULL terminated array of pointer to strings.
 * Otherwise returns NULL. Note!: Do not forget to free alocated
 * memory on receiving function or when possible.
 */
char **tokenize_path(int index, char *str)
{
	char *env_var;
	int token_count;
	const char *delim = ":\n";
	char **path_tokens;
	int len;

	len = str_len(str);
	token_count = 0;
	/*Moving the pointer len of str plus = sign*/
	env_var = environ[index] + (len + 1);
	path_tokens = token_interface(env_var, delim, token_count);
	if (path_tokens == NULL)
		return (NULL);

	return (path_tokens);
}

/**
 * search_directories - Looks through directories stored in path_tokens for a
 * specific file. aka commmand.
 * @path_tokens: A pointer to an array of strings representing the different
 * paths contained in the PATH environmental varible.
 * @cmd: Represents a command. For example ls, echo, pwd, /bin/ls etc.
 * Return: Upon success a string with the upper most directory containing
 * the command file. Otherwise returns NULL.
 */
char *search_directories(char **path_tokens, char *cmd)
{
	int i, s;
	char *cwd;
	char *buf;
	size_t size;
	struct stat stat_buf;

	buf = NULL;
	size = 0;
	cwd = getcwd(buf, size);
	if (cwd == NULL)
		return (NULL);
	if (cmd[0] == '/')
		cmd = cmd + 1;
	for (i = 0; path_tokens[i] != NULL; i++)
	{
		s = chdir(path_tokens[i]);
		if (s == -1)
		{
			perror("ERROR!");
			return (NULL);
		}
		s = stat(cmd, &stat_buf);
		if (s == 0)
		{
			chdir(cwd);
			free(cwd);
			return (path_tokens[i]);
		}
	}
	chdir(cwd);
	free(cwd);
	return (NULL);
}

/**
 * path_builder - Combines two strings one representing the path directory and
 * one representing the command file.
 * @directory: Represents a directory in the path.
 * @cmd: Represents a file in a directory of the path.
 * Return: Upon success a string representing the full path of a command.
 * Otherwise NULL.
 */
char *path_builder(char *directory, char *cmd)
{
	int i, j;
	int dir_len;
	int cmd_len;
	int len;
	char *built;

	if (directory == NULL || cmd == NULL)
		return (NULL);
	dir_len = str_len(directory) + 1;
	cmd_len = str_len(cmd) + 1;
	len = dir_len + cmd_len;

	built = malloc(sizeof(char) * len);
	if (built == NULL)
		return (NULL);

	i = 0;
	while (i < len)
	{
		for (j = 0; directory[j] != '\0'; j++, i++)
			built[i] = directory[j];
		built[i] = '/';
		i++;
		for (j = 0; cmd[j] != '\0'; j++, i++)
			built[i] = cmd[j];
		i++;
	}
	built[--i] = '\0';
	return (built);
}
