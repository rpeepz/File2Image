/*
 * 2023 Robert Papagna
 *
 * https://github.com/rpeepz/File2Image
 * 
 */

#include "File2Image.h"

void create_png(png_bytepp image, char *image_name)
{
	FILE *fp = fopen(image_name, "wb");
	if (!fp)
	{
		abort_("[create_png] %s: fopen failed", image_name);
		complete(image, NULL, NULL, IMAGE_HEIGHT - 1, 0, 1);
	}

	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		abort_("[create_png] png_create_read_struct failed");
		complete(image, NULL, fp, IMAGE_HEIGHT - 1, 0, 1);
	}
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png_destroy_write_struct(&png_ptr, NULL);
		abort_("[read_png_file] png_create_info_struct failed");
		complete(image, NULL, fp, IMAGE_HEIGHT - 1, 0, 1);
	}
	png_init_io(png_ptr, fp);
	png_set_IHDR(png_ptr, info_ptr, IMAGE_WIDTH, IMAGE_HEIGHT, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png_ptr, info_ptr);
	png_write_image(png_ptr, image);
	png_write_end(png_ptr, NULL);

	png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(fp);
}
