_out: mv ixr, sp
      ld ixr, 1
      out 0x00
      ret
_in:  mv ixr, sp
      in 0x00
      st ixr, 1
      ret
