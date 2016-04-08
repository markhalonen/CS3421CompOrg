add $a, $a, 0 #set a to 0
add $b, $b, 3 #set b to 10
loop:
	add $a, $c, 1
	beq $a, $b, done
	beq $d, $d, loop
done:
halt

#a should get set to 3
