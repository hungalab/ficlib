#include "ficlib_highlevel.h"

void writeShort(rpi_gpio gpio, short c)
{
    for (int i = 1; i <= 16 / RPI_DATA_WIDTH; i++)
    {
        write_cont(gpio, c >> (16 - i * RPI_DATA_WIDTH)); // MSB first
    }
}

void writeInt(rpi_gpio gpio, int c)
{
    for (int i = 1; i <= INTEGER_BIT_LENGTH / RPI_DATA_WIDTH; i++)
    {
        write_cont(gpio, c >> (INTEGER_BIT_LENGTH - i * RPI_DATA_WIDTH)); // MSB first
    }
}

void writeFloat(rpi_gpio gpio, float fl)
{
    union float_and_uint float_to_int;
    float_to_int.fval = fl;
    unsigned int integer = float_to_int.ival;
    // printf("%x  ", integer);

    for (int i = 1; i <= FLOAT_BIT_LENGTH / RPI_DATA_WIDTH; i++)
    {
        write_cont(gpio, integer >> (FLOAT_BIT_LENGTH - i * RPI_DATA_WIDTH)); // MSB first
    }
}

unsigned short readShort(rpi_gpio gpio)
{
    unsigned short short_val = 0;
    for (int l = 1; l <= 16 / RPI_DATA_WIDTH; l++)
    {
        unsigned int tmp = read_cont(gpio);
        short_val |= tmp << (16 - l * RPI_DATA_WIDTH); // MSB first
    }
    return short_val;
}

int readInt(rpi_gpio gpio)
{
    int int_val = 0;
    for (int l = 1; l <= INTEGER_BIT_LENGTH / RPI_DATA_WIDTH; l++)
    {
        unsigned int tmp = read_cont(gpio) & 0xf;
        int_val |= tmp << (INTEGER_BIT_LENGTH - l * RPI_DATA_WIDTH); // MSB first
    }
    return int_val;
}

float readFloat(rpi_gpio gpio)
{
    union float_and_uint int_to_float;
    for (int l = 1; l <= FLOAT_BIT_LENGTH / RPI_DATA_WIDTH; l++)
    {
        unsigned int tmp = read_cont(gpio);
        int_to_float.ival |= tmp << (FLOAT_BIT_LENGTH - l * RPI_DATA_WIDTH); // MSB first
    }
    return int_to_float.fval;
}

void setDramAddress(rpi_gpio gpio, long long addr)
{
    comwrite(gpio, COM_WRITE);
    addwrite(gpio, DRAM_WR_ADDR);
    for (int k = 0; k < 24; k++)
    {
        write_cont(gpio, 0);
    }
    // So is the address 64 bit or 32 bit???
    for (int l = 7; l >= 0; l--)
    {
        write_cont(gpio, (addr >> (l * 4)) & 0xf);
    }
    write_fin(gpio);
}

void writeIntArrayToDram(rpi_gpio gpio, long long *start_addr, int *arr, int len)
{
    setDramAddress(gpio, (*start_addr));
    comwrite(gpio, COM_WRITE); // start hls_write
    addwrite(gpio, DRAM_HLS_WRITE);
    write_byte(gpio, 1); // MAGIC NUMBER

    comwrite(gpio, COM_WRITE); // write data
    addwrite(gpio, DRAM_WR_DATA);
    for (int l = 0; l < len; l++)
    {
        int val = arr[l];
        writeInt(gpio, val);
    }
    write_fin(gpio);
    *start_addr += (len * 32) / 8; // 8 = byte
}

void readIntArrayFromDram(rpi_gpio gpio, long long *start_addr, int *arr, int len)
{
    setDramAddress(gpio, (*start_addr));
    comwrite(gpio, COM_WRITE); // start hls_read
    addwrite(gpio, DRAM_HLS_READ);
    write_byte(gpio, 1); // MAGIC NUMBER

    comwrite(gpio, COM_READ);
    addwrite(gpio, DRAM_WR_DATA);
    init_read(&gpio);
    fflush(stdout);
    for (int l = 0; l < len; l++)
    {
        arr[l] = readInt(gpio);
    }
    read_fin(gpio);
    *start_addr += (len * 32) / 8;
}

int read_to_array(char *path, float *array, int size)
{
    FILE *fp;
    if ((fp = fopen(path, "r")) == NULL)
    {
        return -1;
    }
    for (int i = 0; i < size; i++)
        if (fscanf(fp, "%f\n", &array[i]) != '\0')
            ;
    fclose(fp);
    return 0;
}

int settbl(rpi_gpio gpio, char *path)
{
    FILE *fp;
    if ((fp = fopen(path, "r")) == NULL)
    {
        return -1;
    }
    init_test(&gpio);
    write_test(&gpio, 0x000);
    int port, slot, addr, src;
    fscanf(fp, "%d %d\n", &port, &slot);
    for (int i = 0; i < port; i++)
    {
        fscanf(fp, "%x \n", &addr);
        printf("port:%x\n", i);
        for (int k = 0; k < slot; k++)
        {
            fscanf(fp, "%d\n", &src);
            comwrite(gpio, COM_WRITE);
            addwrite(gpio, addr);
            write_byte(gpio, src);
            printf("data goes to:%x, data comes from:%x\n", addr, src);
            addr++;
        }
    }
    return 0;
}