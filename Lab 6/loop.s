loop:
    sub   $a $a 0      # Set $ra = 0
    load  $b $a count  # set $rb = count
    load  $c $a limit  # set $rc = limit
    load  $d $a sum    # set $rd = sum
    add   $d $b 0      # $rd = $rb (count) + $rd (sum) + 0
    store $d $a sum    # set sum = $rd
    add   $b $a 1      # set $rb = $rb (count) + $ra (0) + 1
    store $b $a count  # set count = $rb
    bne   $b $c loop
    halt

limit: .word 11
count: .word 1
sum:   .word 0      # Should be 55 when done (0x37)
