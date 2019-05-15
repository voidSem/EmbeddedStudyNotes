
#ifndef  _BL_OP_H
#define  _BL_OP_H


#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <time.h>

int Min_Light(void);
int Max_Light(void);
int Sub_Light(void);
int Add_Light(void);
void Exit_Backlight(void);
void Init_Backlight(void);



#endif  //_BL_OP_H
