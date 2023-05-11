            .text

start:      mv a0, zero              # t0 = 0
counter:    addi a0, a0, 1           # t0 = t0 + 1
            # push: [0000.0000.0000:any][00000:xN][001][00000:any][0100101]
push:      .word 0x00051025
            # pop : [0000.0000.0000:any][00000:xN][001][00000:xNo][0000101]
pop:       .word 0x00051505
            beq zero, zero, counter  # if t0 == t1 then counter
