/* APIs for FiC communication & file I/O */

#define CH0_ADDR 0x0000
#define CH1_ADDR 0x0100
#define HLS_ADDR 0x1000 // 0x1000 & 0x0200 used interchagably?

// used for table setting
#define FROM_HLS 0
#define FROM_CH0 1
#define FROM_CH1 2
#define IDEL_SLOT 3

#define DATA_WIDTH 128
#define RPI_DATA_WIDTH 4
#define DATABLOCK_NUMBER DATA_WIDTH / RPI_DATA_WIDTH // data is sent in blocks of RPi's input length
#define COM_START 1
#define COM_WRITE 2
#define COM_READ 3
#define COM_RESET 4

///// for DRAM ///////
#define BASE_DATA 0x1
#define BASE_ADDR 0x00000000
#define OFFSET 0x4000
#define END_ADDR 0xFFFFFFFF
#define DRAM_WR_DATA 0x1000 //continuous
#define DRAM_WR_ADDR 0x2000 // continuous
#define DRAM_HLS_WRITE 0xDDD1   // defined by Akram
#define DRAM_HLS_READ 0xDDD2
/////////////////////

#include "ficlib.h"
#include <time.h>
// #include <signal.h>

#define FLOAT_BIT_LENGTH 32
#define INTEGER_BIT_LENGTH 32
union float_and_uint {
    float fval;
    unsigned int ival;
};

// Caution: read/write funcions have to be called between read/write start and read/write finish
//////// write helpers ////////
void writeFloat(rpi_gpio gpio, float fl);             // for floats
void writeInt(rpi_gpio gpio, int c);
void writeShort(rpi_gpio gpio, short c);

//////// write helpers ////////
float readFloat(rpi_gpio gpio);
int readInt(rpi_gpio gpio);
unsigned short readShort(rpi_gpio gpio); // read out 16 bits

int read_to_array(char *path, float *array, int size);

//////// DRAM //////////
void writeIntArrayToDram(rpi_gpio gpio, long long *start_addr, int* arr, int len);
void readIntArrayFromDram(rpi_gpio gpio, long long *start_addr, int* arr, int len);
/* don't use it in application code */
void setDramAddress(rpi_gpio gpio, long long addr);


// to replace settbl program. sould be called after MapGPIO
int settbl(rpi_gpio gpio, char *path);

