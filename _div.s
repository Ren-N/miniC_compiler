; calculation library (div/mod)
_div:
	mv ixr, sp
	ld ixr, 2
	or #0
	jps _div1
	ld #0
	jp _div2
_div1:
	not
	inc acc
	st ixr, 2
	ld #3
_div2:
	push
	ld ixr,1
	or #0
	jps _div3
	jpz _div_error
	jp _div4
_div3:
	not
	inc acc
	st ixr, 1
	ld ixr, -1
	eor #1
	st ixr, -1
_div4:
	ld #1
	push
	ld #1
	push
_div_pre:
	ld ixr, 1
	lsl
	jps _div_loop
	st ixr 1
	ld ixr, -3
	lsl
	st ixr, -3
	jp _div_pre
_div_loop:
	ld ixr, 2
	sub ixr, 1
	jps _div5
	st ixr, 2
	ld ixr, -2
	or #1
	st ixr, -2
_div5:
	ld ixr, -3
	lsr
	jpc _div6
	st ixr, -3
	ld ixr, 1
	lsr
	st ixr, 1
	ld ixr, -2
	lsl
	st ixr, -2
	jp _div_loop
_div6:
	ld ixr, -1
	and #0xFE
	ld ixr, -2
	jpz _div7
	not
	inc acc
_div7:
	st ixr, 1
	ld ixr, -1
	and #0xFD
	jpz _div8
	ld ixr, 2
	not
	inc acc
	st ixr, 2
_div8:
	mv sp, ixr
	ret
_div_error:
	halt