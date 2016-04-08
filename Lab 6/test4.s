add $a, $a, 10

loop:
	add $b, $c, 1
	beq $b, $a, loop2
	beq $a, $a, loop
done:
halt
loop2:
add $c, $c, 3
beq $a, $a, done
