/*
 * 2023 Robert Papagna
 *
 * https://github.com/rpeepz/File2Image
 * 
 */

#include "File2Image.h"

const char signature[] =
{
	0xFF, 0x28, 0x64,
	0xFF, 0x50, 0x64,
	0xFF, 0x78, 0x64,
	0xFF, 0xA0, 0x64,
	0xFF, 0xC8, 0x64
};

static size_t check_filename(void)
{
	size_t len;
	char *last_separator;
	
	// Extract Filename from given argument
	last_separator = strrchr(f2i.filename, '/');
	if (last_separator != NULL)
		f2i.filename = last_separator + 1;
	// Check Length of Filename
	len = strlen(f2i.filename);
	if (len > (HEADERSIZE - sizeof(f2i.filesize) - sizeof(signature)))
	{
		abort_("[check_filename] Input file name too long");
		complete(NULL, NULL, NULL, 0, 0, 1);
	}
	return (len);
}

void header_encode(void)
{
	size_t len = check_filename();
	bzero(f2i.header, HEADERSIZE);
	memcpy(f2i.header, signature, sizeof(signature));
	memcpy(f2i.header + sizeof(signature), f2i.filename, len);
	memcpy(f2i.header + (HEADERSIZE - sizeof(f2i.filesize)), (void*)&f2i.filesize, sizeof(f2i.filesize));
}

char *header_decode(png_bytepp image)
{
	// copy header data
	bzero(f2i.header, HEADERSIZE);
	memcpy(f2i.header, *image, HEADERSIZE);
	if (memcmp(*image, signature, sizeof(signature)))
		return NULL;

	// get file name from header
	char *file_name = malloc(HEADERSIZE - sizeof(f2i.filesize) - sizeof(signature));
	if (!file_name)
	{
		abort_("[header_decode] Malloc failed");
		complete(image, NULL, NULL, f2i._height - 1, 0, 1);
	}
	bzero(file_name, sizeof(*file_name));
	memcpy(file_name, f2i.header + sizeof(signature), HEADERSIZE - sizeof(f2i.filesize) - sizeof(signature));

	// get filesize from header
	uint8_t *ptr = (uint8_t*)f2i.header+(HEADERSIZE - sizeof(f2i.filesize));
	off_t retrieve;
	for (int i = 0; i < 8; i++)
		memcpy((char*)(&retrieve)+i, &ptr[i], 1);
	f2i.filesize = retrieve;
	return file_name;
}
