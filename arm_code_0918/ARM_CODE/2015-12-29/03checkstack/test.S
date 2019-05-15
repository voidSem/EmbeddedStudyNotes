
.global _start
_start:

	stmfd sp! , {r4-r12 , lr}

	mov	 r5 , sp

	ldr	sp , =0x50000000

	ldr	r0 , =0x12345678
	ldr	r1 , =0x87654321

	stmfd sp! , {r0,r1}

	ldr	r0 , =str
	ldr	r4 , =0xC3E11ad0
	ldr	r6 , =0x50000000

	ldr	r1 , [r6]
	ldr	r2 , [r6 , #-4]
	ldr	r3 , [r6 , #-8]

	mov	lr , pc
	mov	pc , r4

	mov	sp , r5

	ldmfd sp! , {r4-r12 , pc}

str:
	.string "1 : %p 2 : %p 3 : %p \n"
	.align 5

