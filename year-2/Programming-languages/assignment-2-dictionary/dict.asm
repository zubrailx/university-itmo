global find_word

extern string_equals

section .text

; Params:
;   rdi - pointer to null-terminated key string
;   rsi - pointer to the last word in dictionary
; Out:
;   rax - if not found ? 0 : list element ptr
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
    jnz  .get_element_ptr; 1 - equals, 0 - not
    mov rsi, [rsi - 8]
    jmp find_word

.get_element_ptr:
    sub rsi, 8
    mov rax, rsi
    ret

.not_found:
    mov rax, 0
    ret

