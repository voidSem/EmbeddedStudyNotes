
.global	main
main:
	
	stmfd sp! , {r0-r12 , lr}


	ldr	r0 , =str  @ r0 为格式字符 第一个参数
	mov	r1 , #33
	mov	r2 , #34
	mov	r3 , #36
	@APCS   之后的参数必须压栈

	mov	r4 , #44
	mov	r5 , #45
	@先压后面的参数  从后往前压栈
	push {r5}
	push {r4}

	bl	printf

	pop  {r4}
	pop  {r5}


	@ldmfd sp! , {r0-r12 , lr}
	@mov	pc , lr
	ldmfd sp! , {r0-r12 , pc}


str:
	.string "hello world %d %d %d next : %d %d \n"
	.align	5
