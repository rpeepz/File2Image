/*
 * 2023 Robert Papagna
 *
 * https://github.com/rpeepz/File2Image
 * 
 */

#include "File2Image.h"

png_byte **read_png(void)
{
	png_byte png_header[8];

	// Open the PNG file
	FILE *fp = fopen(f2i.filename, "rb");
	if (!fp)
	{
		abort_("[read_png] %s: could not be opened for reading", f2i.filename);
		complete(NULL, NULL, NULL, 0, 0, 1);
	}

	// Check if it's a PNG file
	fread(png_header, 1, 8, fp);
	if (png_sig_cmp(png_header, 0, 8))
	{
		abort_("[read_png] %s: is not recognized as a PNG file", f2i.filename);
		complete(NULL, NULL, fp, 0, 0, 1);
	}

	// Create the PNG structures
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		abort_("[read_png] png_create_read_struct failed");
		complete(NULL, NULL, fp, 0, 0, 1);
	}
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		abort_("[read_png] png_create_info_struct failed");
		complete(NULL, NULL, fp, 0, 0, 1);
	}
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		abort_("[read_png] Error during init_io");
		complete(NULL, NULL, fp, 0, 0, 1);
	}

	// Initialize the PNG IO
	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);

	// Read the PNG info
	png_read_info(png_ptr, info_ptr);

	// Get the PNG image attributes
	int width = png_get_image_width(png_ptr, info_ptr);
	int height = png_get_image_height(png_ptr, info_ptr);
	png_byte color_type = png_get_color_type(png_ptr, info_ptr);
	// png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr); //Unused


//	The following commented code are excess processes
//	if PNG output is standard, none of this is needed

/*
 *	if (bit_depth == 16)
 *		png_set_strip_16(png_ptr);
*/

/*
 *	if (color_type == PNG_COLOR_TYPE_PALETTE)
 *		png_set_palette_to_rgb(png_ptr);
*/

/*
 *	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
 *		png_set_expand_gray_1_2_4_to_8(png_ptr);
*/

/*
 *	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
 *		png_set_tRNS_to_alpha(png_ptr);
*/

	if (color_type == PNG_COLOR_TYPE_RGB ||
		color_type == PNG_COLOR_TYPE_GRAY ||
		color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);

/*
 *	if (color_type == PNG_COLOR_TYPE_GRAY ||
 *		color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
 *		png_set_gray_to_rgb(png_ptr);
*/

	png_read_update_info(png_ptr, info_ptr);

	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
	// int channels = png_get_channels(png_ptr, info_ptr); //Unused

	// Store png info in row_pointers
	png_bytepp row_pointers = (png_byte**) malloc(sizeof(png_bytep) * height);
	if (!row_pointers)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		abort_("[read_png] Malloc Failed");
		complete(NULL, NULL, fp, 0, 0, 1);
	}
	for (int y = 0; y < height; y++)
	{
		row_pointers[y] = (png_byte*) malloc(rowbytes);
		if (!row_pointers[y])
		{
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
			abort_("[read_png] Malloc Failed");
			complete(row_pointers, NULL, fp, y - 1, 0, 1);
		}
	}
	png_read_image(png_ptr, row_pointers);

	// Extract RGB values from row_pointers and store in image
	png_bytepp image = (png_byte**) malloc(sizeof(png_bytep) * height);
	if (!image)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		abort_("[read_png] Malloc Failed");
		complete(row_pointers, NULL, fp, height - 1, 0, 1);
	}
	for (int y = 0; y < height; y++)
	{
		image[y] = (png_byte*) malloc(width * 3);
		if (!image[y])
		{
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
			abort_("[read_png] Malloc Failed");
			complete(row_pointers, image, fp, height - 1, y - 1, 1);
		}
	}

	for (int y = 0; y < height; y++)
	{
		png_bytep row = row_pointers[y];
		for (int x = 0; x < width; x++)
		{
			memcpy(image[y] + (x * 3), &(row[x * 4]), 3);
		}
	}
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	fclose(fp);
	for (int y = 0; y < height; y++)
	{
		free(row_pointers[y]);
		row_pointers[y] = NULL;
	}
	free(row_pointers);
	row_pointers = NULL;
	f2i._width = width;
	f2i._height = height;
	return image;
}
