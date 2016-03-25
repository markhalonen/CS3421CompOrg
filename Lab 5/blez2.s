    lw   $7,x
    lw   $8,y1    
    blez $7,end
    lw   $8,y2
end:
    sw   $8,y3
    lw	 $9, y4
    blez $9, continue
    halt
continue:
    sw $7,y4
    halt
x:  .word  -5	 
y1: .word  5
y2: .word  3	
y3: .word  0	# should become 0x5
y4: .word 0	#should become 0x-5
