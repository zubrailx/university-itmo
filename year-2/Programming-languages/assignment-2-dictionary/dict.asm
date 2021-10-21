global find_word

extern string_equals
extern string_length

section .text

; Params:
;   rdi - pointer to null-terminated key string
;   rsi - pointer to the last word in dictionary
; Out:
;   rax - if not found ? 0 : record address
find_word:
    cmp rsi, 0
    je .not_found
    add rsi, 8
    push rdi
    push rsi
    call string_equals
    pop rsi
    pop rdi
    test rax, rax
    jnz .get_data_ptr  ; 1 - equals, 0 - not
    sub rsi, 8
    mov rsi, [rsi]
    jmp find_word

.not_found:
    mov rax, 0
    ret

.get_data_ptr:
    call string_length
    inc rax
    add rsi, rax
    mov rax, rsi    ; address of data
    ret
