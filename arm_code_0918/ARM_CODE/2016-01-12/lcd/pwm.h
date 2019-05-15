/*
 */

#ifndef __ASM_ARM_ARCH_PWM_H_
#define __ASM_ARM_ARCH_PWM_H_

/* Interval mode(Auto Reload) of PWM Timer 4 */
#define TCON3_AUTO_RELOAD	(1 << 19)
/* Update TCNTB4 */
#define TCON3_UPDATE		(1 << 17)
/* start bit of PWM Timer 4 */
#define TCON3_START		(1 << 16)

#define  CONFIG_SYS_HZ   1000

#ifndef __ASSEMBLY__
struct s5p_timer {
	unsigned int	tcfg0;
	unsigned int	tcfg1;
	unsigned int	tcon;
	unsigned int	tcntb0;
	unsigned int	tcmpb0;
	unsigned int	tcnto0;
	unsigned int	tcntb1;
	unsigned int	tcmpb1;
	unsigned int	tcnto1;
	unsigned int	tcntb2;
	unsigned int	tcmpb2;
	unsigned int	tcnto2;
	unsigned int	tcntb3;
	unsigned int	tcmpb3;
	unsigned int	tcnto3;
	unsigned int	tcntb4;
	unsigned int	tcnto4;
	unsigned int	tintcstat;
};
#endif	/* __ASSEMBLY__ */

void reset_timer_masked(void);
unsigned long get_timer_masked(void);
unsigned long get_tbclk(void);
unsigned long long get_ticks(void);
void __udelay(unsigned long usec);
int timer_init(void);
#endif
