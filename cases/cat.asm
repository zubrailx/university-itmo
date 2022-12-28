section .text

_start:
  ld 0

.loop:
  in 0 ; read from port 0
  cmp 0
  je end
  out 1
  jmp .loop

end:
  halt
