            .text
start:      mv a0, zero
counter:    addi a0, a0, 5
      .word 0x51025 # lru.push(a0)
            mv a1, zero
      .word 0x59605 # a2 = lru.pop(a1)
            addi a0, a0, 5
      .word 0x51025 # lru.push(a0)
            addi a1, a1, 1
      .word 0x59605 # a2 = lru.pop(a1)
            beq zero, zero, counter


# push: hex(int('000000000000_01010_001_00000_0100101', 2))
# pop : hex(int('000000000000_01011_001_01100_0000101', 2))
