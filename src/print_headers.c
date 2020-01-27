/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_headers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afonck <afonck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/27 16:57:18 by afonck            #+#    #+#             */
/*   Updated: 2020/01/27 17:00:23 by afonck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libbmp.h"
#include "libft.h"

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
