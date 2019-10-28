/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libbmp.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afonck <afonck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 15:24:46 by afonck            #+#    #+#             */
/*   Updated: 2019/10/28 16:31:31 by afonck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBBMP_H
# define LIBBMP_H

# define PIX_WIDTH 128
# define PIX_HEIGHT 128
# define BITS_PER_PIX 32//24

# define BMP_ID ('M' * 256) + 'B'
# define OFFSET_BEFORE_DATA 54
# define BMP_HEADERSIZE 54
# define BYTES_PER_PIX BITS_PER_PIX / 8
# define BMP_SIZE ((PIX_WIDTH * PIX_HEIGHT) * BYTES_PER_PIX) + BMP_HEADERSIZE
# define IMG_SIZE ((PIX_WIDTH * PIX_HEIGHT) * BYTES_PER_PIX)

/*
** ERROR MESSAGES
*/
# define ERRBMP_ID "file ID is %{r}d, it should be %{g}d\n"
# define ERRBMP_SIZE "wrong size @ index %{b}d: %{r}d\n"
# define ERRBMP_RES "two bytes @ offset %d should be 0, but are %{r}d\n"
# define ERRBMP_OFFSET "offset to image data %{r}d @ index %{b}d wrong\n"

# define ERRBMP_HDSIZE "info header size: %{r}d, expected: 40\n"
# define ERRBMP_W_IMG "width in header: %{r}d is wrong\n"
# define ERRBMP_H_IMG "height in header: %{r}d is wrong\n"
# define ERRBMP_CLR_PLN "format error, color planes: %d\n"
# define ERRBMP_BBP "nb bits per pixel: %d, should be BITS_PER_PIX in header\n"
# define ERRBMP_COMPR "compression: %{r}d, only uncompressed bmp are handled\n"
# define ERRBMP_IMGSIZE "uncorrect image size: %{r}d\n"
# define ERRBMP_XRES "uncorrect horizontal resolution: %{r}d pixels per meter\n"
# define ERRBMP_YRES "uncorrect vertical resolution: %{r}d pixels per meter\n"
# define ERRBMP_NBCOLOR "uncorrect number of colors in image: %{r}d\n"
# define ERRBMP_NBCLI "uncorrect number of important colors: %{r}d\n"

# define ERRBMP_EOF "unexpected EOF while reading at index %d\n"
# define ERRBMP_OPEN "failed to open %{r}s: %{r}s\n"
# define ERRBMP_RD_BMPHEADER "error while reading bmp header of %{r}s\n"
# define ERRBMP_RD_INFOHEADER "error while reading info header of %{r}s\n"
# define ERRBMP_RD_IMG "error while reading pixel data of %{r}s\n"

typedef struct			s_bmp_header
{
	unsigned short int	type;
	unsigned int		size;
	unsigned short int	reserved1;
	unsigned short int	reserved2;
	unsigned int		offset;
}						t_bmp_header;

typedef struct			s_info_header
{
	unsigned int		size;
	int					width;
	int					height;
	unsigned short int	planes;
	unsigned short int	bits;
	unsigned int		compression;
	unsigned int		imagesize;
	int					xresolution;
	int					yresolution;
	unsigned int		ncolours;
	unsigned int		importantcolours;
}						t_info_header;

/*
** ERROR HANDLING FUNCTIONS
*/

int						bmpheader_readerror(int ret, char *file);
int						bmp_header_error(char *error_str, int data_one, \
int data_two);

int						info_header_error(char *error_str, int data);

int						parse_bmp_error(char *error_msg, char *param);
void					*parse_mallocbmp_error(char *error_msg, char *param);

/*
** BMP HEADER FUNCTIONS
*/

int						fill_bmp_header(t_bmp_header *bmp_header, int fd, \
char *file, int *offset);

int						fill_info_header(t_info_header *info_header, int fd, \
char *file, int *offset);

/*
** UTILS
*/

int						is_bmp(char *file);

/*
** FUNCTIONS TO USE
*/

int						simpleparse_bmp(char *file, int *pixels);

int						parse_bmp(char *file, t_bmp_header *bmp_header, \
t_info_header *info_header, int *pixels);

int						*parse_bmp_malloc(char *file, \
t_bmp_header *bmp_header, t_info_header *info_header);
#endif
