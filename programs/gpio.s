ldr r0,=0x20200004
ldr r1,=0x20200028
ldr r2,=0x2020001C

mov r3,#1
lsl r3,#18
str r3,[r0]

mov r4,#1
lsl r4,#16

str r4,[r1]

loop:

str r4,[r2]
mov r5,#0xFF0000
loop_on:
sub r5,r5,#1
cmp r5,#0
bne loop_on

str r4,[r1]
mov r5,#0xFF0000
loop_off:
sub r5,r5,#1
cmp r5,#0
bne loop_off

b loop