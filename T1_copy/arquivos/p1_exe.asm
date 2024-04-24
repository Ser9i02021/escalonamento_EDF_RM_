.data
.text
.globl main

main:
    li $a0, 10       # Load the first argument into $a0
    li $a1, 20       # Load the second argument into $a1
    jal sum          # Call the sum function
    move $s0, $v0    # Move the result to $s0 for further use

    li $v0, 10       # Load syscall for exit
    syscall          # Exit the program

sum:
    # Save registers that will be used
    addi $sp, $sp, -8  # Allocate space on the stack for two registers
    sw $ra, 4($sp)     # Save the return address
    sw $a0, 0($sp)     # Save the value of $a0 (optional here for educational purposes)

    # Perform the addition
    add $v0, $a0, $a1  # Add the values in $a0 and $a1, result in $v0

    # Restore the saved registers
    lw $a0, 0($sp)     # Restore $a0
    lw $ra, 4($sp)     # Restore the return address
    addi $sp, $sp, 8   # Deallocate stack space

    jr $ra             # Return to caller

