#ifndef MY_BMP_PARSER_H
# define MY_BMP_PARSER_H

# include <fcntl.h>
# include <unistd.h>
# include <errno.h>

# define OFFSET_BEFORE_DATA 54
# define PIX_WIDTH 256//128
# define PIX_HEIGHT 256//128
# define BITS_PER_PIX 24

typedef struct          s_bmp_header
{
    unsigned short int  type;
    unsigned int        size;
    unsigned short int  reserved1;
    unsigned short int  reserved2;
    unsigned int        offset;
}                       t_bmp_header;

typedef struct          s_info_header
{
   unsigned int         size;               /* Header size in bytes      */
   int                  width;
   int                  height;                /* Width and height of image */
   unsigned short int   planes;       /* Number of colour planes   */
   unsigned short int   bits;         /* Bits per pixel            */
   unsigned int         compression;        /* Compression type          */
   unsigned int         imagesize;          /* Image size in bytes       */
   int                  xresolution;
   int                  yresolution;     /* Pixels per meter          */
   unsigned int         ncolours;           /* Number of colours         */
   unsigned int         importantcolours;   /* Important colours         */
}                       t_info_header;

//int     *parse_BMP(char *file, t_bmp_header *bmp_header, t_info_header *info_header);

int                     ft_dprintf_bmp(int fd, const char *fmt, ...);

int     parse_BMP(char *file, t_bmp_header *bmp_header, t_info_header *info_header, int *pixels);

#endif
