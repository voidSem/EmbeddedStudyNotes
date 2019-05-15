# 1 "test.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "test.c"


# 1 "res.h" 1





# 1 "bcd.h" 1
# 15 "bcd.h"
typedef unsigned char u8 ;

unsigned int bcd2bin(u8 val);
u8 bin2bcd (unsigned int val);
# 7 "res.h" 2

typedef volatile unsigned int U32 ;
typedef volatile unsigned short U16 ;
typedef volatile unsigned char U8 ;

typedef int (*print_t)(const char *,...) ;
# 77 "res.h"
int led_all_on(void);
int led_all_off(void);
int led_which_on(int which);
int buzzer_on(void);
int buzzer_off(void);
int key_configure(void);
int get_key(void);
int delay(unsigned int val);
int led_configure(void);
int buzzer_configure(void);
int led_flash(void);
int led_run(void);
void __div0(void);




struct s5p_gpio_bank {
 unsigned int con;
 unsigned int dat;
 unsigned int pull;
 unsigned int drv;
 unsigned int pdn_con;
 unsigned int pdn_pull;
 unsigned char res1[8];
};


void gpio_cfg_pin(struct s5p_gpio_bank *bank, int gpio, int cfg);
void gpio_direction_output(struct s5p_gpio_bank *bank, int gpio, int en);
void gpio_direction_input(struct s5p_gpio_bank *bank, int gpio);
void gpio_set_value(struct s5p_gpio_bank *bank, int gpio, int en);
unsigned int gpio_get_value(struct s5p_gpio_bank *bank, int gpio);
void gpio_set_pull(struct s5p_gpio_bank *bank, int gpio, int mode);
void gpio_set_drv(struct s5p_gpio_bank *bank, int gpio, int mode);
void gpio_set_rate(struct s5p_gpio_bank *bank, int gpio, int mode);
# 4 "test.c" 2
# 1 "Uart.h" 1




typedef char *va_list;
# 5 "test.c" 2
# 1 "rtc.h" 1
# 26 "rtc.h"
struct rtc_time {
 int tm_sec;
 int tm_min;
 int tm_hour;
 int tm_mday;
 int tm_mon;
 int tm_year;
 int tm_wday;
 int tm_yday;
 int tm_isdst;
};

int rtc_get (struct rtc_time *);
int rtc_set (struct rtc_time *);
void rtc_reset (void);
# 6 "test.c" 2

print_t printf = (void *)0xC3E11ad0 ;


void putchar(char ch);
char getchar(void);
int puts(const char *str);
int print_num(int number , int base);


int uprintf(const char *fmt , ...);

int _start(void)
{





    *(U32 *)0x11400020 &= ~(0xff << 8) ;
    *(U32 *)0x11400020 |= 0x22 << 8 ;


    *(U32 *)(0x13800000 + 0x0000 ) = 0x3 ;


    *(U32 *)(0x13800000 + 0x0004 ) = 0x5 ;




    *(U32 *)(0x13800000 + 0x0028 ) = 53 ;


    *(U32 *)(0x13800000 + 0x002C ) = 4 ;

    char ch ;
    while(1)
    {
        ch = getchar();
        putchar(ch);
    }




 return 0 ;
}
# 62 "test.c"
void putchar(char ch)
{
    while(1)
    {
        if((*(U32 *)(0x13800000 + 0x0010 ) & (1 << 2)))
        {
            *(U32 *)(0x13800000 + 0x0020 ) = ch ;
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
        if(*(U32 *)(0x13800000 + 0x0010 ) & 1)
        {
            ch = *(U32 *)(0x13800000 + 0x0024 ) ;
            if(ch == '\r')
                putchar('\n');
            return ch ;
        }
    }
}

int uprintf(const char *fmt , ...)
{
    va_list ap ;

    (void) ((ap) = (((char *) &(fmt)) + ((((sizeof (fmt)) + ((sizeof (long) - 1))) & (~((sizeof (long) - 1)))))));

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

        ++fmt ;

        switch(*fmt)
        {
        case 's': puts(va_args(ap , char *)); break;
        case 'c': putchar((char)(*fmt)); break;
        case 'o': print_num(va_args(ap , int), 8); break;
        case 'x': print_num(va_args(ap , int), 16);break;
        case 'd': print_num(va_args(ap , int), 10);break;
        }
        ++fmt ;
    }

    (void) 0;
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


    print_num(number/base , base);

    putchar("0123456789abcdef"[number%base]);
}
