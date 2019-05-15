/*
 */
#include "res.h"
#include "cpu.h"
#include "pwm.h"

typedef  unsigned int u32 ; 

#define PRESCALER_1		(16 - 1)	/* prescaler of timer 2, 3, 4 */
#define MUX_DIV_2		1		/* 1/2 period */
#define MUX_DIV_4		2		/* 1/4 period */
#define MUX_DIV_8		3		/* 1/8 period */
#define MUX_DIV_16		4		/* 1/16 period */
#define MUX3_DIV_SHIFT		12

#define TCON_TIMER3_SHIFT	16

static unsigned long count_value;

/* Internal tick units */
static unsigned long long timestamp;	/* Monotonic incrementing timer */
static unsigned long lastdec;		/* Last decremneter snapshot */

/* macro to read the 16 bit timer */

static inline struct s5p_timer *s5p_get_base_timer(void)
{
	//return (struct s5p_timer *)samsung_get_base_timer();
	return (struct s5p_timer *)0x139D0000;
}

int timer_init(void)
{
	struct s5p_timer *const timer = s5p_get_base_timer();
	u32 val;

	/*
	 * @ PWM Timer 4
	 * Timer Freq(HZ) =
	 *	PWM_CLK / { (prescaler_value + 1) * (divider_value) }
	 */
    //100M  16  2   
	/* set prescaler : 16 */
	/* set divider : 2 */
	writel((PRESCALER_1 & 0xff) << 8, &timer->tcfg0);
	writel((MUX_DIV_2 & 0xf) << MUX3_DIV_SHIFT, &timer->tcfg1);
	/* count_value = 2085937.5(HZ) (per 1 sec)*/
	//count_value = get_pwm_clk() / ((PRESCALER_1 + 1) *
	//		(MUX_DIV_2 + 1));    3125000/s
	count_value = 3125000;

	/* count_value / 100 = 20859.375(HZ) (per 10 msec) */
	count_value = count_value / 100;

	/* set count value */
	writel(count_value, &timer->tcntb3);
	lastdec = count_value;

	val = (readl(&timer->tcon) & ~(0x07 << TCON_TIMER3_SHIFT)) |
		TCON3_AUTO_RELOAD;

	/* auto reload & manual update */
	writel(val | TCON3_UPDATE, &timer->tcon);

	/* start PWM timer 4 */
	writel(val | TCON3_START, &timer->tcon);

	timestamp = 0;

	return 0;
}

/*
 * timer without interrupts
 */
void reset_timer(void)
{
	reset_timer_masked();
}

unsigned long get_timer(unsigned long base)
{
	return get_timer_masked() - base;
}

void set_timer(unsigned long t)
{
	timestamp = t;
}

/* delay x useconds */
void __udelay(unsigned long usec)
{
	struct s5p_timer *const timer = s5p_get_base_timer();
	unsigned long tmo, tmp;

	count_value = readl(&timer->tcntb3);

	if (usec >= 1000) {
		/*
		 * if "big" number, spread normalization
		 * to seconds
		 * 1. start to normalize for usec to ticks per sec
		 * 2. find number of "ticks" to wait to achieve target
		 * 3. finish normalize.
		 */
		tmo = usec / 1000;
		tmo *= (CONFIG_SYS_HZ * count_value / 10);  //1000
		tmo /= 1000;
	} else {
		/* else small number, don't kill it prior to HZ multiply */
		tmo = usec * CONFIG_SYS_HZ * count_value / 10;
		tmo /= (1000 * 1000);
	}

	/* get current timestamp */    //10ms  走了多少HZ
	tmp = get_timer(0);

	/* if setting this fordward will roll time stamp */
	/* reset "advancing" timestamp to 0, set lastdec value */
	/* else, set advancing stamp wake up time */
	if ((tmo + tmp + 1) < tmp)
		reset_timer_masked();
	else
		tmo += tmp;

	/* loop till event */
	while (get_timer_masked() < tmo)
		;	/* nop */
}

void reset_timer_masked(void)
{
	struct s5p_timer *const timer = s5p_get_base_timer();

	/* reset time */
	lastdec = readl(&timer->tcnto3);
	timestamp = 0;
}

unsigned long get_timer_masked(void)
{
	struct s5p_timer *const timer = s5p_get_base_timer();
	unsigned long now = readl(&timer->tcnto3);

	if (lastdec >= now)
		timestamp += lastdec - now;
	else
		timestamp += lastdec + count_value - now;

	lastdec = now;

	return timestamp;
}

/*
 * This function is derived from PowerPC code (read timebase as long long).
 * On ARM it just returns the timer value.
 */
unsigned long long get_ticks(void)
{
	return get_timer(0);
}

/*
 * This function is derived from PowerPC code (timebase clock frequency).
 * On ARM it returns the number of timer ticks per second.
 */
unsigned long get_tbclk(void)
{
	return CONFIG_SYS_HZ;
}
