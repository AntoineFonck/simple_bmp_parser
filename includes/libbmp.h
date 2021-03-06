/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libbmp.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afonck <afonck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 15:24:46 by afonck            #+#    #+#             */
/*   Updated: 2020/01/27 16:14:39 by afonck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBBMP_H
# define LIBBMP_H

# include "SDL.h"

/*
** BMP ID --> ('M' * 256) + 'B'
*/
# define BMP_ID 19778

/*
** BYTES PER PIX --> BITS PER PIX / 8
*/

# define MAX_PIXW 2000
# define MAX_PIXH 2000

/*
** BMP_SIZE --> ((PIX_WIDTH * PIX_HEIGHT) * BYTES_PER_PIX) + BMP_HEADERSIZE
** IMG_SIZE --> ((PIX_WIDTH * PIX_HEIGHT) * BYTES_PER_PIX)
*/

/*
** ERROR MESSAGES
*/
# define ERRBMP_ID "file ID is %{r}d, it should be %{g}d\n"
# define ERRBMP_SIZE "wrong size @ index %{b}d: %{r}d\n"
# define ERRBMP_RES "two bytes @ offset %d should be 0, but are %{r}d\n"
# define ERRBMP_OFFSET "offset %{r}d @ index %{b}d: bmp type not supported\n"

# define ERRBMP_HDSIZE "info header size: %{r}d, bmp type not supported\n"
# define ERRBMP_W_IMG "width in header: %{r}d is wrong\n"
# define ERRBMP_H_IMG "height in header: %{r}d is wrong\n"
# define ERRBMP_CLR_PLN "format error, color planes: %d\n"
# define ERRBMP_BBP "nb bits per pixel: %d, only 24 bpp and 32 bpp supported\n"
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

typedef struct			s_headers
{
	t_bmp_header		bmp_header;
	t_info_header		info_header;
	int					inverse_h;
}						t_headers;

typedef struct			s_bmpix
{
	SDL_Surface			*bmp_surf;
	unsigned char		*pixeldata;
	int					error;
}						t_bmpix;

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
int *inverse_h, int *offset);

/*
** UTILS
*/

int						is_bmp(char *file);

/*
** FILL PIX DATA BUFFER
*/

int						fill_surfpix(SDL_Surface *surf, \
unsigned char *pixeldata);

int						revfill_surfpix(SDL_Surface *surf, \
unsigned char *pixeldata);

/*
** FUNCTIONS TO USE (load bmp to surf is the only solid one at the moment)
*/

SDL_Surface				*load_bmp(char *file);
/*
** int						simpleparse_bmp(char *file, int *pixels);
**
** int						parse_bmp(char *file, t_bmp_header *bmp_header, \
** t_info_header *info_header, int *pixels);
**
** int						*parse_bmp_malloc(char *file, \
** t_bmp_header *bmp_header, t_info_header *info_header);
*/
#endif
