/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afonck <afonck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 15:26:27 by afonck            #+#    #+#             */
/*   Updated: 2019/12/11 17:23:28 by afonck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libbmp.h"
#include "libft.h"
#include <errno.h>

static int	read_pixel(int fd, int *pixel, int index)
{
	int				ret;
	unsigned char	pixbuf[BYTES_PER_PIX];

	if ((ret = read(fd, pixbuf, BYTES_PER_PIX)) == -1)
	{
		ft_dprintf(STDERR_FILENO, "read fail: %{r}s\n", strerror(errno));
		return (EXIT_FAILURE);
	}
	else if (ret == 0)
	{
		ft_dprintf(STDERR_FILENO, ERRBMP_EOF, index);
		return (EXIT_FAILURE);
	}
	if (BYTES_PER_PIX == 4)
		pixbuf[BYTES_PER_PIX - 1] = 0x00;
	*pixel = *((int *)pixbuf);
	return (EXIT_SUCCESS);
}

static int	fill_pixels(int fd, int *pixels)
{
	int index;

	index = 0;
	while (index < PIX_WIDTH * PIX_HEIGHT)
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
	if ((fill_pixels(fd, pixels)) != EXIT_SUCCESS)
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
	if ((fill_pixels(fd, pixels)) != EXIT_SUCCESS)
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
	if ((fill_pixels(fd, pixels)) != EXIT_SUCCESS)
		return (parse_mallocbmp_error(ERRBMP_RD_IMG, file));
	return (pixels);
}
