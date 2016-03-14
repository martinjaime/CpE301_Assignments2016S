;
; DA2T1.asm
;
; Created: 3/1/2016 18:47:33
; Author : Martin
;

.macro		initstack
	ldi		r16,	high(ramend)
	out		sph,	r16
	ldi		r16,	low(ramend)
	out		spl,	r16
.endmacro

initstack 
	sbi		ddrc,	0		;; Set first pin as output
	ldi		r17,	0
	ldi		r16,	0x01
	out		portc,	r16		;; Set PC0 high. 
start:
	call	delay
	eor		r17,	r16		;; toggle value written to PC0
	out		portc,	r17		;; output toggled value to PC0
	rjmp	start			;; Restart loop

;; Delay routine that delays for a 0.25 s
delay:
	;; Set timer/counter1 to 0x85EE
	push	r20		;; store r20 and restore before return.
	ldi		r20,	high(34286)
	sts		tcnt1h,	r20		
	ldi		r20,	low(34286)
	sts		tcnt1l,	r20		; tcnt1 = 34286
	ldi		r20,	0x00	;
	sts		tccr1a,	r20		; wgm11:10 = 0x00. Normal operation
	ldi		r20,	0x03
	sts		tccr1b,	r20		; cs1 = 0x03. Prescaler = 64
again:
	in		r20,	tifr1	; read tifr
	sbrs	r20,	tov1	; if tov1 is set, skip next instruction
	rjmp	again
	ldi		r20,	0x00
	sts		tccr1b, r20		; stop timer1. Resetting CS1
	ldi		r20,	0x01
	out		tifr1,	r20		; clear tov1 flag
	pop		r20
	ret
