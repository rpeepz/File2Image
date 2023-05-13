/*
 * 2023 Robert Papagna
 *
 * https://github.com/rpeepz/File2Image
 * 
 */

#include "File2Image.h"

const size_t cmd_size = 128;

// TODO
// a multifile conversion needs to programatically step through each file
void  increment_filename(void)
{
  char  *filename;
  size_t size = strlen(f2i.filename);
  int number = atoi(&f2i.filename[3]);

  // int fd = open(f2i.filename, O_RDONLY);
  // if (fd == -1)
  //   return ; //exit

  //find out if we are going to need a new digit
  // eg 9, 99, 999, 9999 ...
  if (f2i.filename[size - 5] == '9')
  {
    size++;
    filename = malloc(size);
    memcpy(filename, f2i.filename, size);
    // if going to 3 or more digits
    if (size > 9)
      free(f2i.filename);
    f2i.filename = filename;
  }
  number++;
  snprintf(f2i.filename, size + 1, "f2i%d.png", number);
  printf("after %s\n", f2i.filename);
  // free(f2i.filename);
}

// wrapper for ffmpeg:
//
// ffmpeg -i f2i1.png -c:v libx264rgb -preset ultrafast -crf 0 -pix_fmt rgb24 -t 1 output.mp4
void	convert_image_to_video(void)
{
  // TODO
  // initial checks for input f2i.filename to make sure its an f2i image
  char *cmd = malloc(sizeof(char) * cmd_size);
  if (!cmd)
  {
    //Handle error
    return ;
  }
  char *oname = "output.mp4";
  int duration = 1;
  snprintf(cmd, cmd_size, "ffmpeg -i %s %s -t %d %s", f2i.filename, F2I_VIDEO_ENCODE_OPTIONS, duration, oname);
  system(cmd);
  free(cmd);
  
  // call function when multiple files are used to create a video
  // increment_filename();
  // if (strlen(f2i.filename) > 8)
  //   free(f2i.filename);
}

// wrapper for ffmpeg
//
// ffmpeg -i output.mp4 -vframes 1 f2i2.png
void map_video_to_image(void)
{
  // TODO
  // initial checks for input f2i.filename to make sure its an f2i video
  size_t cmd_size = 128;
  char *cmd = malloc(sizeof(char) * cmd_size);
  if (!cmd)
  {
    //Handle error
    return ;
  }
  char *oname = "f2i1.png";
  snprintf(cmd, cmd_size, "ffmpeg -i %s -vframes 1 %s", f2i.filename, oname);
  system(cmd);
  free(cmd);

  // TODO
  // utilize vframe option when outputting multiple files from the video
}
