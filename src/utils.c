//#include "myBMPparser.h"
//#include "libft.h"

int     is_bmp(char *file)
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
