;;; Zain Bhaila
;;; 115197541
;;; zainb

;;; Transform an array by adding the value at an index to the sum of the
;;; remainder of the array. Return the sum.
;;; Returns a uint16_t, takes pointer v to int8_t array.

	.global reverse_prefix_sum ; linker
	
reverse_prefix_sum:
	push r18		; save int value
	push r19		; save int value
	push r26		; save pointer value
	push r27		; save pointer value
	
	mov r26, r24		; load pointer X with input
	mov r27, r25		; load X with input

	ld r18, X		; get value of input pointer
	ld 19, X+		; get value of input wiht post inc

	cpi r18, -1		; check if input is -1
	breq end		; array has no more values
	
	mov r24, r26 		; set next value for recursive call
	mov r25, r27		; set for recursive
	call reverse_prefix_sum	; get value of next in array

	clc		        ; clear carry flag
	add r24, r18		; add rps(pointer+1) + value of pointer
	st -X, r24		; save integer value
	brcs add_carry		; if overflow

	jmp return		; end program
	
end:				; if the end of array is reached
	clr r24			; set return value to 0
	clr r25			; set return value to 0
	
return:				; exit program
	pop r27			; restore pointer value
	pop r26			; restore pointer value
	pop r19			; restore int value
	pop r18			; restore int value
	
	ret			; return

add_carry:
	inc r25	 		; add carry bit
	jmp return		; end program

	
;;; Print out array.
;;; No return value. Takes pointer v to int8_t array.

	.global print_array	; linker
	
print_array:			; begin print array
	mov r26, r24		; load pointer X with input
	mov r27, r25		; load X with input

	ld r18, X		; get value of input pointer
	ld r19, X+		; get value of input with post inc

	cpi r18, -1		; check if input is -1
	breq end_array		; array has no more values

	mov r24, r18		; get ready to print value
	clr r25			; clear high byte
	call pint		; print value

loop:				; begin array loop
	ld r18, X		; get value of pointer
	ld r19, X+		; get value of pointer with post inc

	cpi r18, -1		; check if input is -1
	breq end_array		; array has no more values

	mov r24, r18		; get ready to print value
	clr r25			; clear high byte
	call pint2		; print value

	jmp loop		; continue while input not -1

end_array:			; end of array
	call prt_newline	; print new line
	ret			; return
	

;;; Prints out an integer
	
	.data			; data
	
pctd:				; format string 1
        .asciz "%d"          	; defines a string (nul terminated)
	
pctd2:				; format string 2
        .asciz " %d"          	; defines a string (nul terminated)

        .text			; text

pint:	 			; print int value without space
        ldi r22, lo8(pctd)    	; lower byte of the string address
        ldi r23, hi8(pctd)    	; higher byte of the string address
	jmp print_int		; print out int

pint2:	 			; print int value with space
        ldi r22, lo8(pctd2)    	; lower byte of the string address
        ldi r23, hi8(pctd2)    	; higher byte of the string address
	jmp print_int		; print out int

print_int:			; prints out the int
	push r27		; save pointer value
	push r26		; save pointer value
	push r25		; save int value
        push r24		; save int value
        push r23		; save string format
        push r22		; save string format
        call printf		; print string
        pop r22			; restore string format
        pop r23			; restore string format
        pop r24			; restore int value
        pop r25			; restore int value
	pop r26			; restore pointer value
	pop r27			; restore pointer value

        ret			; return

;;; Print new line
	
prt_newline: 			; begin prt_newline
	push r24		; save values
	push r25		; save values
	
        clr r25			; clr high byte
        ldi r24, 10		; 10 is the character '\n'
        call putchar		; print character '\n'

	pop r25			; restore values
	pop r24			; restore values
	
        ret			; return

