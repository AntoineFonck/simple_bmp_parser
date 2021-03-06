/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bmp_header.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afonck <afonck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 15:26:06 by afonck            #+#    #+#             */
/*   Updated: 2020/01/31 14:48:31 by afonck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libbmp.h"
#include "libft.h"
#include <errno.h>

static int	read_reserved_bmp(int fd, t_bmp_header *bmp_header, int *offset, \
char *file)
{
	int ret;

	ret = 0;
	if ((ret = read(fd, &bmp_header->reserved1, 2)) != 2)
		return (bmpheader_readerror(ret, file));
	if (bmp_header->reserved1 != 0)
		return (bmp_header_error(ERRBMP_RES, *offset, bmp_header->reserved1));
	*offset += ret;
	if ((ret = read(fd, &bmp_header->reserved2, 2)) != 2)
		return (bmpheader_readerror(ret, file));
	if (bmp_header->reserved2 != 0)
		return (bmp_header_error(ERRBMP_RES, *offset, bmp_header->reserved2));
	*offset += ret;
	return (EXIT_SUCCESS);
}

int			fill_bmp_header(t_bmp_header *bmp_header, int fd, char *file, \
int *offset)
{
	int ret;

	if ((ret = read(fd, &bmp_header->type, 2)) != 2)
		return (bmpheader_readerror(ret, file));
	if (bmp_header->type != 0x4d42)
		return (bmp_header_error(ERRBMP_ID, bmp_header->type, BMP_ID));
	*offset += ret;
	if ((ret = read(fd, &bmp_header->size, 4)) != 4)
		return (bmpheader_readerror(ret, file));
	*offset += ret;
	if (read_reserved_bmp(fd, bmp_header, offset, file) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	if ((ret = read(fd, &bmp_header->offset, 4)) != 4)
		return (bmpheader_readerror(ret, file));
	if (bmp_header->offset != 54 && bmp_header->offset != 122 \
		&& bmp_header->offset != 138)
		return (bmp_header_error(ERRBMP_OFFSET, bmp_header->offset, *offset));
	*offset += ret;
	return (EXIT_SUCCESS);
}

static int	check_info_header2(t_info_header *info_header)
{
	if (info_header->bits != 24 && info_header->bits != 32)
		return (info_header_error(ERRBMP_BBP, info_header->bits));
	if (info_header->compression != 0)
		return (info_header_error(ERRBMP_COMPR, info_header->compression));
	if (info_header->imagesize < 0)
		return (info_header_error(ERRBMP_IMGSIZE, info_header->imagesize));
	if (info_header->xresolution < 0)
		return (info_header_error(ERRBMP_XRES, info_header->xresolution));
	if (info_header->yresolution < 0)
		return (info_header_error(ERRBMP_YRES, info_header->yresolution));
	if (info_header->ncolours != 0)
		return (info_header_error(ERRBMP_NBCOLOR, info_header->ncolours));
	if (info_header->importantcolours != 0)
		return (info_header_error(ERRBMP_NBCLI, info_header->importantcolours));
	return (EXIT_SUCCESS);
}

static int	check_info_header(t_info_header *info_header, int *inverse_h)
{
	if (info_header->size != 40 && info_header->size != 108 \
		&& info_header->size != 124)
		return (info_header_error(ERRBMP_HDSIZE, info_header->size));
	if (info_header->width <= 0 || info_header->width > MAX_PIXW)
		return (info_header_error(ERRBMP_W_IMG, info_header->width));
	if (info_header->height == 0 || info_header->height > MAX_PIXH)
		return (info_header_error(ERRBMP_H_IMG, info_header->height));
	if (info_header->height < 0)
	{
		ft_dprintf(STDERR_FILENO, "height is inversed\n");
		*inverse_h = 1;
		info_header->height = -info_header->height;
	}
	if (info_header->planes != 1)
		return (info_header_error(ERRBMP_CLR_PLN, info_header->planes));
	if (check_info_header2(info_header) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int			fill_info_header(t_info_header *info_header, int fd, \
int *inverse_h, int *offset)
{
	int ret;

	*inverse_h = 0;
	if ((ret = read(fd, info_header, sizeof(t_info_header))) \
	!= sizeof(t_info_header))
	{
		ft_dprintf(STDERR_FILENO, "read fail: %{r}s\n", strerror(errno));
		return (EXIT_FAILURE);
	}
	else if (ret == 0)
	{
		ft_dprintf(STDERR_FILENO, "unexpected end of file in info header\n");
		return (EXIT_FAILURE);
	}
	*offset += sizeof(t_info_header);
	if (check_info_header(info_header, inverse_h) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
