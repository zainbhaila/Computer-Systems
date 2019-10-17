;;; Zain Bhaila
;;; 115197541
;;; zainb

;;; Implementation that computes the nth fibonacci number.
;;; Takes n as uint16_t parameter
	
    .global fib                 ; exposes fib as a label for the linker
fib:                            ; the label: the next instruction starts fib()
	cpi r24, 0		; check if n is 0
	breq 0f			; go to 0 if n is 0

	cpi r24, 1		; check if n is one
	breq 1f			; go to 1 if n is 1

	push r24		; save n
	push r25		; save n

	sbiw r24, 1		; reduce n by 1
	call fib		; fibonacci of n - 1

	pop r23			; get original n value
	pop r22			; get original n value

	push r24		; save return value
	push r25		; save return value

	mov r24, r22		; store n for later use
	mov r25, r23		; store n for later use

	sbiw r24, 2		; subtract 2 from n
	call fib 		; fibonacci of n - 2

	pop r23			; get n - 1 return value
	pop r22			; get n - 1 return value

	add r24, r22		; add values of fib(n - 1) and fib(n - 2)
	adc r25, r23		; add value with carry bit
	
	ret                	; returns.  necessary.

0:				; base case value of 0
	ldi r24, 0		; store 0 in return register
	clr r25			; clear high register
	ret			; return

1:				; base case of value 1
	ldi r24, 1		; store 1 in return register
	clr r25			; clear high register
	ret			; return
