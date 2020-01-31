/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afonck <afonck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 15:26:27 by afonck            #+#    #+#             */
/*   Updated: 2020/01/31 14:49:20 by afonck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libbmp.h"
#include "libft.h"
#include <errno.h>

int				fill_rest(int fd, int offset)
{
	unsigned char	v4_rest[68];
	unsigned char	v5_rest[84];

	if (offset == 122)
	{
		if ((read(fd, v4_rest, 68)) != 68)
		{
			ft_dprintf(STDERR_FILENO, "error or unexpected EOF\n");
			return (1);
		}
	}
	else if (offset == 138)
	{
		if ((read(fd, v5_rest, 84)) != 84)
		{
			ft_dprintf(STDERR_FILENO, "error or unexpected EOF\n");
			return (1);
		}
	}
	return (0);
}

static int		actual_load_bmp(t_bmpix *bmpix, t_headers *headers, int fd)
{
	if ((bmpix->pixeldata = (unsigned char *)malloc(bmpix->bmp_surf->h \
	* bmpix->bmp_surf->pitch)) == NULL)
		bmpix->error = 1;
	if (bmpix->error == 1 || (read(fd, bmpix->pixeldata, bmpix->bmp_surf->h \
	* bmpix->bmp_surf->pitch)) != bmpix->bmp_surf->h * bmpix->bmp_surf->pitch)
		bmpix->error = 1;
	if (headers->inverse_h == 1)
	{
		if (bmpix->error == 1 || (revfill_surfpix(bmpix->bmp_surf, \
		bmpix->pixeldata)) != EXIT_SUCCESS)
			bmpix->error = 1;
	}
	else if (bmpix->error == 1 || (fill_surfpix(bmpix->bmp_surf, \
	bmpix->pixeldata)) != EXIT_SUCCESS)
		bmpix->error = 1;
	free(bmpix->pixeldata);
	bmpix->pixeldata = NULL;
	if (bmpix->error == 1)
	{
		ft_dprintf(STDERR_FILENO, "error during bmp loading\n");
		SDL_FreeSurface(bmpix->bmp_surf);
		bmpix->bmp_surf = NULL;
	}
	return (bmpix->error);
}

static int		init_load_bmp(char *file, int *fd, t_bmpix *bmpix, int *offset)
{
	bmpix->bmp_surf = NULL;
	bmpix->pixeldata = NULL;
	*offset = 0;
	bmpix->error = 0;
	if (!file || (is_bmp(file)) != 1)
		return (EXIT_FAILURE);
	if ((*fd = open(file, O_RDONLY | O_NOFOLLOW)) == -1)
	{
		ft_dprintf(STDERR_FILENO, ERRBMP_OPEN, file, strerror(errno));
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static void		*cleanup(int fd, char *file)
{
	if (close(fd) != 0)
	{
		if (file != NULL)
			ft_dprintf(STDERR_FILENO, "failed to close \
			%{r}s\n", file);
		else
			ft_dprintf(STDERR_FILENO, "failed to close NULL file\n");
	}
	return (NULL);
}

SDL_Surface		*load_bmp(char *file)
{
	t_bmpix			bmpix;
	t_headers		headers;
	int				fd;
	int				offset;

	if (init_load_bmp(file, &fd, &bmpix, &offset) != EXIT_SUCCESS)
		return (NULL);
	if ((fill_bmp_header(&headers.bmp_header, fd, file, \
	&offset)) != EXIT_SUCCESS)
		return (cleanup(fd, file));
	if ((fill_info_header(&headers.info_header, fd, &headers.inverse_h, \
	&offset)) != EXIT_SUCCESS)
		return (cleanup(fd, file));
	if ((fill_rest(fd, headers.bmp_header.offset)) != 0)
		return (cleanup(fd, file));
	if ((bmpix.bmp_surf = SDL_CreateRGBSurface(0, headers.info_header.width, \
	headers.info_header.height, headers.info_header.bits, 0, 0, 0, 0)) == NULL)
	{
		ft_dprintf(STDERR_FILENO, "SDL error %{r}s\n", SDL_GetError());
		return (cleanup(fd, file));
	}
	actual_load_bmp(&bmpix, &headers, fd);
	cleanup(fd, file);
	return (bmpix.error != 0 ? NULL : bmpix.bmp_surf);
}
