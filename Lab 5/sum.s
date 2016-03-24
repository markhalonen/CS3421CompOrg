# store 1+2+...+10 in sum
 	lw      $1,n
 	lw      $10,max
loop:
 	beq     $2, $10, done
 	add     $2, $2, $1
 	add     $3, $3, $2
 	j	loop
done:
	sw	$3,sum
# test and and or instructions
	lw      $10,three
	lw	$11,five
	and     $12,$10,$11
	sw	$12,and
	or      $12,$10,$11
	sw	$12,or
 	halt
n:	.word	1
max:	.word	10
sum:	.word	0	# should become 55
three:	.word	3
five:	.word	5
and:	.word	0	# should become 1
or:	.word	0	# should become 7
