

.global	 _start 
_start:
	
	stmfd sp! , {r0-r12 , lr}

	nop

	ldr	r1 , =0xc3e11ad0

	ldr	r0 , =str
	mov	lr , pc
	mov	pc , r1


	ldmfd sp! , {r0-r12 , pc}

str:
	.string  "hello uboot... \n"
	.align 5
