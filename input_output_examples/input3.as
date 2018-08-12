;file input3.as

.extern D
.entry A
.entry STRINGARR
INTARR: .data 1,2,3,4
STRINGARR: .string "Hello World!"
L1: prn #-1
	bne L1(r1,r2)
	jmp INTARR(#3,r3)
END: stop
.entry L1
A:	add B,C
C: .data 2
B: .data 3
sub #3,r1
dec C
	jmp L2(D,A)
inc r7
L2: cmp A,r1
clr A
clr B
rts
stop

