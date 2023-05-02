/*
 * 2023 Robert Papagna
 *
 * https://github.com/rpeepz/File2Image
 * 
 */

#include "File2Image.h"
int type = ENCODE;

static void usage(void)
{
	printf(USAGE);
	_exit(0);
}

static void option(char *option, bool *success)
{
	if (option[0] == '-')
	{
		if (option[1] == 'e')
		{
			*success = true;
		}
		else if (option[1] == 'd')
		{
			type = DECODE;
			*success = true;
		}
	}
}

int main (int ac, char **av)
{
	bool option_success = false;

	if (ac < 2 || ac > 3)
		usage();

	if (ac == 3)
	{
		option(av[1], &option_success);
		if (!option_success)
		{
			usage();
		}
	}

	f2i.filename = av[ac - 1];
	open_file();
	get_filesize();

	if (type == ENCODE)
	{
		encode();
	}
	else if (type == DECODE)
	{
		decode();
	}

	close(f2i.fd);
	return 0;
}
