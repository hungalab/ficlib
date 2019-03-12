#include "ficlib.h"
// #define DEBUG

/* Fic library */

void comwrite(rpi_gpio gpio, int com) {
	int d,tdata,data;
	tdata = com<<4; data = 0xc00 | tdata; write_test(&gpio,data);  // req=1 stb=1 Write Start=1
	do {
		d= read_test(&gpio);
#ifdef DEBUG
		printf("CMD %d write %x %x\n", com, d,data);
#endif
	}while((0x00100000 & d) == 0x00000000) ; // wait while fak=0
	if(com==1| com==4 ) data = 0x000;  //req=0, stb=0
	else data = 0x800; // stb=0
	write_test(&gpio,data);  
	do {
		d= read_test(&gpio);
#ifdef DEBUG
		printf("CMD %d write end %x\n",com, d);
#endif
	}while((0x00100000 & read_test(&gpio)) == 0x00100000) ; // wait while fak=1
}

void addwrite(rpi_gpio gpio, int add) {
		int data,d;
		data = 0xc00 | (0xf000 & add)>>8 ; write_test(&gpio,data);  // stb=1  Add 15:12
		do {
			d= read_test(&gpio);
		#ifdef DEBUG
			printf("Add 15:12 %x \n", d);
		#endif 
	    }while( (0x00100000 & d ) == 0x00000000) ; // wait while fak=0

		data = 0x800; write_test(&gpio,data);  // stb=0
		do {
			d= read_test(&gpio);
		#ifdef DEBUG
			printf("Add 15:12 end %x \n", d);
		#endif 
		}while((0x00100000 & d) == 0x00100000) ; // wait while fak=1

		data = 0xc00 | (0xf00 & add)>>4; write_test(&gpio,data);  // stb=1  Add 11:8
		do {
			d= read_test(&gpio);
		#ifdef DEBUG
			printf("Add 11:8 %x\n", d);
		#endif 
	    }while( (0x00100000 & d) == 0x00000000) ; // wait while fak=0

		data = 0x800; write_test(&gpio,data);  // stb=0
		do {
			d= read_test(&gpio);
		#ifdef DEBUG
			printf("Add 11:8 end %x\n", d);
		#endif 
		}while((0x00100000 & d) == 0x00100000) ; // wait while fak=1

		data = 0xc00 | 0xf0 & add ; write_test(&gpio,data);  // stb=1  Add 7:4
		do {
			d= read_test(&gpio);
		#ifdef DEBUG
			printf("Add 7:4 %x\n", d);
		#endif 
	    }while( (0x00100000 & d) == 0x00000000) ; // wait while fak=0

		data = 0x800; write_test(&gpio,data);  // stb=0
		do {
			d= read_test(&gpio);
		#ifdef DEBUG
			printf("Add 7:4 end %x \n", d);
		#endif 
		}while((0x00100000 & d) == 0x00100000) ; // wait while fak=1

		data = 0xc00 | (0xf & add)<<4 ; write_test(&gpio,data);  // stb=1  Add 3:0 
		do {
			d= read_test(&gpio);
		#ifdef DEBUG
			printf("Add 3:0 %x\n", d);
		#endif 
	    }while( (0x00100000 & d) == 0x00000000) ; // wait while fak=0

		data = 0x800; write_test(&gpio,data);  // stb=0
		do {
			d= read_test(&gpio);
		#ifdef DEBUG
			printf("Add 3:0 end %x\n", d);
		#endif 
	    }while( (0x00100000 & d) == 0x00100000) ; // wait while fak=1
}

