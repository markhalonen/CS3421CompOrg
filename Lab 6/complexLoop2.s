add $a, $a, 0
add $d, $d, 7
loop:
	beq $a, $d, done
	add $c, $c, 1
	add $a, $a, 1
	beq $d, $d, loop
done:
halt
