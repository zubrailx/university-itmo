section .rodata

colon "third_word", third_word
db "data_1", 0

colon "second_word", second_word
db "data_2", 0

colon "first_word", first_word
db "data_3dfjaskfjsajfklsajfjsakfjsakfj", 0


; third_value: dq 0
; @key_string: "key"
; db "data"

; second_word: dq third_word
; @key_string: "key2"
; db "data2" 

; first_word: dq second_word
; @key_string: "key3"
; db "data3"
