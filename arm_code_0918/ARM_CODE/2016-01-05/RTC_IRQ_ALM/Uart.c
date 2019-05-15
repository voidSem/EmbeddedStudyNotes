
#include "Uart.h"


void Init_Uart(void)
{
    //1>   先接硬件电路 配置IO
    //clear
//    GPA1CON &= ~0xff ; 
    //set RX  TX
//    GPA1CON |= 0x22 ; 
    GPA1CON &= ~(0xff << 8) ; 
    GPA1CON |= 0x22 << 8 ; 

    //set 8N1         
    ULCONn = 0x3 ; 

    //set polling mode
    UCONn = 0x5 ; 

    //set Baudrate as 115200
    //Pclk = 100M
    //100M/115200/16=>54.2534   -1  => 53.2534
    UBRDIVn = 53 ; 

    //0.2534*16 ==> 4
    UFRACVALn  = 4 ; 
}

void putchar(char ch)
{
    while(1)
    {
        if((UTRSTATn & (1 << 2)))
        {
            UTXHn = ch ; 
            if(ch == '\n')
                putchar('\r');
            break;
        }
    }
}

char getchar(void)
{
    char ch ; 
    while(1)
    {
        if(UTRSTATn & 1)
        {
            ch = URXHn ; 
            if(ch == '\r')
                putchar('\n');
            return ch ; 
        }
    }   
}

int uprintf(const char *fmt , ...)
{
    va_list  ap ; 
    
    va_start(ap , fmt);

    while(*fmt)
    {
        while(*fmt)
        {
            if(*fmt != '%')       
            {
                putchar(*fmt++);
                continue ; 
            }else
                break;
        }
    
        if(*fmt == '\0')
            return ; 

        ++fmt ; 

        switch(*fmt)
        {
        case 's':  puts(va_arg(ap , char *)); break;  
        case 'c':  putchar(va_arg(ap,char));  break;
        case 'o':  print_num(va_arg(ap , int), 8); break;
        case 'x':  print_num(va_arg(ap , int), 16);break;
        case 'd':  print_num(va_arg(ap , int), 10);break;
        default :  break;
        }
        ++fmt ; 
    }

    va_end(ap);
}

int puts(const char *str)
{
    while(*str)
        putchar(*str++);
}

int print_num(int number , int base)
{
    if(number == 0)
        return ; 

    //过
    print_num(number/base , base);
    //回
    putchar("0123456789abcdef"[number%base]);   
}

int gets(char *buffer)
{
    char ch ; 
    while(1)
    {
        ch = getchar();
        *buffer++ = ch ; 
        if(ch == '\r')
        {
            *(--buffer) = '\0';
            break;
        }
        putchar(ch);
    }
}
