;file data_storage_without_label.as

L5: mov X,Y
L4: jsr W(r1,r2)
not r2
.entry X
.extern CP
.data 1
IND:.data 7
mov Z,r1
mov Z,r2
FUNC: add r1,r2
mov r1,r2
prn r2
dec IND
bne FUNC(r1,r2)
inc CP
END: stop
X:.data -10,-9,-8,-7,5000
.string "Hello Amazing C World"
F: lea IND, r2
prn Y
cmp r3,#-5
