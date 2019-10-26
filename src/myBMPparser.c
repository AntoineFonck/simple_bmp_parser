#include "myBMPparser.h"
#include "libft.h"

int     fill_bmp_header(t_bmp_header *bmp_header, int fd, char *file, int *offset)
{
    int ret;

    if ((ret = read(fd, &bmp_header->type, 2)) == -1)
    {
        ft_dprintf(STDERR_FILENO, "read fail: %{r}s\n", strerror(errno));
        return (EXIT_FAILURE);
    }
    else if (ret == 0)
    {
        ft_dprintf(STDERR_FILENO, "unexpected end of file in %{r}s\n", file);
        return (EXIT_FAILURE);
    }
    *offset += ret;
    if (bmp_header->type != 0x4d42) //424d in little endian --> first byte (42=B) is the smallest, second byte is bigger ((4d=M) * 256)
    {
        ft_dprintf(STDERR_FILENO, "file ID is %{r}d, it should be %{g}d\n", bmp_header->type, ('M' * 256) + 'B');
        return (EXIT_FAILURE);
    }
    if ((ret = read(fd, &bmp_header->size, 4)) == -1)
    {
        ft_dprintf(STDERR_FILENO, "read fail: %{r}s\n", strerror(errno));
        return (EXIT_FAILURE);
    }
    else if (ret == 0)
    {
        ft_dprintf(STDERR_FILENO, "unexpected end of file in %{r}s\n", file);
        return (EXIT_FAILURE);
    }
    *offset += ret;
    if (bmp_header->size <= 0)
    {
        ft_dprintf(STDERR_FILENO, "size declared in header is %{r}d, error with file format\n", bmp_header->size);
        return (EXIT_FAILURE);
    }
    if ((ret = read(fd, &bmp_header->reserved1, 2)) == -1)
    {
        ft_dprintf(STDERR_FILENO, "read fail: %{r}s\n", strerror(errno));
        return (EXIT_FAILURE);
    }
    else if (ret == 0)
    {
        ft_dprintf(STDERR_FILENO, "unexpected end of file in %{r}s\n", file);
        return (EXIT_FAILURE);
    }
    *offset += ret;
    if (bmp_header->reserved1 != 0)
    {
        ft_dprintf(STDERR_FILENO, "the two bytes at offset %d should be 0, but are %{r}d\n", offset, bmp_header->reserved1);
        return (EXIT_FAILURE);
    }
    if ((ret = read(fd, &bmp_header->reserved2, 2)) == -1)
    {
        ft_dprintf(STDERR_FILENO, "read fail: %{r}s\n", strerror(errno));
        return (EXIT_FAILURE);
    }
    else if (ret == 0)
    {
        ft_dprintf(STDERR_FILENO, "unexpected end of file in %{r}s\n", file);
        return (EXIT_FAILURE);
    }
    *offset += ret;
    if (bmp_header->reserved2 != 0)
    {
        ft_dprintf(STDERR_FILENO, "the two bytes at offset %d should be 0, but are %{r}d\n", offset, bmp_header->reserved2);
        return (EXIT_FAILURE);
    }
    if ((ret = read(fd, &bmp_header->offset, 4)) == -1)
    {
        ft_dprintf(STDERR_FILENO, "read fail: %{r}s\n", strerror(errno));
        return (EXIT_FAILURE);
    }
    else if (ret == 0)
    {
        ft_dprintf(STDERR_FILENO, "unexpected end of file in %{r}s\n", file);
        return (EXIT_FAILURE);
    }
    *offset += ret;
    if (bmp_header->offset != OFFSET_BEFORE_DATA)//54)
    {
        ft_dprintf(STDERR_FILENO, "the offset to the image data %d at index %d seems incorrect\n", bmp_header->offset, *offset);
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

int     fill_info_header(t_info_header *info_header, int fd, char *file, int *offset)
{
    int ret;

    if ((ret = read(fd, info_header, sizeof(t_info_header))) == -1)
    {
        ft_dprintf(STDERR_FILENO, "read fail: %{r}s\n", strerror(errno));
        return (EXIT_FAILURE);
    }
    else if (ret == 0)
    {
        ft_dprintf(STDERR_FILENO, "unexpected end of file in %{r}s\n", file);
        return (EXIT_FAILURE);
    }
    *offset += sizeof(t_info_header);
    if (info_header->size != 40)
    {
        ft_dprintf(STDERR_FILENO, "format error: size of info header is declared %d, should be 40\n", info_header->size);
        return (EXIT_FAILURE);
    }
    //if (info_header->width <= 0 || info_header->height <= 0) //128 * 128 ?
    if (info_header->width != PIX_WIDTH || info_header->height != PIX_HEIGHT) //128 * 128 ?
    {
        ft_dprintf(STDERR_FILENO, "width and/or height in header (%d x %d) are wrong\n", info_header->width, info_header->height);
        return (EXIT_FAILURE);
    }
    if (info_header->planes != 1)
    {
        ft_dprintf(STDERR_FILENO, "format error, color planes in header = %d\n", info_header->planes);
        return (EXIT_FAILURE);
    }
    //if (info_header->bits != 24) //&& info_header->bits != 8) //only accept 24 bit bmp ?
    if (info_header->bits != BITS_PER_PIX) //&& info_header->bits != 8) //only accept 24 bit bmp ?
    {
        ft_dprintf(STDERR_FILENO, "nb of bits per pixel is %d, only 24 is accepted\n", info_header->bits);
        return (EXIT_FAILURE);
    }
    if (info_header->compression != 0)
    {
        ft_dprintf(STDERR_FILENO, "%{r}s is a compressed file, only uncompressed bmp are handled\n", file);
        return (EXIT_FAILURE);
    }
    if (info_header->imagesize <= 0)
    {
        ft_dprintf(STDERR_FILENO, "uncorrect image size: %{r}d\n", info_header->imagesize);
        return (EXIT_FAILURE);
    }
    if (info_header->xresolution <= 0)
    {
        ft_dprintf(STDERR_FILENO, "uncorrect horizontal resolution: %{r}d pixels per meter\n", info_header->xresolution);
        return (EXIT_FAILURE);
    }
    if (info_header->yresolution <= 0)
    {
        ft_dprintf(STDERR_FILENO, "uncorrect vertical resolution: %{r}d pixels per meter\n", info_header->yresolution);
        return (EXIT_FAILURE);
    }
    if (info_header->ncolours < 0)
    {
        ft_dprintf(STDERR_FILENO, "uncorrect number of colors in image: %{r}d\n", info_header->ncolours);
        return (EXIT_FAILURE);
    }
    if (info_header->importantcolours < 0)
    {
        ft_dprintf(STDERR_FILENO, "uncorrect number of important colors: %{r}d\n", info_header->importantcolours);
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

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

int     read_pixel(int fd, int *pixel, int index)
{
    int ret;

    //if ((ret = read(fd, pixel, 4)) == -1)
    if ((ret = read(fd, pixel, 3)) == -1)
    {
        ft_dprintf(STDERR_FILENO, "read fail: %{r}s\n", strerror(errno));
        return (EXIT_FAILURE);
    }
    else if (ret == 0)
    {
        ft_dprintf(STDERR_FILENO, "unexpected end of file while reading at index %d\n", index);
        return (EXIT_FAILURE);
    }
    //printf("index = %d\n", index);
    return (EXIT_SUCCESS);
}

int     fill_pixels(int fd, int *pixels)
{
    int index;

    index = 0;
    while (index < PIX_WIDTH * PIX_HEIGHT)
    //while (index < 128 * 128)
    {
        if ((read_pixel(fd, &pixels[index], index)) != EXIT_SUCCESS)
            return (EXIT_FAILURE);
        index++;
    }
    return (EXIT_SUCCESS);
}

int     simpleparse_BMP(char *file, int *pixels)
{
    t_bmp_header    bmp_header;
    t_info_header   info_header;
    int             fd;
    int             offset;

    offset = 0;
    if ((is_bmp(file)) != 1)
    {
        ft_dprintf(STDERR_FILENO, "%{r}s is not a valid bmp file\n", file);
        return (EXIT_FAILURE);
    }
    if ((fd = open(file, O_RDONLY | O_NOFOLLOW)) == -1)
    {
        ft_dprintf(STDERR_FILENO, "failed to open %{r}s: %{r}s\n", file, strerror(errno));
        return (EXIT_FAILURE);
    }
    if ((fill_bmp_header(&bmp_header, fd, file, &offset)) != EXIT_SUCCESS)
    {
        ft_dprintf(STDERR_FILENO, "error while reading bmp header of %{r}s\n", file);
        return (EXIT_FAILURE);
    }
    if ((fill_info_header(&info_header, fd, file, &offset)) != EXIT_SUCCESS)
    {
        ft_dprintf(STDERR_FILENO, "error while reading info header of %{r}s\n", file);
        return (EXIT_FAILURE);
    }
    fill_pixels(fd, pixels);
    return (EXIT_SUCCESS);
}

int     parse_BMP(char *file, t_bmp_header *bmp_header, t_info_header *info_header, int *pixels)
{
    int fd;
    int offset;

    offset = 0;
    //pixels = NULL;
    if ((is_bmp(file)) != 1)
    {
        ft_dprintf(STDERR_FILENO, "%{r}s is not a valid bmp file\n", file);
        return (EXIT_FAILURE);
    }
    if ((fd = open(file, O_RDONLY | O_NOFOLLOW)) == -1)
    {
        ft_dprintf(STDERR_FILENO, "failed to open %{r}s: %{r}s\n", file, strerror(errno));
        return (EXIT_FAILURE);
    }
    if ((fill_bmp_header(bmp_header, fd, file, &offset)) != EXIT_SUCCESS)
    {
        ft_dprintf(STDERR_FILENO, "error while reading bmp header of %{r}s\n", file);
        return (EXIT_FAILURE);
    }
    if ((fill_info_header(info_header, fd, file, &offset)) != EXIT_SUCCESS)
    {
        ft_dprintf(STDERR_FILENO, "error while reading info header of %{r}s\n", file);
        return (EXIT_FAILURE);
    }
    //if ((pixels = (int *)malloc(sizeof(char) * (info_header->width * info_header->height * (info_header->bits / 3)))) == NULL)
    //    return (NULL);
    fill_pixels(fd, pixels);
    return (EXIT_SUCCESS);
}

int     *parse_BMP_malloc(char *file, t_bmp_header *bmp_header, t_info_header *info_header)
{
    int fd;
    int offset;
    int *pixels;

    offset = 0;
    pixels = NULL;
    if ((is_bmp(file)) != 1)
    {
        ft_dprintf(STDERR_FILENO, "%{r}s is not a valid bmp file\n", file);
        return (NULL);
    }
    if ((fd = open(file, O_RDONLY | O_NOFOLLOW)) == -1)
    {
        ft_dprintf(STDERR_FILENO, "failed to open %{r}s: %{r}s\n", file, strerror(errno));
        return (NULL);
    }
    if ((fill_bmp_header(bmp_header, fd, file, &offset)) != EXIT_SUCCESS)
    {
        ft_dprintf(STDERR_FILENO, "error while reading bmp header of %{r}s\n", file);
        return (NULL);
    }
    if ((fill_info_header(info_header, fd, file, &offset)) != EXIT_SUCCESS)
    {
        ft_dprintf(STDERR_FILENO, "error while reading info header of %{r}s\n", file);
        return (NULL);
    }
    if ((pixels = (int *)malloc(sizeof(char) * (info_header->width * info_header->height * (info_header->bits / 3)))) == NULL)
        return (NULL);
    fill_pixels(fd, pixels);
    return (pixels);
}
/*
int     main(int argc, char *argv[])
{
    t_bmp_header bmp_header;
    t_info_header info_header;
    int *test;

    test = NULL;
    if (argc != 2)
        return (1);
    //parse_BMP("textures/SiliconCircuit.bmp", &bmp_header, &info_header);
    test = parse_BMP(argv[1], &bmp_header, &info_header);
    //printf("bmp_header->size = %d\n", bmp_header.size);
    //printf("bmp_header->offset = %d\n", bmp_header.offset);
    //printf("test = %p\n", test);
    write(STDOUT_FILENO, test, PIX_HEIGHT * PIX_WIDTH);
    //write(STDOUT_FILENO, &bmp_header, sizeof(bmp_header));
    //write(STDOUT_FILENO, &info_header, sizeof(info_header));
    //printf("sizeof bmp header = %lu\n", sizeof(bmp_header));
    //printf("sizeof info header = %lu\n", sizeof(info_header));
    //printf("sizeof info header = %lu\n", sizeof(t_info_header));
    return (0);
}
*/
