/*
 * 2023 Robert Papagna
 *
 * https://github.com/rpeepz/File2Image
 * 
 */

#ifndef _FILE2IMAGE_H_
# define _FILE2IMAGE_H_

# define VERSION "v1.1.1"

# include <unistd.h>
# include <errno.h>
# include <stdio.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/stat.h>
# include <string.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdlib.h>
# include <stdarg.h>
# include <png.h>

# define _RES_CRAP	20  // 320 x 180
# define _RES_SD		40  // 360p
# define _RES_HD		80  // 720p
# define _RES_FHD		120 // 1080p
# define _RES_QHD		160 // 1440p
# define _RES_UHD		240 // 4k
# define RESOLUTION_SCALE _RES_SD // USED RESOLTION
# define IMAGE_WIDTH	16 * RESOLUTION_SCALE
# define IMAGE_HEIGHT	9 * RESOLUTION_SCALE
// TODO
// side note: when changing this value, ensure the width and height will support the scale
/* when modifying pixel scale, ensure encoder/decoder follows the scale
*
* v1.1 - only `-e` will work when this value is changed
*/
# define PIXEL_SCALE	1
# define _PIXEL_SIZE	((PIXEL_SCALE <= 1) ? 1 : ipow(2, PIXEL_SCALE))
# define PIXEL_COUNT	IMAGE_WIDTH*IMAGE_HEIGHT/_PIXEL_SIZE
# define HEADERSIZE		144

# define INFO						69
# define FILE_TO_IMAGE	1 // -e
# define IMAGE_TO_FILE	2 // -d
# define IMAGE_TO_VIDEO	3 // -c
# define VIDEO_TO_IMAGE	4 // -m
# define FILE_TO_VIDEO	5 // -t
# define VIDEO_TO_FILE	6 // -r
# define USAGE "usage: $> File2Image [ -e | -d | -c | -m | -t | -r ] [ \033[4mfile\033[0m ]\n"
# define DETAILS "-e : ENCODE a \033[4mfile\033[0m to f2i Image file\n-d : DECODE a \033[4mf2i Image file\033[0m to its original file\n-c : CONVERT a \033[4mf2i Image file\033[0m to f2i Video file\n-m : MAP a \033[4mf2i Video file\033[0m to f2i Image file\n-t : TRANSFORM a \033[4mfile\033[0m to f2i Video file\n-r : REVERT a \033[4mf2i Video file\033[0m to its original file\n-v : Version Info\n"

// unable to upload to youtube
# define F2I_VIDEO_ENCODE_OPTIONS "-c:v libx264rgb -preset ultrafast -crf 0 -pix_fmt rgb24"

int				ipow(int base, int exp);
void			open_file(void);
void			get_filesize(void);
void			create_png(png_bytepp image, char *image_name);
void			convert_image_to_video(void);
void			map_video_to_image(void);
void			transform_file_to_video(void);
void			revert_video_to_file(void);
png_byte		**read_png(void);
void			header_encode(void);
char			*header_decode(png_bytepp image);
void			encode(void);
void			decode(void);
void			complete(png_bytepp image1, png_bytepp image2, FILE *fp, int y1, int y2, int ret);
void			abort_(const char *s, ...);

typedef struct	s_file2image {
	int			fd;
	char		*filename;
	off_t		filesize;
	mode_t	filemode;
	uint8_t		header[HEADERSIZE];
	int			_width;
	int			_height;
	char		author[15];
}				t_file2image;

t_file2image	f2i;
#endif
