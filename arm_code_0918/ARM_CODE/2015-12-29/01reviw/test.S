
.global  _start
_start:

	stmfd sp! , {r4-r12 , lr}

	ldr	r0 , =str
	@By Default u-boot Open MMU
	ldr	r1 , =0xC3e11ad0   @ 40000000 - 7fffffff

	@保护现场
	mov	lr , pc
	mov	pc , r1

	mov	r0 , #33
	mov	r1 , #44

	bl	Add_Func

	ldr	r0 , =pstr
	mov r1 , r2
	ldr	r2 , =0xC3e11ad0   
	
	mov	lr , pc
	mov	pc , r2


	ldmfd sp! , {r4-r12 , lr}
	@ return 0 ; 
	mov	r0 , #0
	mov	pc , lr



str:
	.string  "hello world \n"
	.align 5

pstr:
	.string "add : %d\n"
	.align 5
	

.global  Add_Func
Add_Func:
	stmfd sp! , {r4-r12 , lr}

	add	r2 , r0 , r1

	ldmfd sp! , {r4-r12 , pc}

