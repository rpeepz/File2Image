/*
 * 2023 Robert Papagna
 *
 * https://github.com/rpeepz/File2Image
 * 
 */

#include "File2Image.h"

void get_filesize(void)
{
	struct stat fileStat;
	if (fstat(f2i.fd, &fileStat) < 0)
	{
		abort_("[get_filesize] %s: fstat failed", f2i.filename);
		complete(NULL, NULL, NULL, 0, 0, 1);
	}
	if (!S_ISREG(fileStat.st_mode) || S_ISDIR(fileStat.st_mode))
	{
		abort_("[get_filesize] %s: is %s", f2i.filename, S_ISDIR(fileStat.st_mode) ? "a directory" : "not a regular file" );
		complete(NULL, NULL, NULL, 0, 0, 1);
  }
	f2i.filesize = fileStat.st_size;
	f2i.filemode = fileStat.st_mode;
}

void open_file(void)
{
	f2i.fd = open(f2i.filename, O_RDONLY);
	if (f2i.fd == -1)
	{
		if (errno == ENOENT)
			abort_("[open_file] %s: file does not exist", f2i.filename);
		else
			abort_("[open_file] %s: fopen failed", f2i.filename);
		_exit(1);
	}
}
