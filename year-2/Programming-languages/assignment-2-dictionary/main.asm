%include "colon.inc"
%include "words.inc"

global _start

extern print_string
extern read_word
extern exit
extern find_word
extern print_newline

%define STD_OUT 0x01
%define STD_ERR 0x02

section .rodata
mes_enter: db "enter the key: ", 0
mes_key_not_found_str: db "ERROR: such key is not in dictionary", 10, 0
mes_string_too_long: db "ERROR: input string is too long", 10, 0
mes_result: db "value: ", 0

section .text
_start:
    sub rsp, 256

    mov rdi, mes_enter
    mov rsi, STD_OUT
    call print_string
    mov rdi, rsp
    mov rsi, 256
    call read_word
    cmp rax, 0
    je .print_error_length

    mov rdi, rax
    mov rsi, LAST_LABEL ; LAST_LABEL - directive in colon.inc
    call find_word
    test rax, rax
    jz .print_not_found
.print_key:
    mov rdi, rax
    mov rsi, STD_OUT
    call print_string
    call print_newline
    add rsp, 256
    call exit

.print_error_length:
    mov rdi, mes_string_too_long
    mov rsi, STD_ERR
    call print_string
    add rsp, 256
    call exit

.print_not_found:
    mov rdi, mes_key_not_found_str
    mov rsi, STD_ERR
    call print_string
    add rsp, 256
    call exit
