    lwrb $7,x
    sw   $7,y
    halt
x:  .word  0x12345678
y:  .word  0			# should become 0x78563412
