section .data
; program variables
cur: 2 ; current number
nth: 0 ; nth number - 1 we need to find 

input: 0

sum: 0 ; 
sum_inv: 0 ; because we will receive result like 45 instead of 54
dig: 1 ; cnt of inverted sum digits

tmp: 0

ftmp: 0
fp: 2 ; previous value of fibonacci
fpp: 1 ; previous of previous value

section .text
_start:
.input:
  ld 0
  in 0 ; read fibonacci number from port 0
  cmp '\n' ; cmp with line separator
  je input_end

  ctoi ; covert value to character
  st [tmp]
  ld [input]
  mul 10
  add [tmp]
  st [input]
  jmp .input


input_end:
  ld [input]
  cmp 0
; user can't input negative value
  je out_sum_inv
  cmp 1
  je out_sum_inv

  ; else value is at least 2
calc:
  inc
  st [nth]      ; increment fth value
  ld [cur]

.loop2:
  cmp [nth]
  je invert_sum
  mod 2
  jne .not_add

.add:
  ld [sum]
  add [fp]
  st [sum]

.not_add:
  ld [fp]
  add [fpp]
  st [ftmp]
  ld [fp]
  st [fpp]
  ld [ftmp]
  st [fp]
  ld [cur]
  inc
  st [cur]
  jmp .loop2

invert_sum:
.loop4: ; invert sum numbers
  ld [sum]
  mod 10
  add [sum_inv]
  st [sum_inv]
; divide sum by 10
  ld [sum]
  div 10
  je out_sum_inv
  st [sum]
; multiply sum_inv by 10
  ld [sum_inv]
  mul 10
  st [sum_inv]
; increment cnt of digits
  ld [dig]
  inc
  st [dig]
  jmp .loop4
  
out_sum_inv: 
  ld [sum_inv]
.loop3:
  mod 10
  itoc
  out 1
; decrease cnt of digits
  ld [dig]
  dec
  st [dig]
  je halt
; load next inverted char
  ld [sum_inv]
  div 10
  st [sum_inv]
  jmp .loop3


halt:
  halt
