/*
 * 2023 Robert Papagna
 *
 * https://github.com/rpeepz/File2Image
 * 
 */

#include "File2Image.h"

// TODO
// FOUND BUG WHEN USING UHD RESOLUTION
void encode(void)
{
	int y;
	header_encode();
	off_t pixelsNeeded = (f2i.filesize / (off_t)3) + !!(f2i.filesize % (off_t)3) + HEADERSIZE;

	// RGB values from each byte of data read
	png_byte pixels[PIXEL_COUNT][3];

	png_bytepp image = (png_byte**) malloc(sizeof(png_bytep) * IMAGE_HEIGHT);
	if (!image)
	{
		abort_("[encode] Malloc Failed");
		complete(NULL, NULL, NULL, 0, 0, 1);
	}
	for (y = 0; y < IMAGE_HEIGHT; y++)
	{
		image[y] = (png_byte*) malloc(sizeof(png_byte) * IMAGE_WIDTH * 3);
		if (!image[y])
		{
			abort_("[encode] Malloc Failed");
			complete(image, NULL, NULL, y - 1, 0, 1);
		}
	}
	int loops = 0;
	char image_name[16];
	while (pixelsNeeded > 0)
	{
		bzero(pixels, sizeof(pixels));
		for (y = 0; y < IMAGE_HEIGHT; y++)
			bzero(image[y], IMAGE_WIDTH * 3);
		//first loop only, assign header info to pixels array
		if (!loops)
		{
			memcpy(pixels, f2i.header, HEADERSIZE);
			read(f2i.fd, pixels + (HEADERSIZE / 3), sizeof(pixels) - HEADERSIZE);
		}
		else
		{
			read(f2i.fd, pixels, sizeof(pixels));
		}
		int x = 0; //position in image array
		y = 0;
		for (int i = 0; i < PIXEL_COUNT; i++)
		{
			for (int row = 0; row < PIXEL_SCALE; row++){
				for (int col = 0; col < PIXEL_SCALE; col++)
				{
					image[y + row][((x + col) * 3) + 0] = pixels[i][0];
					image[y + row][((x + col) * 3) + 1] = pixels[i][1];
					image[y + row][((x + col) * 3) + 2] = pixels[i][2];
				}
			}
			x += PIXEL_SCALE;
			if (x >= IMAGE_WIDTH)
			{
				x = 0;
				y += PIXEL_SCALE;
			}
		}
		pixelsNeeded -= PIXEL_COUNT;
		loops++;
		snprintf(image_name, 16, "f2i%d.png", loops);

		create_png(image, image_name);
		printf("Success: created %s\n", image_name);
	}
	for (y = 0; y < IMAGE_HEIGHT; y++)
	{
		free(image[y]);
		image[y] = NULL;
	}
	free(image);
	image = NULL;
}

static int get_fd(char **file_name, png_bytepp image)
{
	int fd = open(*file_name, O_WRONLY | O_CREAT | O_APPEND | O_EXCL, f2i.filemode);
	if (fd == -1)
	{
		int error = errno;
		if (!strcmp(strerror(error), "File exists"))
		{
			// TODO overwrite or make a copy, currenty only makes a copy
			printf("[%s]: %s. Overwrite? [y / n]: ", *file_name, strerror(error));
	    fflush(stdout);
			int buf = getchar();
			if (buf == 'y' || buf == 'Y')
			{
				printf("Creating a copy\n");
				size_t length = strlen(*file_name);
				size_t new_length = length + 7;
				char *new_file = malloc(new_length);
				if (!new_file)
				{
					abort_("[get_fd] Malloc failed");
					complete(image, NULL, NULL, f2i._height - 1, 0, 1);
				}
				bzero(new_file, new_length);
				char *extension = strrchr(*file_name, '.'); // pointer to file ext
				if (!extension)
				{
					snprintf(new_file, new_length, "%s(copy)", *file_name);
				}
				else
				{
					size_t prefix_length = extension - *file_name;
					memcpy(new_file, *file_name, prefix_length);
					memcpy(&new_file[prefix_length], "(copy)", 6);
					memcpy(&new_file[prefix_length + 6], extension, strlen(extension));
				}
				free(*file_name);
				*file_name = new_file;
				fd = open(*file_name, O_WRONLY | O_CREAT | O_APPEND | O_EXCL, 0755);
			}
			else
				dprintf(2, "Aborting\n");
		}
		else
			abort_("[get_fd] %s: %s", *file_name, strerror(error));
		return (fd);
	}
	return (fd);
}

void decode(void)
{
	png_byte **image = read_png();
	char *new_file = header_decode(image);
	if (!new_file)
	{
		abort_("[decode] Invalid signature. Please input 'f2i1.png' to decode");
		complete(image, NULL, NULL, f2i._height - 1, 0, 1);
	}
	int fd = get_fd(&new_file, image);
	if (fd != -1)
	{
		if (f2i.filesize < (f2i._width * 3))
			write(fd, *image + HEADERSIZE, f2i.filesize);
		else
		{
			int loops = 0;
			bool multi = false;
			size_t index;
			if (f2i.filesize > (off_t)(f2i._width * f2i._height * 3))
			{
				multi = true;
				size_t len = strlen(f2i.filename);
				char *ptr = strstr(f2i.filename, "f2i1.png");
				index = ptr - f2i.filename + 3;
				char *tmp = (char *)malloc(sizeof(char) * len + 3);
				bzero(tmp, len + 3);
				memcpy(tmp, f2i.filename, len);
				f2i.filename = tmp;
			}
			off_t bytes = write(fd, *image + HEADERSIZE, (f2i._width * 3) - HEADERSIZE);
			while(bytes < f2i.filesize)
			{
				for (int i = !loops ? 1 : 0; i < f2i._height; i++)
				{
					if (f2i.filesize - bytes < f2i._width * 3)
					{
						bytes += write(fd, image[i], f2i.filesize - bytes);
						break ;
					}
					bytes += write(fd, image[i], (f2i._width * 3));
				}
				loops++;
				if (bytes < f2i.filesize && multi == true)
				{
					for (int y = 0; y < f2i._height; y++)
					{
						free(image[y]);
						image[y] = NULL;
					}
					free(image);
					image = NULL;
					snprintf(&f2i.filename[index], strlen(&f2i.filename[index]) + 1, "%d.png", loops + 1);
					image = read_png();
				}
			}
			if (multi == true)
			{
				free(f2i.filename);
				f2i.filename = NULL;	
			}
		}
		close(fd);
		printf("Success: created %s\n", new_file);
	}

	free(new_file);
	new_file = NULL;
	for (int y = 0; y < f2i._height; y++)
	{
		free(image[y]);
		image[y] = NULL;
	}
	free(image);
	image = NULL;
}
