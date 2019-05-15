
#define     GPM4CON    0x110002E0
#define     GPM4DAT	   0x110002E4

.global _start
_start:

	stmfd sp! , {r0-r12 , lr}

	bl	led_configure
#if 0
	@while  :
	loop:
		bl	led_all_on	
		bl	delay
		bl	led_all_off
		bl	delay
	b	loop
#endif 

	mov	r0 , #0

	loop:
	@ 0 - 3
	
	bl	led_which_on
	bl	delay
	bl	led_all_off


	add	r0 , #1
	cmp	r0 , #4
	moveq	r0 , #0

	b	loop

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

	ldr	 r0 , =0xfffffff

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

