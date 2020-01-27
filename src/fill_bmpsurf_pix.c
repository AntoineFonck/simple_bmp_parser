/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_bmpsurf_pix.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afonck <afonck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/27 16:12:14 by afonck            #+#    #+#             */
/*   Updated: 2020/01/27 16:17:47 by afonck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libbmp.h"
#include "libft.h"

void	add_bmp_pad(void *pixel_line, int pad)
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

int		revfill_surfpix(SDL_Surface *surf, unsigned char *pixeldata)
{
	int			rowsize;
	Uint8		*bits;
	int			pad;

	rowsize = surf->w * surf->format->BytesPerPixel;
	bits = (Uint8 *)surf->pixels;
	pad = ((rowsize % 4) ? (4 - (rowsize % 4)) : 0);
	while (bits < (Uint8 *)surf->pixels + (surf->h * surf->pitch))
	{
		bits += surf->pitch;
		ft_memcpy(bits, pixeldata, rowsize);
		if (pad)
			add_bmp_pad(bits + rowsize, pad);
		pixeldata += surf->pitch;
	}
	return (0);
}

int		fill_surfpix(SDL_Surface *surf, unsigned char *pixeldata)
{
	int			rowsize;
	Uint8		*bits;
	int			pad;

	rowsize = surf->w * surf->format->BytesPerPixel;
	bits = (Uint8 *)surf->pixels + (surf->h * surf->pitch);
	pad = ((rowsize % 4) ? (4 - (rowsize % 4)) : 0);
	while (bits > (Uint8 *)surf->pixels)
	{
		bits -= surf->pitch;
		ft_memcpy(bits, pixeldata, rowsize);
		if (pad)
			add_bmp_pad(bits + rowsize, pad);
		pixeldata += surf->pitch;
	}
	return (0);
}
