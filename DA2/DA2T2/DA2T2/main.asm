;
; DA2T2.asm
;
; Created: 3/4/2016 22:02
; Author : Martin
;

.def	zero	= r0

.macro		initstack
	ldi		r16,	high(ramend)
	out		sph,	r16
	ldi		r16,	low(ramend)
	out		spl,	r16
.endmacro

initstack					;; initiate stack.
	sub		zero,	zero	;; make zero register.
	ldi		r20,	0		;; initiate counter.
;	sbi		ddrc,	0		;; make PC0 an output.
	ldi		r16,	0xFF	;;
	out		ddrb,	r16		;; make all of portB an output.
	ldi		r17,	0
	ldi		r16,	0x01
;	out		portc,	r16		;; output initial value of 1.
start:
	call	delay
	eor		r17,	r16		;; toggle output value.
	sbrc	r17,	0		;; if (r17 == 1)
	inc		r20				;;    r20++
;	out		portc,	r17		;; output 1/0
	out		portb,	r20		;; output count to portB
	rjmp	start

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
