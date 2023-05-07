/*
 * 2023 Robert Papagna
 *
 * https://github.com/rpeepz/File2Image
 * 
 */

#ifndef _FILE2IMAGE_H_
# define _FILE2IMAGE_H_

# include <unistd.h>
# include <errno.h>
# include <stdio.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/stat.h>
# include <string.h>
# include <stdbool.h>
# include <libavcodec/avcodec.h>
# include <libavformat/avformat.h>
# include <libswscale/swscale.h>
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
# define PIXEL_SCALE	1 //
# define _PIXEL_SIZE	((PIXEL_SCALE <= 1) ? 1 : ipow(2, PIXEL_SCALE))
# define PIXEL_COUNT	IMAGE_WIDTH*IMAGE_HEIGHT/_PIXEL_SIZE
# define HEADERSIZE		144
# define ENCODE 1
# define DECODE 2
# define USAGE "usage: $> File2image [ -e | -d ] [ \033[4mfile\033[0m ... ] \n"

int				ipow(int base, int exp);
void			open_file(void);
void			get_filesize(void);
void			create_png(png_bytepp image, char *image_name);
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
