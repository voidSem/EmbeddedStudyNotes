
#include "res.h"

static void mk_ttb_self(unsigned int *ttb)
{//基础内存
    unsigned int va , pa ; 

    //1.系统默认内存
    for(va = 0x00000000 ; va < 0x10000000 ; va+=0x100000)
    {
        pa = va ; 
        ttb[(va>>20)] = (pa & (0xfff<< 20)) | 2 ;   
    }
    //2.SFR
    for(va = 0x10000000 ; va < 0x14000000 ; va+=0x100000)
    {
        pa = va ; 
        ttb[(va>>20)] = (pa & (0xfff<< 20)) | 2 ;   
    }
    //3.DRAM mmap itself
    for(va = 0x40000000 ; va < 0x80000000 ; va+=0x100000)
    {
        pa = va ; 
        ttb[(va>>20)] = (pa & (0xfff<< 20)) | 2 ;   
    }
}

static void mmap(unsigned int va , unsigned int pa , unsigned int *ttb)
{//自已做额外的映射
    ttb[(va>>20)] = (pa & (0xfff<< 20)) | 2 ;   
}

void enable_mmu(void)
{

    //内存必须以4B对齐 
    unsigned int  va = 0xFFFF0000; 
    unsigned int  pa = 0x5FFF0000; 
    unsigned int  ttb =0x61000000; 

    unsigned int  sysctl_r = 1 | (1 << 11) | (1 << 13) ; 
    //1  enable mmu
    //11 enable branch
   
    
    //before enable mmu
    
    //clean ttb address   16K  ttb start
    memset((char *)ttb , 0 , 16*1024);

    //make  ttb
    mk_ttb_self((unsigned int *)ttb);

    //mmap   每次映射只能1M
    mmap(va , pa , (unsigned int *)ttb);  //ttb  OK

    //enable mmu
   
   __asm__ __volatile__ (
    //1.set Domain Access Control Register   给权限
//    "ldr    r0 , =0xffffffff   \n"  //manager
    "mvn    r0 , #0            \n"  //取反
    "MCR p15, 0, r0, c3, c0, 0 \n"  //write

    //2.write ttb
    "mov    r0 , %0              \n"
    "MCR p15, 0, r0, c2, c0, 0   \n"

    //3.enable ttb
    "mov    r0 , %1              \n"
    "MRC p15, 0, r1, c1, c0, 0   \n"
    "orr    r1 , r1 , r0         \n"
    "MCR p15, 0, r1, c1, c0, 0   \n"

    ::"r"(ttb),"r"(sysctl_r)
    :"r0","r1"
    ); 
}
