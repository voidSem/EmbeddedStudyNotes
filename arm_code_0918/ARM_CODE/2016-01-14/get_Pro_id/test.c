

#include "res.h"
#include "Uart.h"
#include "rtc.h"
#include "serial.h"
#include "i2c.h"
#include "pwm.h"

static const unsigned char crc8_tab[] = {
0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15,
0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D,
0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65,
0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D,
0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5,
0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85,
0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,
0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2,
0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,
0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2,
0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32,
0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,
0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42,
0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,
0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C,
0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC,
0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,
0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C,
0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,
0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C,
0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B,
0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63,
0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B,
0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13,
0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB,
0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB,
0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3,
};

#define crc8_init(crc) ((crc) = 0XACU)
#define crc8(crc, v) ( (crc) = crc8_tab[(crc) ^(v)])

static volatile unsigned int io_data;
static volatile unsigned int io_bit_count;
static volatile unsigned char one_wire_request;
static unsigned last_req;
enum {
	IDLE,
	START,
	REQUEST,
	WAITING,
	RESPONSE,
	STOPING,
}bl_status = IDLE;
static print_t  printf = (void *)0x43E11A2C; 
static int count = 0 ; 
static unsigned int Code = 0 ; 
char buffer[32] = {0};
static unsigned int PicAddress = 0x63000000 ; 


void Reset(void);
void Undef(void);
void Svc(unsigned int SystemCallNum);
void PrefetchAbt(void);
void DataAbt(void);
void Irq(void);
void Fiq(void);
void init_timer();
void start_bl(unsigned char req);
void stop_timer();
int get_pin_value();
void set_pin_value(int v);
void set_pin_as_input();
void set_pin_as_output();

int _start(void)
{

    //2.copy vector
    vector_copy();

    *(unsigned int *)0x62000000 = (unsigned int)Reset;
    *(unsigned int *)0x62000004 = (unsigned int)Undef;
    *(unsigned int *)0x62000008 = (unsigned int)Svc;
    *(unsigned int *)0x6200000C = (unsigned int)PrefetchAbt;
    *(unsigned int *)0x62000010 = (unsigned int)DataAbt;
    *(unsigned int *)0x62000014 = (unsigned int)Irq;
    *(unsigned int *)0x62000018 = (unsigned int)Fiq;

    //IO 初始化
    GPIO_Init();

    //1.enable mmu
    enable_mmu();

    //3.clear cpsr I bit
    enable_cpsr_irq();

    //打印产品ＩＤ
    #define   PRO_ID     *(U32 *)0x10000000
    #define   PACKAGE_ID *(U32 *)0x10000004

    printf("product ID:%p ROM MainRev:%d  ROM SubRev:%d \n" ,    PRO_ID>>12 , (PRO_ID >>4)&0xF , PRO_ID&0xF );

    printf("PacketID:%p \n" , PACKAGE_ID);


#if 0   
    //开屏 
    enable_irq(0xF,0xF,72);

    init_timer();

    start_bl(127);


    delay(1000*1000);
    //定时器初始化
    //timer_init();

    

	//unsigned int irqno = 64 ; 
	//
	//0xF   0b1111    cpu0 - cpu3   interrupt ID
	//enable_irq(0xF ,0x1 , irqno);
	
    enable_external_irq(0x2 , 14);

#if 0
	//EXT
	//	0x0 = Low level 
	//	0x1 = High level 
	//	0x2 = Triggers Falling edge 
	//	0x3 = Triggers Rising edge 
	//	0x4 = Triggers Both edge 
	//	0x5 to 0x7 = Reserved 
	//	总共有外部中断32    0->31

	unsigned int tiggermode = 0x4 ; 
	unsigned int eirqno = 26 ; 

	enable_external_irq(tiggermode , 26);
#endif

    //初始化LCD
    Init_LCD(PicAddress);

#if 1
    while(1)
    {
        if(Code != 0)
        {
            VIDW00ADD0B0 = PicAddress + count*800*480*4;
            VIDW00ADD1B0 = PicAddress + (count+1)*800*480*4 ; 
            count = (count+1)%4 ; 
            //__udelay(1000*500);
            delay(1000*1000);
            Code = 0 ; 
        }
    }
#endif
#if 0
    int i ; 
    while(1)
    {
        for(i = 0 ; i < 4 ; i++)
        {
            VIDW00ADD0B0 = PicAddress + i*800*480*4;
            VIDW00ADD1B0 = PicAddress + (i+1)*800*480*4 ; 
            __udelay(1000*1000);
        }
    }
#endif

#endif
	return 0 ; 
}


