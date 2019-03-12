#include "ficlib/ficlib_highlevel.h"

rpi_gpio gpio = {GPIO_BASE};
int main()
{
    // map RPi GPIO
    int map_status = MapGPIO(&gpio);
    if (map_status)
    {
        printf("Failed to map GPIO.\n");
        return map_status;
    }
    // don't remove
    init_test(&gpio);
    write_test(&gpio, HLS_ADDR);
    printf("Reset FPGA modules ...\n\n");
    comwrite(gpio, COM_RESET);
    printf("Start FPGA modules ...\n\n");
    comwrite(gpio, COM_START);
    // template ends

    int write_array[1024]; 
    int read_array[1024];
    for (int i=0; i<1024; i++)
    {
        write_array[i] = i;   
    }
    long long w_addr, r_addr = BASE_ADDR;
    // write
    writeIntArrayToDram(gpio, &w_addr, write_array, 1024);
    printf("write finish \n\n");
    usleep(100);
    // read
    readIntArrayFromDram(gpio, &r_addr, read_array, 1024);

    int err_counter;
    for (int i=0; i<1024; i++)
    {
        if (read_array[i] != write_array[i])
            err_counter ++;
    }
    printf("%d errors occured \n", err_counter);
}