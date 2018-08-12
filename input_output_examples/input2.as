;file input2.as

add x, r4
y: .data 7
L1: mov y, r3
x: .data 10, -3
mov z,y
jmp L1(#-5,y)
.entry y
mov #3,z
.extern z