;;; Zain Bhaila
;;; 115197541
;;; zainb

;;; Bitwise computation of sqrt of an integer.
;;; Takes unit8_t as parameter n, return uint8_t

	.global isqrt		; linker
isqrt:				; begin integer sqrt
	push r22		; save old n value
	
	cpi r24, 2		; check if input is less than 2
	brlo exit		; if input is less than 2, quit

	mov r22, r24		; save n for later
	clr r23			; clear high byte

	;; 0 digit calculation (small)
	lsr r24			; divide by 2
	lsr r24			; divide by 2
	clr r25			; clear high byte
	call isqrt		; recursively take sqrt of input
	lsl r24			; multiply by 2
	clr r25			; clear high byte

	mov r20, r24		; store small value for later use
	clr r21			; clear high byte

	;; 1 digit calculation (large)
	inc r24			; increase small value by 1
	mov r18, r24		; store large value for later use
	clr r19			; clear high byte

	mul r18, r18		; square large value
	clr r1			; set back to 0
	cp r22, r0		; check if large squared is n
	brlo exit_small		; if n is smaller than large squared, exit
	
exit:				; exit function
	clr r25			; clear high byte
	pop r22			; restore original n
	ret			; return

exit_small:			; if large value squared is greater than n
	mov r24, r20		; set return to small
	jmp exit
