/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afonck <afonck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 15:27:01 by afonck            #+#    #+#             */
/*   Updated: 2019/10/28 16:22:57 by afonck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	is_bmp(char *file)
{
	int len;

	len = 0;
	while (file[len])
		len++;
	len -= 1;
	if (len < 4)
		return (0);
	if (file[len--] != 'p')
		return (0);
	if (file[len--] != 'm')
		return (0);
	if (file[len--] != 'b')
		return (0);
	if (file[len] != '.')
		return (0);
	return (1);
}
