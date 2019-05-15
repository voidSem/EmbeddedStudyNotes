#ifndef __SERIAL_H__
#define __SERIAL_H__

#define NAMESIZE 16
#define CTLRSIZE 8

typedef unsigned int u32 ; 
#define  NULL  0


struct serial_device {
	char name[NAMESIZE];
	char ctlr[CTLRSIZE];

	int  (*init) (void);
	int  (*uninit) (void);
	void (*setbrg) (void);
	int (*getc) (void);
	int (*tstc) (void);
	void (*putc) (const char c);
	void (*puts) (const char *s);

	struct serial_device *next;
};

extern struct serial_device s5p_serial0_device;
extern struct serial_device s5p_serial1_device;
extern struct serial_device s5p_serial2_device;
extern struct serial_device s5p_serial3_device;


#endif
