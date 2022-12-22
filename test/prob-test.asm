section .text
_start:

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
