section .data

fmax: 4000000 ; max value of fibonacci

sum: 0 
sum_inv: 0 ; inverted sum (for output)
dig: 1 ; count of digits in output

ftmp: 0
fp: 2 ; previous
fpp: 1 ; previous of previous
cur: 2 ; current fibonacci

section .text
_start:
  ld [fp]
  cmp [fmax]
  jg print_res
  ; initiate sum with second fib
  st [sum]
.loop:
  st [ftmp]
  add [fpp]
  cmp [fmax]
  jg invert_sum ; if new value is larger than 
  ; recalc current fibonacci
  st [fp]
  ld [ftmp]
  st [fpp]
  ; increment current
  ld [cur]
  inc
  st [cur]
  mod 2
  jne .add_end
.add:
  ld [fp]
  add [sum]
  st [sum]
.add_end:
  ld [fp] ; var need to loop
  jmp .loop
  
invert_sum:
.loop4: ; invert sum numbers
  ld [sum]
  mod 10
  add [sum_inv]
  st [sum_inv]
; divide sum by 10
  ld [sum]
  div 10
  je print_res
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
  
print_res: 
  ld [sum_inv]
.loop3:
  mod 10
  itoc
  out 1
; decrease cnt of digits
  ld [dig]
  dec
  je halt
  st [dig]
; load next inverted char
  ld [sum_inv]
  div 10
  st [sum_inv]
  jmp .loop3

halt:
  halt
