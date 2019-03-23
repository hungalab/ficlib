#include "../ficlib/ficlib_highlevel.h"

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

    // your code

    UnmapGPIO(&gpio);
}