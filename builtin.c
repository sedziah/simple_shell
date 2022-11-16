#include "shell.h"

/**
 * cd_s - Changes the current working directory to the parameter passed to cd.
 * if no parameter is passed it will change directory to HOME.
 * @line: A string representing the input from the user.
 */
void cd_s(char *line)
{
  int index;
  int token_count;
  char **param_array;
  const char *delim = "\n\t ";

  token_count = 0;
  param_array = token_interface(line, delim, token_count);
  if (param_array[0] == NULL)
    {
      single_free(2, param_array, line);
      return;
    }
  if (param_array[1] == NULL)
    {
      index = find_path("HOME");
      chdir((environ[index]) + 5);
    }
  else if (_strcmp(param_array[1], "-") == 0)
    print_str(param_array[1], 0);

  else
    chdir(param_array[1]);
  double_free(param_array);
}

/**
 * env_s - Prints all the environmental variables in the current shell.
 * @line: A string representing the input from the user.
 */
void env_s(__attribute__((unused))char *line)
{
  int i;
  int j;

  i = 0;
  while (environ[i] != NULL)
    {
      for (j = 0; environ[i][j] != '\0'; j++)
	write(STDOUT_FILENO, &environ[i][j], 1);
      write(STDOUT_FILENO, "\n", 1);
      i++;
    }
}

/**
 * exit_s - Exits the shell. After freeing allocated resources.
 * @line: A string representing the input from the user.
 */
void exit_s(char *line)
{
  free(line);
  print_str("\n", 1);
  exit(1);
}

/**
 * check_built_ins - Finds the right function needed for execution.
 * @str: The name of the function that is needed.
 * Return: Upon sucess a pointer to a void function. Otherwise NULL.
 */
void (*check_built_ins(char *str))(char *str)
{
  int i;

  builtin_t buildin[] = {
			 {"exit", exit_s},
			 {"env", env_s},
			 {"cd", cd_s},
			 {NULL, NULL}
  };

  for (i = 0; buildin[i].built != NULL; i++)
    {
      if (_strcmp(str, buildin[i].built) == 0)
	{
	  return (buildin[i].fun);
	}
    }
  return (NULL);
}

/**
 * built_in - Checks for builtin functions.
 * @cmd: An array of all the arguments passed to the shell.
 * @line: A string representing the input from the user.
 * Return: If function is found 0. Otherwise -1.
 */
int built_in(char **cmd, char *line)
{
  void (*build)(char *);

  build = check_built_ins(cmd[0]);
  if (build == NULL)
    return (-1);
  if (_strcmp("exit", cmd[0]) == 0)
    double_free(cmd);
  build(line);
  return (0);
}
