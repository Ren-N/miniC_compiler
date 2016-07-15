; calculation library routine (multiply)
_mul:   mv ixr, sp
	ld #0
	push
	ld ixr, 2
	or #0
	jps _mull
	ld #0
	jp _mul2
_mul1:
	not
	inc acc
	st ixr, 2
	ld #1
_mul2:
	push
_mul_loop:
	ld ixr, 2
	asr
	st ixr, 2
	jpc _mul3
	jpz _mul4
	jp _mul5
_mul3:
	ld ixr, 1
	add ixr, -1
	st ixr, -1
_mul5:
	ld ixr, 1
	asl
	st ixr, 1
	jp _mul_loop
_mul4:
	pop
	or #0
	pop
	jpz _mul6
	not
	inc acc
_mul6:
	st ixr, 2
	ret