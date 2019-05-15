
.global _start
_start:

	stmfd sp! , {r0-r12 , lr}

	@configure led as output
	ldr	r0 , =0x110002E0
	
	ldr	r1 , [r0]
	ldr	r2 , =0xffff
	
	@清掉低16位
	bic	r1 , r1 , r2

	@设置成输出   0x1111
	ldr	r2 , =0x1111
	orr	r1 , r1 , r2

	str	r1 , [r0]

	@set as low lever   

	ldr	r0 , =0x110002E4
	ldrb	r1 , [r0]
	bic	r1 , r1 , #0xf
	strb	r1 , [r0]

	ldmfd sp! , {r0-r12 , pc}


