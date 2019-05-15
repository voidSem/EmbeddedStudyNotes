/* Permission is hereby granted to copy, modify and redistribute this code
 * in terms of the GNU Library General Public License, Version 2 or later,
 * at your option.
 */

/* inline functions to translate to/from binary and binary-coded decimal
 * (frequently found in RTC chips).
 */

#ifndef _BCD_H
#define _BCD_H

//内联

typedef   unsigned  char  u8 ; 

unsigned int bcd2bin(u8 val);
u8 bin2bcd (unsigned int val);


#endif /* _BCD_H */
