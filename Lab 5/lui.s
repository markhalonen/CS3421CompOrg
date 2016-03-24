    lui   $8,0x1234
    sw    $8, x
    halt
x:  .word  0    # should become 0x12340000	 
