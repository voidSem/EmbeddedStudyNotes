
#ifndef  _MYIOCTL_H
#define  _MYIOCTL_H

//标准ioctl规定方法
#include <linux/ioctl.h>

/*
 *
 * IOCTL CMD
 *
 * */

#define  MAGIC        'K'
#define  BUZZER_ON    _IO(MAGIC, 0x0001)
#define  BUZZER_OFF   _IO(MAGIC, 0x0002)


#endif   //_MYIOCTL_H
