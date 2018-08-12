;file multiple_first_pass_errors.as

.entry LENGT_H
.extern mov
MAIN:	gre	r3,LENGTH
LOOP:	stop L1(#-1,r6)
		prn r1,r2
		bne W(r4,r7)
		sub r1,r4
		bne L3
L1:		crop K
.entry LOOP
		bne LOOP(K,W)
		,3425sdfv	
END:	stop
STR:	.string "abcdef"
LENGTH: .data 6,-9,15
K:		.data 22, "string"
.extern L3