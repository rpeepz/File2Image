/*
 * 2023 Robert Papagna
 *
 * https://github.com/rpeepz/File2Image
 * 
 */

#include "File2Image.h"
int type = FILE_TO_IMAGE;

static void usage(void)
{
	if (type == INFO)
	{
		printf("File2Image %s\n\n", VERSION);
		printf(DETAILS);
		printf("\n");
	}
	printf(USAGE);
	_exit(0);
}

static void option(char *option, bool *success)
{
	if (option[0] == '-')
	{
		if (option[1] == 'e')
		{
			type = FILE_TO_IMAGE;
			*success = true;
		}
		else if (option[1] == 'd')
		{
			type = IMAGE_TO_FILE;
			*success = true;
		}
		else if (option[1] == 'c')
		{
			type = IMAGE_TO_VIDEO;
			*success = true;
		}
		else if (option[1] == 'm')
		{
			type = VIDEO_TO_IMAGE;
			*success = true;
		}
		else if (option[1] == 't')
		{
			type = FILE_TO_VIDEO;
			// *success = true;
		}
		else if (option[1] == 'r')
		{
			type = VIDEO_TO_FILE;
			// *success = true;
		}
		else if (option[1] == 'v')
		{
			type = INFO;
		}
	}
}

int main (int ac, char **av)
{
	bool option_success = false;

	if (ac < 2 || ac > 3)
		usage();

	if (ac == 2 || ac == 3)
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

	switch (type)
	{
		case FILE_TO_IMAGE:
			encode();
			break;
		case IMAGE_TO_FILE:
			decode();
			break;
		case IMAGE_TO_VIDEO:
			convert_image_to_video();
			break;
		case VIDEO_TO_IMAGE:
			map_video_to_image();
			break;
		case VIDEO_TO_FILE:
			;
			break;
		default:
			break;
	}

	close(f2i.fd);
	return 0;
}