void write_byte (rpi_gpio gpio, int d) {
		int data, rd;
		data = 0xc00 | 0xf0 & d ; write_test(&gpio,data);  // stb=1  data
		do {
			rd=read_test(&gpio);
		#ifdef DEBUG
			printf("Data Upper write %x\n", rd);
		#endif 
	    }while( (0x00100000 & rd ) == 0x00000000) ; // wait while fak=0
		data = 0x800; write_test(&gpio,data);  // stb=0 
		do {
			rd=read_test(&gpio);
		#ifdef DEBUG
			printf("Data Upper write end %x\n", rd);
		#endif 
		}while((0x00100000 & read_test(&gpio)) == 0x00100000) ; // wait while fak=1
		data = 0xc00 | (0xf & d)<<4 ; write_test(&gpio,data);  // stb=1  data
		do {
			rd=read_test(&gpio);
		#ifdef DEBUG
			printf("Data Lower write %x\n", rd);
		#endif 
	 	}while( (0x00100000 & read_test(&gpio)) == 0x00000000) ; // wait while fak=0
		data = 0x000; write_test(&gpio,data);  // stb=0 req=0
		do {
			rd=read_test(&gpio);
		#ifdef DEBUG
			printf("Data Lower write end %x\n", rd);
		#endif 
		}while((0x00100000 & rd) == 0x00100000) ; // wait while fak=1
}

void write_cont(rpi_gpio gpio, int d){
		int data,rd,rd1,rd2;
		int i;
		data = 0xc00 | (0xf & d)<<4 ; write_test(&gpio,data);  // stb=1  data
		rd=read_test(&gpio); rd1=0; rd2=1;
		do {
			rd2=rd1; rd1=rd; rd=read_test(&gpio);
		#ifdef DEBUG
			printf("Data %x \n", rd);
		#endif 
	    }while( ((0x00100000 & rd ) == 0x00000000) || (rd1!=rd) || (rd2!=rd1) ) ; // wait while fak=0
		data = 0x800; write_test(&gpio,data);  // stb=0
		rd=read_test(&gpio); rd1=0; rd2=1;
		do {
			rd2=rd1; rd1=rd; rd=read_test(&gpio);
		#ifdef DEBUG
		   printf("Data end %x\n", rd);
		#endif 
		}while(( (0x00100000 & rd) == 0x00100000) || (rd1!=rd) ||(rd2!=rd1) ) ; // wait while fak=1
}

int read_byte(rpi_gpio gpio) {
		int data,d,rd,d1,d2;
		init_read(&gpio );
		data = 0xc00 ; write_test(&gpio,data);  // stb=1  reading data
		d=read_test(&gpio); d1=0; d2=1;
		do {
			d2=d1; d1=d; d=read_test(&gpio); 
		#ifdef DEBUG
			printf("Read Data Upper %x \n", d);
		#endif 
	    }while( ( (0x00100000 & d) == 0x00000000) || (d1!=d)||(d2!=d1)) ; // wait while fak=0 
		#ifdef DEBUG
		printf("Read result %x\n", 0xf&(d>>16) );
		#endif 
		rd = 0xf0 &(d>>12);
		data = 0x800; write_test(&gpio,data);  // stb=0
		d2=d1; d1=d; d=read_test(&gpio); 
		do {
			d2=d1; d1=d; d=read_test(&gpio);
		#ifdef DEBUG
			printf("Read data Upper end %x\n", d);
		#endif 
		}while( ((0x00100000 & d) == 0x00100000)|| (d1!=d)||(d2!=d1) ) ; // wait while fak=1

		data = 0xc00 ; write_test(&gpio,data);  // stb=1  reading data
		d=read_test(&gpio); d1=0; d2=1;
		do {
			d2=d1; d1=d; d=read_test(&gpio);
		#ifdef DEBUG
			printf("Read Data Lower %x \n", d);
		#endif 
	    }while( ( (0x00100000 & d) == 0x00000000) | (d1!=d)||(d2!=d1)) ; // wait while fak=0 
		rd = rd | 0xf&(d>>16);
		data = 0x000; write_test(&gpio,data);  // req = 0 stb=0
		do {
			d2=d1; d1=d; d=read_test(&gpio);
		#ifdef DEBUG
			printf("Read data Lower end %x\n", d);
		#endif 
		}while( ((0x00100000 & d ) == 0x00100000)|(d1!=d)||(d2!=d1)) ; // wait while fak=1
		init_test(&gpio);
		return rd;
}
int read_cont(rpi_gpio gpio) {
		int data,d,d1,rd, d2;
		data = 0xc00 ; write_test(&gpio,data);  // stb=1  reading data
		d=read_test(&gpio); d1=0; d2=1;
		do {
			d2=d1; d1=d; d=read_test(&gpio);
		#ifdef DEBUG
			printf("Read Data %x \n", d);
		#endif 
	   	}while( ( (0x00100000 & d) == 0x00000000) || (d1!=d)||(d2!=d1)) ; // wait while fak=0 
		rd = d;
		data = 0x800; write_test(&gpio,data);  // stb=0
		d2=d1; d1=d; d=read_test(&gpio);
		do {
			d2=d1; d1=d; d=read_test(&gpio);
		#ifdef DEBUG
			printf("Read data end %x\n", d);
		#endif 
		}while(( (0x00100000 & d) == 0x00100000)||(d1!=d) ||(d2!=d1)) ; // wait while fak=1
		return (0xf&(rd>>16));
}
void write_fin(rpi_gpio gpio){
    int data,d;
    data = 0x000; write_test(&gpio,data);  // req=0, stb=0
    do {
	        d=read_test(&gpio);
		#ifdef DEBUG
	        printf("Write Request end %x\n",d);
		#endif 
    }while((0x00200000 & d) == 0x00200000) ; // wait while req=1
}

