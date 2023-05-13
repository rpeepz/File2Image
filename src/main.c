/*
 * 2023 Robert Papagna
 *
 * https://github.com/rpeepz/File2Image
 * 
 */

#include "File2Image.h"
int type = FILE_TO_IMAGE;
static void option(char *option, bool *success);
static void dispatch(void);

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

int main (int ac, char **av)
{
	bool option_success = false;

	if (ac < 2 || ac > 3)
		usage();

	if (ac == 2 || ac == 3)
	{
		if (av[1][0] == '-')
			option(av[1], &option_success);
		if (!option_success)
		{
			usage();
		}
	}

	f2i.filename = av[ac - 1];
	open_file();
	get_filesize();
	dispatch();
	close(f2i.fd);
	return 0;
}

static void option(char *option, bool *success)
{
	switch (option[1])
	{
    case 'e':
        type = FILE_TO_IMAGE;
        *success = true;
        break;
    case 'd':
        type = IMAGE_TO_FILE;
        *success = true;
        break;
    case 'c':
        type = IMAGE_TO_VIDEO;
        *success = true;
        break;
    case 'm':
        type = VIDEO_TO_IMAGE;
        *success = true;
        break;
    case 't':
        type = FILE_TO_VIDEO;
        // *success = true;
        break;
    case 'r':
        type = VIDEO_TO_FILE;
        // *success = true;
        break;
    case 'v':
        type = INFO;
        break;
	}
}

static void dispatch(void)
{
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
		case FILE_TO_VIDEO:
			transform_file_to_video();
			break;
		case VIDEO_TO_FILE:
			revert_video_to_file();
			break;
		default:
			break;
	}
}
