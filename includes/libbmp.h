#ifndef MY_BMP_PARSER_H
# define MY_BMP_PARSER_H

# define PIX_WIDTH 128
# define PIX_HEIGHT 128
# define BITS_PER_PIX 32//24

# define BMP_ID ('M' * 256) + 'B'
# define OFFSET_BEFORE_DATA 54
# define BMP_HEADERSIZE 54
# define BYTES_PER_PIX BITS_PER_PIX / 8
# define BMP_SIZE ((PIX_WIDTH * PIX_HEIGHT) * BYTES_PER_PIX) + BMP_HEADERSIZE
# define IMG_SIZE ((PIX_WIDTH * PIX_HEIGHT) * BYTES_PER_PIX)

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

/*
** ERROR HANDLING FUNCTIONS
*/

int                     bmpheader_readerror(int ret, char *file);
int                     bmp_header_error(char *error_str, int data_one, int data_two);

int                     info_header_error(char *error_str, int data);

int                     parse_bmp_error(char *error_msg, char *param);
void                    *parse_mallocbmp_error(char *error_msg, char *param);

/*
** BMP HEADER FUNCTIONS
*/

int                     fill_bmp_header(t_bmp_header *bmp_header, int fd, char *file, int *offset);

int                     fill_info_header(t_info_header *info_header, int fd, char *file, int *offset);

/*
** UTILS
*/

int                     is_bmp(char *file);

/*
** FUNCTIONS TO USE
*/

int                     simpleparse_BMP(char *file, int *pixels);

int                     parse_BMP(char *file, t_bmp_header *bmp_header, t_info_header *info_header, int *pixels);

int                     *parse_BMP_malloc(char *file, t_bmp_header *bmp_header, t_info_header *info_header);
#endif
