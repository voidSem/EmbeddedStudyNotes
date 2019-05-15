
#ifndef _DEVICEIOCTL_H
#define _DEVICEIOCTL_H


#include <linux/ioctl.h>

#define   MAGIC   'X'

#define   LED_ON     _IO(MAGIC , 0)
#define   LED_OFF    _IO(MAGIC , 1)
#define   BUZZER_ON  _IO(MAGIC , 2)
#define   BUZZER_OFF _IO(MAGIC , 3)

#endif  //_DEVICEIOCTL_H
