#include "../ficlib/ficlib_highlevel.h"
#define ARRAY_SIZE 4096 // burst size defined in HLS 

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

    int write_array[ARRAY_SIZE]; 
    int read_array[ARRAY_SIZE];
    for (int i=0; i<ARRAY_SIZE; i++)
    {
        write_array[i] = i;   
    }
    long long w_addr, r_addr = BASE_ADDR;
    // write
    writeIntArrayToDram(gpio, &w_addr, write_array, ARRAY_SIZE);
    printf("write finish \n\n");
    // read
    readIntArrayFromDram(gpio, &r_addr, read_array, ARRAY_SIZE);

    int err_counter;
    for (int i=0; i<ARRAY_SIZE; i++)
    {
        if (read_array[i] != write_array[i])
            err_counter ++;
    }
    printf("%d errors occured \n", err_counter);

    init_test(&gpio);
	UnmapGPIO(&gpio);
}
