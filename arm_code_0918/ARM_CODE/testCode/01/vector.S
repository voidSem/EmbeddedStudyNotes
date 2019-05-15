
#define  CONFIG_PHY_UBOOT_BASE   0x43E00000
#define  second_boot_info   0xFFFFFFFF
.global  _start
_start:

    stmfd sp! , {r0-r12 , lr}

    /* store second boot information in u-boot C level variable */
    ldr r0, =CONFIG_PHY_UBOOT_BASE   @.    0x43E00000
    sub r0, r0, #8
    ldr r1, [r0]
    ldr r0, _second_boot_info
    str r1, [r0]


    ldmfd sp! , {r0-r12 , pc}

_second_boot_info:
    .word second_boot_info


