
#include "bcd.h"


unsigned int bcd2bin(u8 val)
{
	return ((val) & 0x0f) + ((val) >> 4) * 10;
}

//static inline
//
u8 bin2bcd (unsigned int val)
{
	return (((val / 10) << 4) | (val % 10));
}
