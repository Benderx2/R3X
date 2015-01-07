; This example showcases how to use the stack in r3x VM.
; Since the stack is not modifiable directly by the program, 
; it is supposed to use a set of instructions to change it.
; This example will showcase the following instructions:
; push
; pushr
; pop
; popr 
; popn 
; loads
; stores
; loadsr
; storesr
; You should expect the following output
; 35 65 35 12345 5 49 12 100
include 'libR3X/libR3X.pkg'
.text {
	function main
	; The push instruction pushes an integer (which could be anything pointer, number, pointer to structure..)
	push 35
	; Let's print what we just pushed!
	syscall SYSCALL_PUTI
	; Let's load a register with immediate
	loadr R8, 65
	; Let's push the value of the register
	pushr R8 ; 65 will be pushed to stack!
	; Let's print it.
	syscall SYSCALL_PUTI
	; Let's clean up the stack, that is remove the stuff we just pushed
	pop ; Removes 65 from stack
	; Since 65 is removed, what remains is 35 on stack, let's pop it into R8
	popr R8
	; Let's push R8 and see what value it contains
	pushr R8
	syscall SYSCALL_PUTI
	pop ; Pop out and destroy value
	; Let push 6 things to stack
	push 12345
	push 1
	push 2
	push 3
	push 4 
	push 5
	; Let's pop off 5 of em
	popn 5
	; The number left on stack would be 12345
	; Let's print it!
	syscall SYSCALL_PUTI
	; Pop it off
	pop
	; Let's load something from stack offset
	push 5
	push 7
	push 9
	; What if we wanted to push the value we pushed 3rd last?
	loads 3 ; Pushes 5, remember we pushed 5 3rd last?
	syscall SYSCALL_PUTI
	popn 4 ; Pop off 4 things pushed to stack
	; Let's store something to stack
	push 7
	push 3
	push 12
	; What if we wanted to change the '7' to '49' we pushed to stack?
	push 49
	stores 4 ; Remember, position of 7 changed when we pushed 49 to stack by 1.
	popn 3 ; Pop off the last 3 elements of stack, which makes 49 on top
	; Let's print it!
	syscall SYSCALL_PUTI
	; Let's use storesr and loadsr, they are same as loads/stores but they use registers instead of immediates
	push 15
	push 12
	; Change 15 to 12
	loadr R15, 2 ; Load 15's stack offset
	storesr R15
	pop 
	; Now 12 is on top of stack since we changed 15 to 12
	; Print it!
	syscall SYSCALL_PUTI
	pop
	push 100
	push 150
	push 200
	; Let's load 100 from stack, through loadsr.
	loadr R15, 3 ; Load 100's stack offset
	loadsr R15
	; Now 100 is pushed to stack
	syscall SYSCALL_PUTI
	popn 4 ; Clean up stack
	; Let's wait for user input
	Console.WaitKey
	; And exit!
	System.Quit 0
	endfunction main
}
; REMEMBER TO PUT THIS AT THE END OF EVERY MAIN MODULE!
end 
