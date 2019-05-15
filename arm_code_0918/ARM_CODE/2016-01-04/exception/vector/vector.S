
.global  _start
_start:

    b   Reset
    b   Undef
    b   Svc
    b   PrefetchAbt
    b   DataAbt
    nop
    b   Irq
    b   Fiq

Reset:
    stmfd sp! , {r0-r12 , lr}

    ldr r0 , =0x62000000  
    ldr r1 , [r0]
    mov lr , pc
    mov pc , r1

    ldmfd sp! , {r0-r12 , pc}^   @ ^  模式回复
Undef:
    stmfd sp! , {r0-r12 , lr}

    ldr r0 , =0x62000004
    ldr r1 , [r0]
    mov lr , pc
    mov pc , r1

    ldmfd sp! , {r0-r12 , pc}^   @ ^  模式回复
Svc:
    stmfd sp! , {r0-r12 , lr}

    ldr r0 , =0x62000008
    ldr r1 , [r0]
    mov lr , pc
    mov pc , r1

    ldmfd sp! , {r0-r12 , pc}^   @ ^  模式回复
PrefetchAbt:
    stmfd sp! , {r0-r12 , lr}

    ldr r0 , =0x6200000C
    ldr r1 , [r0]
    mov lr , pc
    mov pc , r1

    ldmfd sp! , {r0-r12 , pc}^   @ ^  模式回复
DataAbt:
    stmfd sp! , {r0-r12 , lr}

    ldr r0 , =0x62000010
    ldr r1 , [r0]
    mov lr , pc
    mov pc , r1

    ldmfd sp! , {r0-r12 , pc}^   @ ^  模式回复
Irq:
    stmfd sp! , {r0-r12 , lr}

    ldr r0 , =0x62000014
    ldr r1 , [r0]
    mov lr , pc
    mov pc , r1

    ldmfd sp! , {r0-r12 , pc}^   @ ^  模式回复
Fiq:
    stmfd sp! , {r0-r12 , lr}

    ldr r0 , =0x62000018
    ldr r1 , [r0]
    mov lr , pc
    mov pc , r1

    ldmfd sp! , {r0-r12 , pc}^   @ ^  模式回复


