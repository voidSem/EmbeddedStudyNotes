
#include "res.h"


static print_t  printf = (void *)0x43E11A2C; 

int Init_LCD(unsigned int PicAddress)
{


    //1.可调时钟
    //打印CLK_DIV_LCD   确定LCD时钟源的频率
    #define    CLK_DIV_LCD    *(U32 *)0x1003C534
    printf("FIMD0_RATIO : %d \n" ,   CLK_DIV_LCD & 0xf);  //15
    //LCD_CLK  ==>   800M / 16 ==> 50M
    CLK_DIV_LCD &= ~0xF ; 
  //  CLK_DIV_LCD |= 7 ;   //100M
    printf("FIMD0_RATIO : %d \n" ,   CLK_DIV_LCD & 0xf);  //15
    
    //2.配置IO
    #define GPF0CON     *(U32 *)0x11400180
    #define GPF1CON     *(U32 *)0x114001A0
    #define GPF2CON     *(U32 *)0x114001C0
    #define GPF3CON     *(U32 *)0x114001E0
    
    GPF0CON  = 0x22222222 ; 
    GPF1CON  = 0x22222222 ; 
    GPF2CON  = 0x22222222 ; 
    GPF3CON  &= ~0xffff   ; 
    GPF3CON  |= 0x2222    ; 

    //3.系统寄存器 开ByPass 门
    #define    LCDBLK_CFG    *(U32 *)(0x10010210)
    LCDBLK_CFG |= 1 << 1 ; 
    

    //4.配置过程
    //1. VIDCON0: Configures video output format 
    //VIDCON0 |= 1 << 18 ;   //set serial format   
    //VIDCON0 |= 2 << 6 ;  //SCLK/3   100M/3  ==> 33.333M

    //2. VIDCON1: Specifies RGB I/F control signal.
        //1> let vclk running
        VIDCON1 |= 1 << 9 ; 
        //2> Fetches Video data at VCLK rising edge
        VIDCON1 |= 1 << 7 ; 
        //3> invert HSYNC   
        VIDCON1 |= 1 << 6 ; 
        //4> invert VSYNC
        VIDCON1 |= 1 << 5 ; 

  
    //3. VIDCON2: Specifies output data format control.
    //set RGB order no dummy no skip
    VIDCON2 |= 1 << 14 ;  //Reserve and must set 1
   
    //4. VIDTCONx: Configures video output timing and determines the size of display.
    VIDTCON0 &= ~0xFFFFFF ;  
    VIDTCON0 |= 10 | (22 << 8) | (13 << 16) ; 
    //VIDTCON0 |= 1 | (20 << 8) | (40 << 16) ; 
    VIDTCON1 &= ~0xFFFFFF ; 
    VIDTCON1 |= 20 | (210 << 8) | (26 << 16) ; 
    //VIDTCON1 |= 1 | (20 << 8) | (20 << 16) ; 
    VIDTCON2 = (479 << 11) | 799 ; 
  
    //5. WINCONx: Specifies each window feature setting.
    WINCON0 |= 0xB << 2 ;  ///RGB 888  24
    //enable
    WINCON0 |= 1 ; 
  
    //6. VIDOSDxA, VIDOSDxB: Specifies window position setting.
    VIDOSD0A = 0 ;   //set left top 0,0
    VIDOSD0B =  (800<<11) | 480 ;  //set right bottom
    //7. VIDOSDxC: Specifies On Screen Display (OSD) size setting.
    VIDOSD0C = 800 * 480 ; 

    //8. VIDWxxADDx: Specifies source image address setting.
    VIDW00ADD0B0 = PicAddress ;
    VIDW00ADD1B0 = PicAddress + 800*480*4 ; 
    VIDW00ADD2   = 800 ; 

   
    //9. SHADOWCON: Specifies shadow control register.
    SHADOWCON |= 1 ; 

    //10. displays enable/disable.  start display controller
    VIDCON0 |= 0x3 ; 

#define ISRAM_ADDRESS   0x02020000
#define EXTERNAL_FUNC_ADDRESS   (ISRAM_ADDRESS + 0x0030)
#define SDMMC_ReadBlocks(uStartBlk, uNumOfBlks, uDstAddr)   \
    (((void(*)(unsigned int, unsigned int, unsigned int*))(*((unsigned int *)(EXTERNAL_FUNC_ADDRESS))))(uStartBlk, uNumOfBlks, uDstAddr))

//    SDMMC_ReadBlocks(0x3A98, 0x1838 , (unsigned int *)PicCopyAddr);
    SDMMC_ReadBlocks(0x4000, 0x4000 , (unsigned int *)PicAddress);
    return 0 ; 
}
