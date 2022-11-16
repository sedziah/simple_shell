#include "main.h"
/**
 * create_file - creates a file
 * @filename: name of the file
 * @text_content: string to write to the file
 *
 * Return: 1 on Success or -1 on failure
 */
int create_file(const char *filename, char *text_content)
{
	int fle, i, wrt;

	if (filename == NULL)
	{
		return (-1);
	}

	fle = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);

	if (fle == -1)
	{
		return (-1);
	}

	if (text_content == NULL)
	{
		text_content = "";
	}

	if (text_content == NULL)
	{
		text_content = "";
	}

	i = 0;
	while (text_content[i] != '\0')
	{
		i++;
	};

	wrt = write(fle, text_content, i);

	if (wrt == -1)
	{
		return (-1);
	}

	close(fle);
	return (1);
}
