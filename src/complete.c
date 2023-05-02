/*
 * 2023 Robert Papagna
 *
 * https://github.com/rpeepz/File2Image
 * 
 */

#include "../inc/File2Image.h"

void complete(png_bytepp image1, png_bytepp image2, FILE *fp, int y1, int y2, int ret)
{
	if (image1)
	{
		for (; y1 >= 0; y1--)
		{
			free(image1[y1]);
			image1[y1] = NULL;
		}
		free(image1);
		image1 = NULL;
	}
	if (image2)
	{
		for (; y2 >= 0; y2--)
		{
			free(image2[y2]);
			image2[y2] = NULL;
		}
		free(image2);
		image2 = NULL;
	}
	if (fp)
		fclose(fp);
	close(f2i.fd);
	_exit(ret);
}

void abort_(const char *s, ...)
{
	va_list args;
	va_start(args, s);
	vfprintf(stderr, s, args);
	fprintf(stderr, "\n");
	va_end(args);
}
