/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afonck <afonck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 15:26:27 by afonck            #+#    #+#             */
/*   Updated: 2019/12/20 00:08:48 by afonck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libbmp.h"
#include "libft.h"
#include <errno.h>

static int	read_pixel(int fd, int *pixel, int index)
{
	int				ret;
	unsigned char   pixbuf[4];

	if ((ret = read(fd, pixel, sizeof(int))) == -1)
	{
		ft_dprintf(STDERR_FILENO, "read fail: %{r}s\n", strerror(errno));
		return (EXIT_FAILURE);
	}
	else if (ret == 0)
	{
		ft_dprintf(STDERR_FILENO, ERRBMP_EOF, index);
		return (EXIT_FAILURE);
	}
	pixbuf[3] = 0x00;
	*pixel = *((int *)pixbuf);
	return (EXIT_SUCCESS);
}

void		add_bmp_pad(void *pixel_line, int pad)
{
	Uint8		padbyte;
	int			i;

	padbyte = 0;
	i = 0;
	while (i < pad)
	{
		ft_memcpy(pixel_line, &padbyte, 1);
		++i;
	}
}

static int	fill_surfpix(SDL_Surface *surf, unsigned char *pixeldata)
{
	int			rowsize;
	Uint8		*bits;
    int			pad;

	rowsize = surf->w * surf->format->BytesPerPixel;
	bits = (Uint8 *) surf->pixels + (surf->h * surf->pitch);
	pad = ((rowsize % 4) ? (4 - (rowsize % 4)) : 0);
    while (bits > (Uint8 *) surf->pixels)
	{
		bits -= surf->pitch;
		ft_memcpy(bits, pixeldata, rowsize);
        if (pad)
			add_bmp_pad(bits + (rowsize - pad), pad);
		pixeldata += rowsize;
	}
	return (0);
}

static int	fill_pixels(int fd, int *pixels, int width, int height)
{
	
	int index;

	index = 0;
	while (index < width * height)
	{
		if ((read_pixel(fd, &pixels[index], index)) != EXIT_SUCCESS)
			return (EXIT_FAILURE);
		if (pixels[index] < 0)
			pixels[index] = ft_absolute(pixels[index]);
		if (pixels[index] < 0)
			return (EXIT_FAILURE);
		index++;
	}
	return (EXIT_SUCCESS);
}

int			simpleparse_bmp(char *file, int *pixels)
{
	t_bmp_header	bmp_header;
	t_info_header	info_header;
	int				fd;
	int				offset;

	offset = 0;
	if ((is_bmp(file)) != 1)
		return (parse_bmp_error("%{r}s is not a valid bmp file\n", file));
	if ((fd = open(file, O_RDONLY | O_NOFOLLOW)) == -1)
	{
		ft_dprintf(STDERR_FILENO, ERRBMP_OPEN, file, strerror(errno));
		return (EXIT_FAILURE);
	}
	if ((fill_bmp_header(&bmp_header, fd, file, &offset)) != EXIT_SUCCESS)
		return (parse_bmp_error(ERRBMP_RD_BMPHEADER, file));
	if ((fill_info_header(&info_header, fd, file, &offset)) != EXIT_SUCCESS)
		return (parse_bmp_error(ERRBMP_RD_INFOHEADER, file));
	if ((fill_pixels(fd, pixels, info_header.width, info_header.height)) != EXIT_SUCCESS)
		return (parse_bmp_error(ERRBMP_RD_IMG, file));
	return (EXIT_SUCCESS);
}

int			parse_bmp(char *file, t_bmp_header *bmp_header, \
t_info_header *info_header, int *pixels)
{
	int fd;
	int offset;

	offset = 0;
	if ((is_bmp(file)) != 1)
		return (parse_bmp_error("%{r}s is not a valid bmp file\n", file));
	if ((fd = open(file, O_RDONLY | O_NOFOLLOW)) == -1)
	{
		ft_dprintf(STDERR_FILENO, ERRBMP_OPEN, file, strerror(errno));
		return (EXIT_FAILURE);
	}
	if ((fill_bmp_header(bmp_header, fd, file, &offset)) != EXIT_SUCCESS)
		return (parse_bmp_error(ERRBMP_RD_BMPHEADER, file));
	if ((fill_info_header(info_header, fd, file, &offset)) != EXIT_SUCCESS)
		return (parse_bmp_error(ERRBMP_RD_INFOHEADER, file));
	if ((fill_pixels(fd, pixels, info_header->width, info_header->height)) != EXIT_SUCCESS)
		return (parse_bmp_error(ERRBMP_RD_IMG, file));
	return (EXIT_SUCCESS);
}

int			*parse_bmp_malloc(char *file, t_bmp_header *bmp_header, \
t_info_header *info_header)
{
	int fd;
	int offset;
	int *pixels;

	offset = 0;
	pixels = NULL;
	if ((is_bmp(file)) != 1)
		return (parse_mallocbmp_error("%{r}s is not a valid bmp file\n", file));
	if ((fd = open(file, O_RDONLY | O_NOFOLLOW)) == -1)
	{
		ft_dprintf(STDERR_FILENO, ERRBMP_OPEN, file, strerror(errno));
		return (NULL);
	}
	if ((fill_bmp_header(bmp_header, fd, file, &offset)) != EXIT_SUCCESS)
		return (parse_mallocbmp_error(ERRBMP_RD_BMPHEADER, file));
	if ((fill_info_header(info_header, fd, file, &offset)) != EXIT_SUCCESS)
		return (parse_mallocbmp_error(ERRBMP_RD_INFOHEADER, file));
	if ((pixels = (int *)malloc(sizeof(char) * (info_header->width * \
	info_header->height * (info_header->bits / 8)))) == NULL)
		return (NULL);
	if ((fill_pixels(fd, pixels, info_header->width, info_header->height)) != EXIT_SUCCESS)
		return (parse_mallocbmp_error(ERRBMP_RD_IMG, file));
	return (pixels);
}

void			print_bmpheader(t_bmp_header *bmp_header)
{
	ft_printf("***	BMP HEADER	***\n");
	ft_printf("file type: %hu\n", bmp_header->type);
	ft_printf("file size: %u\n", bmp_header->size);
	ft_printf("res 1: %hu\n", bmp_header->reserved1);
	ft_printf("res 2: %hu\n", bmp_header->reserved2);
	ft_printf("offset to pixel data: %u\n", bmp_header->offset);
}

void			print_infoheader(t_info_header *info_header)
{
	ft_printf("***	INFO HEADER	***\n");
	ft_printf("header size: %u\n", info_header->size);
	ft_printf("file width: %d\n", info_header->width);
	ft_printf("file height: %d\n", info_header->height);
	ft_printf("planes: %hu\n", info_header->planes);
	ft_printf("nb bits per pixel: %hu\n", info_header->bits);
	ft_printf("file compression: %u\n", info_header->compression);
	ft_printf("file size if compression: %u\n", info_header->imagesize);
	ft_printf("file xres: %d\n", info_header->xresolution);
	ft_printf("file yres: %d\n", info_header->yresolution);
	ft_printf("nbcolors: %u\n", info_header->ncolours);
	ft_printf("important colors: %u\n", info_header->importantcolours);
}

SDL_Surface		*load_bmp(char *file)
{
	t_bmpix			bmpix;
	t_headers		headers;
	int				fd;
	int				offset;

	bmpix.bmp_surf = NULL;
	bmpix.pixeldata = NULL;
	offset = 0;
	if ((is_bmp(file)) != 1)
		return (NULL);
	if ((fd = open(file, O_RDONLY | O_NOFOLLOW)) == -1)
	{
		ft_dprintf(STDERR_FILENO, ERRBMP_OPEN, file, strerror(errno));
		return (NULL);
	}
	if ((fill_bmp_header(&headers.bmp_header, fd, file, &offset)) != EXIT_SUCCESS)
		return (NULL);
	if ((fill_info_header(&headers.info_header, fd, file, &offset)) != EXIT_SUCCESS)
		return (NULL);
	if ((bmpix.bmp_surf = SDL_CreateRGBSurface(0, headers.info_header.width, headers.info_header.height, headers.info_header.bits, 0, 0, 0, 0)) == NULL)
	{
		ft_dprintf(STDERR_FILENO, "SDL error %{r}s\n", SDL_GetError());
		return(NULL);
	}
	if ((bmpix.pixeldata = (unsigned char *)malloc(bmpix.bmp_surf->h * bmpix.bmp_surf->pitch)) == NULL)
		return (NULL);
	if ((read(fd, bmpix.pixeldata, bmpix.bmp_surf->h * bmpix.bmp_surf->pitch)) <= 0)
	{
		free(bmpix.pixeldata);
		ft_printf("error while reading pixeldata\n");
		bmpix.pixeldata = NULL;
		return (NULL);
	}
	if ((fill_surfpix(bmpix.bmp_surf, bmpix.pixeldata)) != EXIT_SUCCESS)
		return (NULL);
	free(bmpix.pixeldata);
	bmpix.pixeldata = NULL;
	return (bmpix.bmp_surf);
}
