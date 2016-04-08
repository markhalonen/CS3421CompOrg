add $a, $a, 5 #set a to zero
beq $a, $a, here
notHere:
	add $a, $a, 1
	halt
here:
	add $a, $a, 2
	halt
halt
