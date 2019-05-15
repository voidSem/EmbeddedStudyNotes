
@SFR
@--> led
#define     GPM4CON    0x110002E0
#define     GPM4DAT	   0x110002E4
@--> buzzer
#define     GPD0CON    0x114000A0
#define     GPD0DAT    0x114000A4
@--> key
#define     GPX3CON    0x11000C60
#define     GPX3DAT    0x11000C64

.global _start
_start:

	stmfd sp! , {r0-r12 , lr}
	@初始化硬件

	bl	led_configure
	bl	buzzer_configure
	bl	key_configure

#if 0
	@while  :
	loop:
		bl	led_all_on	
		bl	delay
		bl	led_all_off
		bl	delay
	b	loop

	mov	r0 , #0

	loop:
	@ 0 - 3
	
	bl	buzzer_on
	bl	led_which_on
	bl	delay
	bl	led_all_off
	bl	buzzer_off
	bl	delay

	add	r0 , #1
	cmp	r0 , #4
	moveq	r0 , #0

	b	loop
#endif 

	loop:

	bl	get_key
	@确定有人按下并且传参回来r0

	bl	led_which_on
	bl	buzzer_on
	
	@软件防抖
	bl	delay
	
	bl	led_all_off
	bl	buzzer_off

	cmp	r0 , #3
	bne	loop


	ldmfd sp! , {r0-r12 , pc}

.global	led_all_off
led_all_off:
	stmfd sp! , {r0-r12 , lr}

	ldr	r0 , =GPM4DAT
	@read
	ldrb	r1 , [r0]
	@set low 4 bits as hight
	orr	r1 , r1 , #0xf
	@write
	strb	r1 , [r0]

	ldmfd sp! , {r0-r12 , pc}

.global	led_all_on
led_all_on:
	stmfd sp! , {r0-r12 , lr}

	ldr	r0 , =GPM4DAT
	@read
	ldrb	r1 , [r0]
	@set low 4 bits as low
	bic r1 , r1 , #0xf
	@write
	strb	r1 , [r0]

	ldmfd sp! , {r0-r12 , pc}


.global	led_configure
led_configure:
	stmfd sp! , {r0-r12 , lr}

	@configure led as output
	ldr	r0 , =GPM4CON
	@read register
	ldr	r1 , [r0]

	@清掉低16位
	ldr	r2 , =0xffff
	bic	r1 , r1 , r2

	@设置成输出   0x1111
	ldr	r2 , =0x1111
	orr	r1 , r1 , r2

	@write regster
	str	r1 , [r0]

	@set led off as default
	bl	led_all_off

	ldmfd sp! , {r0-r12 , pc}

.global delay
delay:
	stmfd sp! , {r0-r12 , lr}

	@slow
@	ldr	 r0 , =0xfffffff
	@fast
	ldr	 r0 , =0x4ffffff

	delay_again:
	subs	r0 , #1
	bne	delay_again

	ldmfd sp! , {r0-r12 , pc}

@led_which_on   r0为参数  值为0-3
.global led_which_on
led_which_on:
	stmfd sp! , {r0-r12 , lr}

	ldr r1 , =GPM4DAT
	mov	r3 , #1

	@read
	ldrb	r2 , [r1]
	
	@configure
	bic	r2 , r3 , lsl r0
	
	@write
	strb	r2 , [r1]

	ldmfd sp! , {r0-r12 , pc}

.global buzzer_configure
buzzer_configure:
	stmfd sp! , {r0-r12 , lr}
	
	ldr	r0 , =GPD0CON
	@read
	ldr	r1 , [r0]

	@output
	@clear
	bic	r1 , r1 , #0xf

	@set as output
	orr	r1 , r1 , #0x1
	
	@write
	str	r1 , [r0]

	ldmfd sp! , {r0-r12 , pc}

.global buzzer_on
buzzer_on:

	stmfd sp! , {r0-r12 , lr}
	
	ldr	r0 , =GPD0DAT

	@read
	ldrb	r1 , [r0]

	@set
	orr	r1 , r1 , #0x1

	@write
	strb	r1 , [r0]

	ldmfd sp! , {r0-r12 , pc}

.global buzzer_off
buzzer_off:

	stmfd sp! , {r0-r12 , lr}
	
	ldr	r0 , =GPD0DAT

	@read
	ldrb	r1 , [r0]

	@clear
	bic	r1 , r1 , #0x1

	@write
	strb	r1 , [r0]

	ldmfd sp! , {r0-r12 , pc}

.global key_configure
key_configure:
	
	stmfd sp! , {r0-r12 , lr}
	
	ldr	r0 , =GPX3CON
	
	@read
	ldr	r1 , [r0]
	
	@set as input port
	ldr	r2 , =0xffff
	bic	r1 , r1 , r2 , lsl #8
	
	@write
	str	r1 , [r0]

	ldmfd sp! , {r0-r12 , pc}

@r0 为返回的键值  0 － 3
.global  get_key
get_key:

	stmfd sp! , {r4-r12 , lr}

	ldr	r0 , =GPX3DAT

	key_loop1:
	@read
	ldr	r1 , [r0]

	@shift right 2bits  and low 4 bits
	@shift
	lsr	r1 , r1 , #2
	@and
	and	r1 , r1 , #0xf
	@cmp
	cmp	r1 , #0xf

	beq	key_loop1

	@has check at least one key press down
	@check which key press down

	mov	r0 , #0
	mov	r2 , #1

	key_loop2:
	//r0  0 -> 3
	
	and	r3 , r1 , r2 , lsl r0	
	cmp	r3 , #0
	beq	  key_out

	add	r0 , #1
	cmp	r0 , #4

	bne	key_loop2

key_out:

	ldmfd sp! , {r4-r12 , pc}


