; infinite program
_start:
  ld 0

.loop:
  in 0 ; read from port 0
  out 1
  jmp .loop
