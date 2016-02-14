;
; DA0T1.asm
;
; Created: 2/11/2016 19:06:27
; Author : Martin Jaime-Viveros
;
.cseg
start:
	;; Load arbitrary immediates into registers 16:20
  ldi		r16,	59
	ldi		r17,	59
	ldi		r18,	59
	ldi		r19,	59
	ldi		r20,	59

	sbi		DDRB,	2		;; Set port B pin 2 as output
	out		PORTB,	r0		;; Set all outputs to 0

	;; Add the integers
	add		r16,	r17
	add		r16,	r18
	add		r16,	r19
	add		r16,	r20

	brcc	End				;; Check if carry flag set
	ldi		r17,	4
	out		PORTB,	r17		;; Set output high at pin2 on port B
End:
    rjmp	End
