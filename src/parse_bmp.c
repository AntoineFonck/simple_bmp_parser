#include "libbmp.h"
#include "libft.h"
#include <errno.h>

static int  read_pixel(int fd, int *pixel, int index)
{
    int ret;

    //if ((ret = read(fd, pixel, 4)) == -1)
    if ((ret = read(fd, pixel, BYTES_PER_PIX)) == -1)
    {
        ft_dprintf(STDERR_FILENO, "read fail: %{r}s\n", strerror(errno));
        return (EXIT_FAILURE);
    }
    else if (ret == 0)
    {
        ft_dprintf(STDERR_FILENO, "unexpected EOF while reading at index %d\n", index);
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

static int  fill_pixels(int fd, int *pixels)
{
    int index;

    index = 0;
    while (index < PIX_WIDTH * PIX_HEIGHT)
    {
        if ((read_pixel(fd, &pixels[index], index)) != EXIT_SUCCESS)
            return (EXIT_FAILURE);
        if (pixels[index] < 0)
            pixels[index] = ft_absolute(pixels[index]);
            //ft_dprintf(STDOUT_FILENO, "%d, index = %d\n", pixels[index], index);
        if (pixels[index] < 0)
            return (EXIT_FAILURE);
        index++;
    }
    return (EXIT_SUCCESS);
}

int         simpleparse_BMP(char *file, int *pixels)
{
    t_bmp_header    bmp_header;
    t_info_header   info_header;
    int             fd;
    int             offset;

    offset = 0;
    if ((is_bmp(file)) != 1)
        return (parse_bmp_error("%{r}s is not a valid bmp file\n", file));
    if ((fd = open(file, O_RDONLY | O_NOFOLLOW)) == -1)
    {
        ft_dprintf(STDERR_FILENO, "failed to open %{r}s: %{r}s\n", file, strerror(errno));
        return (EXIT_FAILURE);
    }
    if ((fill_bmp_header(&bmp_header, fd, file, &offset)) != EXIT_SUCCESS)
        return (parse_bmp_error("error while reading bmp header of %{r}s\n", file));
    if ((fill_info_header(&info_header, fd, file, &offset)) != EXIT_SUCCESS)
        return (parse_bmp_error("error while reading info header of %{r}s\n", file));
    if ((fill_pixels(fd, pixels)) != EXIT_SUCCESS)
        return (parse_bmp_error("error while reading pixel data of %{r}s\n", file));
    return (EXIT_SUCCESS);
}

int         parse_BMP(char *file, t_bmp_header *bmp_header, t_info_header *info_header, int *pixels)
{
    int fd;
    int offset;

    offset = 0;
    //pixels = NULL;
    if ((is_bmp(file)) != 1)
        return (parse_bmp_error("%{r}s is not a valid bmp file\n", file));
    if ((fd = open(file, O_RDONLY | O_NOFOLLOW)) == -1)
    {
        ft_dprintf(STDERR_FILENO, "failed to open %{r}s: %{r}s\n", file, strerror(errno));
        return (EXIT_FAILURE);
    }
    if ((fill_bmp_header(bmp_header, fd, file, &offset)) != EXIT_SUCCESS)
        return (parse_bmp_error("error while reading bmp header of %{r}s\n", file));
    if ((fill_info_header(info_header, fd, file, &offset)) != EXIT_SUCCESS)
        return (parse_bmp_error("error while reading info header of %{r}s\n", file));
    if ((fill_pixels(fd, pixels)) != EXIT_SUCCESS)
        return (parse_bmp_error("error while reading pixel data of %{r}s\n", file));
    return (EXIT_SUCCESS);
}

int         *parse_BMP_malloc(char *file, t_bmp_header *bmp_header, t_info_header *info_header)
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
        ft_dprintf(STDERR_FILENO, "failed to open %{r}s: %{r}s\n", file, strerror(errno));
        return (NULL);
    }
    if ((fill_bmp_header(bmp_header, fd, file, &offset)) != EXIT_SUCCESS)
        return (parse_mallocbmp_error("error while reading bmp header of %{r}s\n", file));
    if ((fill_info_header(info_header, fd, file, &offset)) != EXIT_SUCCESS)
        return (parse_mallocbmp_error("error while reading info header of %{r}s\n", file));
    if ((pixels = (int *)malloc(sizeof(char) * (info_header->width * info_header->height * (info_header->bits / 8)))) == NULL)
        return (NULL);
    if ((fill_pixels(fd, pixels)) != EXIT_SUCCESS)
        return (parse_mallocbmp_error("error while reading pixel data of %{r}s\n", file));
    return (pixels);
}
