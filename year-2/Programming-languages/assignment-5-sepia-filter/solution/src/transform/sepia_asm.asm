%define input_ptr r10
%define output_ptr r11
%define pixel_left r9


global  sepia_pixels_asm


section .text

align 16
c1_0: dd 0.272, 0.349, 0.393, 0.000
align 16
c2_0: dd 0.543, 0.686, 0.769, 0.000
align 16
c3_0: dd 0.131,0.168, 0.189, 0.000

align 16
max: dd 0xFF, 0xFF, 0xFF, 0xFF

; rdi - input pixel pointer
; rsi - output pixel pointer
; rdx - pixels count
sepia_pixels_asm:
    ; %1 - pattern for shufps
    ; %2 - address of first pixel
    ; %3 - address of next pixel
    %macro shuffle_and_convert 1
        shufps xmm0, xmm0, %1  
        cvtdq2ps xmm0, xmm0             ; put pixel.r in xmm and convert to float
        shufps xmm1, xmm1, %1  
        cvtdq2ps xmm1, xmm1             ; put pixel.r in xmm and convert to float
        shufps xmm2, xmm2, %1  
        cvtdq2ps xmm2, xmm2             ; put pixel.r in xmm and convert to float
    %endmacro

    ; %1 - pattern for shufps to set matrix_c
    %macro xmm_set_matrix_coef 1
        movaps xmm3, [rel c1_0]
        shufps xmm3, xmm3, %1           ; put c1_0 values in xmm3

        movaps xmm4, [rel c2_0]         
        shufps xmm4, xmm4, %1           ; put c2_0 values in xmm4

        movaps xmm5, [rel c3_0]
        shufps xmm5, xmm5, %1           ; put c3_0 values in xmm5
    %endmacro

    ;----------------------
    mov input_ptr, rdi
    mov output_ptr, rsi
    mov pixel_left, rdx


    test pixel_left, pixel_left
    jz .end
.instruction_loop:
    ; 1
    lea r8, [input_ptr + 3]                 ; next pixel start (for xmm_set_from_input)
    mov rdi, output_ptr                     ; arguments for calculate_and_set
    lea rsi, [output_ptr + 1]
    lea rdx, [output_ptr + 2]
    lea rcx, [output_ptr + 3]
    
    dec pixel_left
    test pixel_left, pixel_left
    jnz .more_one_pixel

.one_pixel:
    mov r8, input_ptr                       ; no next pixel (for xmm_set_from_input)

    mov rcx, output_ptr                     ; change argument for calculate_and_set

.more_one_pixel:
    push rdi
    push rsi        
    mov rdi, input_ptr
    mov rsi, r8
    call xmm_set_from_input 
    pop rsi
    pop rdi

    shuffle_and_convert 11000000b

    xmm_set_matrix_coef 00100100b

    call calculate_and_set

    ; 2
    add input_ptr,  3
    add output_ptr, 4                       ; cuz written 4 pixels
    test pixel_left, pixel_left
    jz .end

    lea r8, [input_ptr + 3]                 ; next pixel start (for xmm_set_from_input)
    mov rdi, output_ptr                     ; arguments for calculate_and_set
    lea rsi, [output_ptr + 1]
    lea rdx, [output_ptr + 2]
    lea rcx, [output_ptr + 3]

    dec pixel_left
    test pixel_left, pixel_left
    jnz .more_two_pixels

.two_pixels:
    mov r8, input_ptr                       ; no next pixel (for xmm_set_from_input)

    lea rdx, [output_ptr - 1]
    mov rcx, output_ptr                     ; change argument for calculate_and_set

.more_two_pixels:
    push rdi
    push rsi
    mov rdi, input_ptr
    mov rsi, r8
    call xmm_set_from_input
    pop rsi
    pop rdi

    shuffle_and_convert 11110000b

    xmm_set_matrix_coef 01001001b

    call calculate_and_set

    ; 3
    add input_ptr,  3
    add output_ptr, 4
    test pixel_left, pixel_left
    jz .end

    lea r8, [input_ptr + 3]                 ; next pixel (for xmm_set_from_input)
    mov rdi, output_ptr                     ; arguments for calculate_and_set
    lea rsi, [output_ptr + 1]
    lea rdx, [output_ptr + 2]
    lea rcx, [output_ptr + 3]

    dec pixel_left
    test pixel_left, pixel_left
    jnz .more_three_pixels

.three_pixels:
    mov r8, input_ptr                       ; no next pixel (for xmm_set_from_input)

    lea rsi, [output_ptr - 2]
    lea rdx, [output_ptr - 1]
    mov rcx, output_ptr                     ; change argument for calculate_and_set

.more_three_pixels:
    push rdi
    push rsi
    mov rdi, input_ptr
    mov rsi, r8
    call xmm_set_from_input
    pop rsi
    pop rdi

    shuffle_and_convert 11111100b

    xmm_set_matrix_coef 10010010b

    call calculate_and_set 


    add input_ptr, 6                        ; set ptrs on next data
    add output_ptr, 4                       
    dec pixel_left
    test pixel_left, pixel_left
    jnz .instruction_loop
.end:
    ret


; rdi - first pixel 
; rsi - next pixel
xmm_set_from_input:
    pxor xmm0, xmm0
    pxor xmm1, xmm1
    pxor xmm2, xmm2                 ; clear xmm registers

    pinsrb xmm0, byte[rdi + 0], 0   ; insert bytes from rdi (input)
    pinsrb xmm0, byte[rsi + 0], 12

    pinsrb xmm1, byte[rdi + 1], 0
    pinsrb xmm1, byte[rsi + 1], 12

    pinsrb xmm2, byte[rdi + 2], 0
    pinsrb xmm2, byte[rsi + 2], 12

    ret
    

; rdi - first pixel
; rsi - second pixel
; rdx - third pixel
; rcx - fourth pixel
calculate_and_set:
    mulps xmm0, xmm3
    mulps xmm1, xmm4
    mulps xmm2, xmm5                ; multiply
    addps xmm0, xmm1
    addps xmm0, xmm2                ; store sum in xmm0
    cvtps2dq xmm0, xmm0             ; convert to int32_t
    movaps xmm7, [rel max]
    pminsd xmm0, xmm7               ; put in xmm0 min(0xFF, xmm0) in int32_t 

    pextrb byte[rdi], xmm0, 0   ; extract bytes in rsi (output)
    pextrb byte[rsi], xmm0, 4
    pextrb byte[rdx], xmm0, 8
    pextrb byte[rcx], xmm0, 12 

    ret
