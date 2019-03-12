/*****************************************************
Raspberri PI 3 GPIO definitions for FiC configuration
*****************************************************/


#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>

#define BLOCK_SIZE      4 * 1024
#define PERIPHERAL_BASE 0x3F000000
#define GPIO_BASE       PERIPHERAL_BASE + 0x00200000

typedef struct {
  unsigned long         gpio_base;
  int                   memory_fd;
  void                  *map;
  volatile unsigned int *addr;
} rpi_gpio;

int MapGPIO(rpi_gpio *gpio);
void UnmapGPIO(rpi_gpio *gpio);
void BlinkLED(rpi_gpio *gpio);
int write_config(rpi_gpio *gpio, FILE *fp1, FILE *fp2, FILE *fp3);
void init_test(rpi_gpio *gpio);
void write_test(rpi_gpio *gpio, int data);
void init_read(rpi_gpio *gpio );
int read_test(rpi_gpio *gpio );
//
void comwrite(rpi_gpio gpio, int j);
void addwrite(rpi_gpio gpio, int add);
void write_byte(rpi_gpio gpio, int d);
void write_cont(rpi_gpio gpio, int d);
int read_byte(rpi_gpio gpio );
int read_cont(rpi_gpio gpio );
void read_fin(rpi_gpio gpio );
void write_fin(rpi_gpio gpio );


/* general RasPI GPIO definitions */
#define GPFSEL0 0
#define GPFSEL1 1
#define GPFSEL2 2
#define GPFSEL3 3
#define GPFSEL4 4
#define GPFSEL5 5
#define GPSET0 7
#define GPSET1 8
#define GPCLR0 10
#define GPCLR1 11
#define GPLEV0 13
#define GPLEV1 14

#define FICINIT    0x00000010 // INIT: GPIO_04 input
#define FICPROGRAM 0x00000020 // PROGRAM: GPIO_05 output
#define FICDONE    0x00000040 // DONE: GPIO_06 input
#define FICCCLK    0x00000080 // CCLK: GPIO_07 output
#define FICRDWR    0x08000080 // RDWR: GPIO_27 output
#define FICCSI     0x04000080 // CSI:  GPIO_26 output

#define FICMASK 0x00ffff00 // CD0-CD15 on GPSET0/GPCLR0
#define FICMASK2 0x00cfff00 // CD0-CD15 on GPSET0/GPCLR0
// CD0: GPIO_08 output
// CD1: GPIO_09 output
// CD2: GPIO_10 output
// CD3: GPIO_11 output
//
// CD4: GPIO_12 output
// CD5: GPIO_13 output
// CD6: GPIO_14 output
// CD7: GPIO_15 output
//
// CD8: GPIO_16 output
// CD9: GPIO__17 output
// CD10: GPIO_18 output
// CD11: GPIO_19 output
//
// CD12: GPIO_20 input
// CD13: GPIO_21 input
// CD14: GPIO_22 output
// CD15: GPIO_23 output
// CD16: GPIO_24 input
// CD17: GPIO_25 input


/* bits for FiC configuration */
#define GPFSEL0_DAT 0x09208000 // set RPI GPIO[04..09], for GPFSEL0,  NOTE!!: should be ORed with GPIO[00..03] settings
#define GPFSEL1_DAT 0x09249249 // set RPI GPIO[10..19], for GPFSEL1 
#define GPFSEL2_DAT 0x00240249 // set RPI GPIO[20..23], for GPFSEL2,  NOTE!!: should be ORed with GPIO[24,25,28,29] settings
#define GPFSEL1_RD 0x00000009 // 





