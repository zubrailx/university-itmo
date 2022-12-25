section .text
_start:
  ld 0
  in 0 ; read fibonacci number from port 0
  cmp '\n' ; cmp with line separator
  je input_end

  st tmp
  ld [input]
  mul 10
  add tmp
  st input
  jmp _start


input_end:
  ld [input]
  cmp 0
  js out_sum
  cmp 1
  je out_sum

  ; else value is at least 2
calc:
  inc
  st nth      ; increment fth value
  ld [cur]

.loop2:
  cmp [nth]
  je out_sum
  mod 2
  jne .not_add

.add:
  ld [sum]
  add [fp]
  st sum

.not_add:
  ld [fp]
  add [fpp]
  st fc
  ld [fp]
  st fpp
  ld [fc]
  st fp
  ld [cur]
  inc
  jmp .loop2

  
out_sum:
  ld [sum]
.loop3:
  mod 10
  itoc
  out 1
  ld [sum]
  div 10
  js halt
  st sum
  jmp .loop3


halt:
  halt
  

section .data
; program variables
cur: 2
nth: 0

sum: 0
input: 0
tmp: 0

fc: 0
fp: 1 ; previous value of fibonacci
fpp: 2 ; previous of previous value
