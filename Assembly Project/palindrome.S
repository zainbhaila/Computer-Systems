;;; Zain Bhaila
;;; 115197541
;;; zainb

;;; Check if the word given is a palindrome.
;;; Takes constant character pointer s

	.global is_palindrome	; linker
is_palindrome:			; begin is_palindrome()
	push r28		; save r28 for pointer to string
	push r29		; save r29 for pointer

	mov r28, r24		; save char pointer
	mov r29, r25		; clear high byte of saved pointer

	call strlen		; get length of string

	mov r26, r24		; save strlen
	mov r27, r25		; save strlen

	lsr r25			; divide strlen by 2
	ror r24			; divide strlen by 2 with carry bit

	mov r22, r24		; store strlen / 2
	mov r23, r25		; store strlen / 2

	clr r24			; use r24 for 'i' in loop
	clr r25			; use r25 for 'i' in loop

	mov r30, r28		; store string pointer into new str pointer
	mov r31, r29		; store high of string pointer into new str

	add r30, r26		; add strlen to new str for end pointer
	adc r31, r27		; add with carry bit for end pointer

	sbiw r30, 1		; dec end pointer
loop:
	cp r24, r22		; loop conditional
	cpc r25, r23		; compare i with strlen / 2
	brsh break_success	; end of loop

	ld r18, Y		; load low of string pointer
        ld r19, Y+		; load high of string pointer with post inc

	ld r20, Z		; load last letter for comparison
	ld r21, Z		; load last letter
	
	cp r18, r20		; compare letters
	brne break_failure	; letters are not equal
	
	sbiw r30, 1		; dec end pointer
	adiw r24, 1		; inc i
	jmp loop		; start loop again
	
break_success:			; string is a palindrom
	ldi r24, 1		; return true
	clr r25			; clear high byte
	jmp exit		; go to return

break_failure:			; string is not a palindrome
	ldi r24, 0		; return false
	clr r25			; clear high byte
	jmp exit		; go to return

exit:				; return, end of function
	pop r29			; restore Y
	pop r28			; restore Y

	ret			; return
	
	
;;; Find the length of a string
;;; Takes char pointer, returns uint16_t
	
strlen:				; begin strlen
        mov r27, r25		; copy input to high byte of string pointer
        mov r26, r24		; copy input to low byte of string pointer
        
        clr r24			; clear return register
        clr r25			; clear return register
	
2:
        ld r18, X		; load low byte of string
        ld r19, X+		; load high byte of string with post inc

        cpi r18, 0		; check if value is 0
        breq 1f			; branch to 1 if 0

        adiw r24, 1		; increase string length
       
        jmp 2b			; jump to start of 2
	
1:				; end function
        ret			; return
