
.global  start
start:

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
    stmfd sp! , {r4-r12 , lr}
    
    ldr r0 , [lr , #-4]
    ldr r1 , =0xffffff
    @ 拿到底24位立即数(系统调用号)
    and r0 , r0 , r1             

    ldr r1 , =0x62000008
    ldr r1 , [r1]
    mov lr , pc
    mov pc , r1

    ldmfd sp! , {r4-r12 , pc}^   @ ^  模式回复

PrefetchAbt:
    sub lr , lr , #4
    stmfd sp! , {r0-r12 , lr}

    ldr r0 , =0x6200000C
    ldr r1 , [r0]
    mov lr , pc
    mov pc , r1

    ldmfd sp! , {r0-r12 , pc}^   @ ^  模式回复


DataAbt:
    sub lr , lr , #4
    stmfd sp! , {r0-r12 , lr}

    ldr r0 , =0x62000010
    ldr r1 , [r0]
    mov lr , pc
    mov pc , r1

    ldmfd sp! , {r0-r12 , pc}^   @ ^  模式回复
Irq:
    sub lr , lr , #4
    stmfd sp! , {r0-r12 , lr}

    ldr r0 , =0x62000014
    ldr r1 , [r0]
    mov lr , pc
    mov pc , r1

    ldmfd sp! , {r0-r12 , pc}^   @ ^  模式回复
Fiq:
    sub lr , lr , #4
    stmfd sp! , {r0-r12 , lr}

    ldr r0 , =0x62000018
    ldr r1 , [r0]
    mov lr , pc
    mov pc , r1

    ldmfd sp! , {r0-r12 , pc}^   @ ^  模式回复

.global end
end:



