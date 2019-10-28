#include "libbmp.h"
#include "libft.h"
#include <errno.h>

static int  read_reserved_bmp(int fd, t_bmp_header *bmp_header, int *offset, char *file)
{
    int ret;

    ret = 0;
    if ((ret = read(fd, &bmp_header->reserved1, 2)) <= 0)
        return (bmpheader_readerror(ret, file));
    if (bmp_header->reserved1 != 0)
        return (bmp_header_error("two bytes at offset %d should be 0, but are %{r}d\n", *offset, bmp_header->reserved1));
    *offset += ret;
    if ((ret = read(fd, &bmp_header->reserved2, 2)) <= 0)
        return (bmpheader_readerror(ret, file));
    if (bmp_header->reserved2 != 0)
        return (bmp_header_error("two bytes @ offset %d should be 0, but are %{r}d\n", *offset, bmp_header->reserved2));
    *offset += ret;
    return (EXIT_SUCCESS);
}

int         fill_bmp_header(t_bmp_header *bmp_header, int fd, char *file, int *offset)
{
    int ret;

    if ((ret = read(fd, &bmp_header->type, 2)) <= 0)
        return (bmpheader_readerror(ret, file));
    if (bmp_header->type != 0x4d42) //424d in little endian --> first byte (42=B) is the smallest, second byte is bigger ((4d=M) * 256)
        return (bmp_header_error("file ID is %{r}d, it should be %{g}d\n", bmp_header->type, BMP_ID));//('M' * 256) + 'B');
    *offset += ret;
    if ((ret = read(fd, &bmp_header->size, 4)) <= 0)
        return (bmpheader_readerror(ret, file));
    if (bmp_header->size != BMP_SIZE)//<= 0)
        return (bmp_header_error("wrong size @ index %{b}d: %{r}d\n", *offset, bmp_header->size));
    *offset += ret;
    if (read_reserved_bmp(fd, bmp_header, offset, file) != EXIT_SUCCESS)
        return (EXIT_FAILURE);
    if ((ret = read(fd, &bmp_header->offset, 4)) <= 0)
        return (bmpheader_readerror(ret, file));
    if (bmp_header->offset != OFFSET_BEFORE_DATA)//54)
        return (bmp_header_error("offset to image data %{r}d @ index %{b}d wrong\n", bmp_header->offset, *offset));
    *offset += ret;
    return (EXIT_SUCCESS);
}

static int  check_info_header(t_info_header *info_header)
{
    if (info_header->size != 40)
        return(info_header_error("info header size: %{r}d, expected: 40\n", info_header->size));
    if (info_header->width != PIX_WIDTH) //128 * 128 ?
        return(info_header_error("width in header: %{r}d is wrong\n", info_header->width));
    if (ft_absolute(info_header->height) != PIX_HEIGHT) //128 * 128 ?
        return(info_header_error("height in header %{r}d is wrong\n", info_header->height));
    if (info_header->planes != 1)
        return(info_header_error("format error, color planes: %d\n", info_header->planes));
    if (info_header->bits != BITS_PER_PIX) //&& info_header->bits != 8) //only accept 24 bit bmp ?
        return(info_header_error("nb bits per pixel: %d, only 24 accepted\n", info_header->bits));
    if (info_header->compression != 0)
        return(info_header_error("compression: %{r}d, only uncompressed bmp are handled\n", info_header->compression));
    if (info_header->imagesize < 0)//!= IMG_SIZE)
        return(info_header_error("uncorrect image size: %{r}d\n", info_header->imagesize));
    if (info_header->xresolution <= 0)
        return(info_header_error("wrong horizontal res: %{r}d \n", info_header->xresolution));
    if (info_header->yresolution <= 0)
        return(info_header_error("uncorrect vertical resolution: %{r}d pixels per meter\n", info_header->yresolution));
    if (info_header->ncolours != 0)
        return(info_header_error("uncorrect number of colors in image: %{r}d\n", info_header->ncolours));
    if (info_header->importantcolours != 0)
        return(info_header_error("uncorrect number of important colors: %{r}d\n", info_header->importantcolours));
    return (EXIT_SUCCESS);
}

int         fill_info_header(t_info_header *info_header, int fd, char *file, int *offset)
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
    if (check_info_header(info_header) != EXIT_SUCCESS)
        return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
}
