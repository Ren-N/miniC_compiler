.text
CRT0:ld16 #0xFFE
      mv sp, acc
      call F0003
      halt
.data
_FP:.space 1
.text
F0002:ld _FP ;funcName:fib
      push
      mv acc, sp
      st _FP
      sub #0
      mv sp, acc
      ld _FP ;  k ; funcParam
      mv ixr, acc
      ld ixr, 2
      push
      ld16 #2 ; Const
      push
      mv ixr, sp ; BLT
      ld ixr, 1
      sub ixr, 0
      ld #1
      jps L0001
      ld #0
L0001:st ixr, 1
      inc sp
      pop ; IF
      or #0
      jpz IFELSE0000
      ld16 #1 ; Const
      push
      jp E0002
IFELSE0000:
IFEND0000:
      dec sp ; FunctionCall
      ld _FP ;  k ; funcParam
      mv ixr, acc
      ld ixr, 2
      push
      ld16 #2 ; Const
      push
      mv ixr, sp ; Sub
      ld ixr, 1
      sub ixr, 0
      st ixr, 1
      inc sp
      call F0002
inc sp
      dec sp ; FunctionCall
      ld _FP ;  k ; funcParam
      mv ixr, acc
      ld ixr, 2
      push
      ld16 #1 ; Const
      push
      mv ixr, sp ; Sub
      ld ixr, 1
      sub ixr, 0
      st ixr, 1
      inc sp
      call F0002
inc sp
      mv ixr, sp ; Add
      ld ixr, 1
      add ixr, 0
      st ixr, 1
      inc sp
      jp E0002
      push
E0002:ld _FP
      mv ixr, acc
      pop
      st ixr, 3
      mv sp, ixr
      pop
      st _FP
      ret
F0003:ld _FP ;funcName:main
      push
      mv acc, sp
      st _FP
      sub #2
      mv sp, acc
      dec sp ; FunctionCall
      call _in
      ld _FP ; localVar=Assignment
      mv ixr, acc
      pop
      st ixr, -1
      push
      inc sp
      dec sp ; FunctionCall
      ld _FP ;  v ; localVar
      mv ixr, acc
      ld ixr, -1
      push
      call F0002
inc sp
      ld _FP ; localVar=Assignment
      mv ixr, acc
      pop
      st ixr, -2
      push
      inc sp
      dec sp ; FunctionCall
      ld _FP ;  w ; localVar
      mv ixr, acc
      ld ixr, -2
      push
      call _out
inc sp
      inc sp
      ld16 #0 ; Const
      push
      jp E0003
      push
E0003:ld _FP
      mv ixr, acc
      pop
      st ixr, 2
      mv sp, ixr
      pop
      st _FP
      ret
_out: mv ixr, sp
      ld ixr, 1
      out 0x00
      ret
_in:  mv ixr, sp
      in 0x00
      st ixr, 1
      ret

.const 0xfff
.word CRT0