void Reset(void)
{
    printf("this is reset ... \n");   
}
void Undef(void)
{
    printf("this is Undef... \n");   
}
void Svc(unsigned int SystemCallNum)
{
    printf("this is Svc... %d \n" , SystemCallNum);   
}
void PrefetchAbt(void)
{
    printf("this is PrefetchAbt... \n");   
}
void DataAbt(void)
{
    printf("this is DataAbt... \n");   
}


void Irq(void)
{
	//中断处理函数
	//不可以输出  因为输入输出函数都有效率问题  调试的时候可以打印
	//不可以阻塞，不可以睡眠
	//不可以操作浮点类型的算法

    if(clear_external_irq(14))
    {
        //VIDW00ADD0B0 = PicAddress + count*800*480*4;
        //VIDW00ADD1B0 = PicAddress + (count+1)*800*480*4 ; 
        //count = (count+1)%4 ; 
        Code = 1 ; 
    }

	//注意要清中断
	if(TINT_CSTAT & (1 << 8))
	{
	TINT_CSTAT |= 0x100;

	io_bit_count--;
	switch(bl_status){
	case START:
		if(io_bit_count == 0){
			io_bit_count = 16;
			bl_status = REQUEST;
		}
		break;
	case REQUEST:
		set_pin_value(io_data & (1 << 31));
		io_data <<= 1;
		if(io_bit_count == 0){
			io_bit_count = 2;
			bl_status = WAITING;
		}
		break;
	case WAITING:
		if(io_bit_count == 0){
			io_bit_count = 32;
			bl_status = RESPONSE;
		}
		if(io_bit_count == 1){
			set_pin_as_input();
			set_pin_value(1);
		}
		break;
	case RESPONSE:
		io_data = (io_data << 1)| get_pin_value();
		if(io_bit_count == 0){
			io_bit_count = 2;
			bl_status = STOPING;
			set_pin_value(1);
			set_pin_as_output();
		}
		break;
	case STOPING:
		if(io_bit_count == 0){
			bl_status = IDLE;
			stop_timer();
		}
		break;
	default:
		stop_timer();
	}
    }
}
void Fiq(void)
{
    printf("this is Fiq... \n");   
}

void init_timer()
{
	TCFG0 &= ~(0xff << 8);
	TCFG0 |= (0xf << 8);

	TCNTB3 = 650;
	
	TCON &= ~(0xf << 16);
	TCON |= (1 << 17);
	TCON &= ~(1 << 17);
	TCON |= (1 << 16)|(1 << 19);
}
void set_pin_as_output()
{
	GPX1CON &= ~(0xf << 8);
	GPX1CON |= 1 << 8;
}
void set_pin_as_input()
{
	GPX1CON &= ~(0xf << 8);
}
void set_pin_value(int v)
{
	if(v)
		GPX1DAT |= 1 << 2;
	else
		GPX1DAT &= ~(1 << 2);
}
int get_pin_value()
{
	int v;
	v = !!(GPX1DAT & (1 << 2));
	return v;
}

void start_bl(unsigned char req)
{
	bl_status = START;
	set_pin_value(1);
	set_pin_as_output();	

	{
	unsigned char crc;
	crc8_init(crc);  //crc=0xAC  req=127
	crc8(crc,req);   // crc = crc_tab[ 0xAC ^ 127]
	io_data = (req << 8) + crc;    //127<<8+crc
	io_data <<= 16;
	}

	last_req = (io_data >> 16);
	one_wire_request = req;
	io_bit_count = 1;
	set_pin_as_output();

	init_timer();
	TINT_CSTAT |= 0x108;

	set_pin_value(0);
}

void stop_timer()
{
	TCON &= ~(1 << 16);
}

