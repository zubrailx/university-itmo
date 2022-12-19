global _start

section .text

; Params:
;   -
; Out:
;   !exit
exit:
  mov rax, 60
  mov rdi, 0
  syscall
  ret


; Params:
;   rdi - file descriptor
;   rsi - code of char
; Out:
;   >1 - char code in std
print_char:
    mov rax, 1      ; value in rdi
    mov rdx, 1      ; length
    push rsi        ; push symbol on stack      
    mov rsi, rsp    ; the address of char in stack
    syscall
    pop rsi
    ret


; Params:
; rdi - file descriptor
; Out:
;   >1 - \n (0xA)
print_newline:
    mov rsi, 10
    call print_char
    ret 


; Params:
;   rdi - string ptr
; Out:
;   rax - string length
string_length:
    xor rax, rax
.loop:
    inc rax
    cmp byte [rdi + rax - 1], 0 ;starts with 1, ends with 1+length-1
    jne .loop
    dec rax
    ret


; Params:
;   rdi - descriptor for writing
;   rsi - pointer on null-terminated string
; Out:
;   >1 - string in std
print_string:
    push rdi
    mov rdi, rsi
    call string_length
    pop rdi
    mov rdx, rax    ; string length
    mov rax, 1      ; call number
    syscall 
    ret


; Params:
;   >0 - one char
; Out:
;   rax - EOF ? 0 : char_symbol
read_char:
  push 0
  mov rsi, rsp
  mov rax, 0
  mov rdi, 0
  mov rdx, 1
  syscall
  pop rax
  ret


; Params:
;   rdi - buffer address
;   rsi - buffer size
;   >0 - symbols in ASCII
; Out:
;   rax - buffer address, failure  - 0
;   rdx - size of word
; Description:
;   Reads word of std-in.
;   1) Skips space chars from the beginning of input (0x20, 0x9, 0xA)
;   2) Adds null-terminator to string
read_word:
    push rdi        ; address of buffer start
    push r12
    mov r12, rdi    ; callee saved registers
    push r13
    mov r13, rsi
.loop_space:
    call read_char
    cmp rax, 0x20
    je .loop_space
    cmp rax, 0x09
    je .loop_space
    cmp rax, 0xA
    je .loop_space
.loop:
    cmp rax, 0x0
    je .finally
    cmp rax, 0x20
    je .finally
    cmp rax, 0x9
    je .finally
    cmp rax, 0xA
    je .finally
    dec r13        ; will be if size=n n+1 cuz of null-terminator
    cmp r13, 0
    jbe .overflow
    mov byte [r12], al
    inc r12
    call read_char
    jmp .loop
    
.finally:
    mov byte [r12], 0   ; putting null terminator
    pop r13             ; pop r13
    pop r12             ; pop r12
    mov rdi, [rsp]      ; get rdi from rsp to add func args
    call string_length  
    mov rdx, rax        ; put size in rdx
    pop rax             ; put rdi in rax
    ret

.overflow:
    pop r13             ; get back everything from stack
    pop r12
    pop rdi
    mov rax, 0
    ret


; Params:
;   rdi - file descriptor
;   rsi - unsigned int 8 bytes
; Out:
;   >1 - unsigned int in decimal
print_uint:
    push 0          ; null terminated string
    mov rbx, 10     ; devident
    mov rax, rsi    ; put in rax for calculation
    add rsp, 7
.count:
    xor rdx, rdx
    div rbx         ; rax div rcx -% rax
    add dl, '0'     ; one lowest byte
    mov dh, byte [rsp]
    add rsp, 1
    push dx
    test rax, rax
    jne .count
    mov rsi, rsp
    call print_string
    mov rdi, rsp
    call string_length
    add rsp, rax
    inc rsp         ; jump over null-terminator
    ret


; Params:
;   rdi - string on ptr (unsigned integer)
; Out:
;   rax - unsigned integer
;   rdx - failure ? 0 : integer_length_in_decimal
parse_uint:
    mov rsi, 0x0A       ; base
    xor r8, r8          ; result
    xor rax, rax
    xor rcx, rcx        ; length counter
.loop:
    mov al, byte[rdi + rcx]
    cmp al, '0'
    jb .finally
    cmp al, '9'
    ja .finally
    sub al, '0'
    push rax
    mov rax, r8
    mul rsi
    mov r8, rax
    pop rax

    add r8, rax
    inc rcx
    jmp .loop

.finally:
    mov rdx, rcx
    mov rax, r8  
    ret


section .data

f_sum: dd 0

f_i: dd 1
f_in: dd 2


section .text

_start:
  sub rsp, 256
  mov rdi, rsp
  mov rsi, 256
  call read_word

  mov rdi, rsp
  call parse_uint

  mov rdi, 1
  mov rsi, rax
  call print_uint
  mov rdi, 1
  call print_newline

  add rsp, 256
  call exit 
  
