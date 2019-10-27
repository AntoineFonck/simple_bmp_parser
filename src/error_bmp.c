//#include "myBMPparser.h"
#include "libft.h"
#include <errno.h>

/*
** BMP HEADER ERROR
*/

int     bmpheader_readerror(int ret, char *file)
{
    if (ret == -1)
        ft_dprintf(STDERR_FILENO, "read fail in %{r}s: %{r}s\n", file, strerror(errno));
    else if (ret == 0)
        ft_dprintf(STDERR_FILENO, "unexpected end of file in %{r}s\n", file);
    return (EXIT_FAILURE);
}

int     bmp_header_error(char *error_str, int data_one, int data_two)
{
    ft_dprintf(STDERR_FILENO, error_str, data_one, data_two);
    return (EXIT_FAILURE);
}

/*
** INFO HEADER ERROR
*/

int     info_header_error(char *error_str, int data)
{
    ft_dprintf(STDERR_FILENO, error_str, data);
    return (EXIT_FAILURE);
}

/*
** MAIN FUNCTION ERROR RESPONSE
*/

int     parse_bmp_error(char *error_msg, char *param)
{
    ft_dprintf(STDERR_FILENO, error_msg, param);
    return (EXIT_FAILURE);
}

void    *parse_mallocbmp_error(char *error_msg, char *param)
{
    ft_dprintf(STDERR_FILENO, error_msg, param);
    return (NULL);
}