void read_fin(rpi_gpio gpio) {
	int data, d;
	data = 0x000 ; write_test(&gpio,data);  // stb=1  reading data
	do {
		d= read_test(&gpio);
		#ifdef DEBUG
			printf("Read request end %x \n", d);
		#endif 
	}while((0x00200000 & d) == 0x00200000) ; // wait while req=1
	init_test(&gpio);
}

/* Fic Basic library */

int MapGPIO(rpi_gpio *gpio) {
	gpio->memory_fd = open("/dev/mem", O_RDWR|O_SYNC);

	if(gpio->memory_fd < 0) {
		perror("Failed to open /dev/mem, try change permission.");
		return 1;
	}

	gpio->map = mmap(
		NULL,
		BLOCK_SIZE,
		PROT_READ|PROT_WRITE,
		MAP_SHARED,
		gpio->memory_fd,
		gpio->gpio_base
	);

	if(gpio->map == MAP_FAILED) {
		perror("mmap");
		return 1;
	}

	gpio->addr = (volatile unsigned int *)gpio->map;
	return 0;
}

void UnmapGPIO(rpi_gpio *gpio) {
	munmap(gpio->map, BLOCK_SIZE);
	close(gpio->memory_fd);
}

void init_test(rpi_gpio *gpio ) {
	*(gpio->addr +GPFSEL0)  = GPFSEL0_DAT;
	*(gpio->addr +GPFSEL1)  = GPFSEL1_DAT;  // 8bit output
	*(gpio->addr +GPFSEL2)  = 0x00000240;   // out out in in
}
int read_test(rpi_gpio *gpio ) {
		int idata;
		idata = *(gpio->addr + GPLEV0); 
		return(idata);
		// fprintf(stderr, "SW: %x Count:%x \n", (idata>>22)&3, (idata>>20)&3); 
}
void init_read(rpi_gpio *gpio ) {
    *(gpio->addr +GPFSEL0)  = GPFSEL0_DAT;
	    *(gpio->addr +GPFSEL1)  = 0x00000009;  // 8bit input
		    *(gpio->addr +GPFSEL2)  = 0x00000240;   // out out in in
			}


void write_test(rpi_gpio *gpio, int data ) {
	data = data << 12;
	*(gpio->addr + GPCLR0) = (~data & FICMASK2);
	*(gpio->addr + GPSET0)  = data & FICMASK2;													// set bits
}

int time_consume() {
	int i;
	for(i=0; i<100000000; i++) ;
}
