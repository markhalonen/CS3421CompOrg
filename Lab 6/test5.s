add $a, $a, -10

loop:
	sub $b, $c, 1
	beq $b, $a, done
	beq $a, $a, loop
done:
halt
