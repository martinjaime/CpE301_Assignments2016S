;
; DA1.asm
;
; Created: 2/18/2016 14:08:21
; Author : Martin Jaime
;

.def	count	=	r22
.def	temp	=	r25
.def	zero	=	r0

.cseg
;******************** Task a ********************;
	sub		zero,	zero	;; make zero register. 
	;; Initialize X and Y pointers to point to ramend / 2
	ldi		xl,		low(ramend / 2)
	ldi		xh,		high(ramend / 2)
	movw	yl,		xl	;; Y = X
	mov		r16,	xl	;; r16 = low(x)
	ldi		count,	25	;; count = 25
loop25:		;; Store 25 integers into ramend/2 and up.
	st		y+,		r16	;; *y = r16; y++
	inc		r16			;; r16++
	dec		count		;; count--
	brne	loop25		;; goto to loop25 if count == 0
;******************** Task b ********************;
	;; reset y register to x
	movw	yl,		xl		;; y = x
	ldi		count,	25		;; counter = 25
	ldi		r17,	7		;; r17 = 7 // divisor
	call	Add25			;; call Add25 routine
	mov		r20,	r10		;; get return values.
	mov		r21,	r11		;;    r21:r20 <- r11:r10
;******************** Task d ********************;
	;; if r21:r20 is larger than 8 bits, set bit 3 in r7
	cp		r21,	zero	;; check if high byte is 0
	breq	lessthan8bits7
	mov		temp,	zero
	sbr		temp,	4
	mov		r7,		temp	;; set bit 3 in r7 is true
lessthan8bits7:
;******************** Task c ********************;
	;; reset y register to x
	movw	yl,		xl		;; y <- x
	ldi		count,	25		;; counter = 25
	ldi		r17,	3		;; r17 = 3 // divisor
	call	Add25
	mov		r23,	r10
	mov		r24,	r11		;; r24:r23 <- r11:r10
;******************** Task d ********************;
	;; if r24:r23 is larger than 8 bits, set bit 3 in r7
	cp		r24,	zero	;; check if high byte is 0
	breq	lessthan8bits3
	mov		temp,	zero
	sbr		temp,	4
	mov		r7,		temp	;; set bit 3 in r7 is true
lessthan8bits3:
end:
	rjmp end

;; Routine to add 25 integers previously stored divisible by 3.
;; Registers used: 
;;		r1, r16, r15, r17, r18, r20, 
Add25:
	clr		r10				;; Clear registers r11:r10
	clr		r11
add25Loop:
	ld		r1,		y+		;; get next value
	mov		r16,	r1		;; store it in r16
	call	div8u			;; call divide routine
	cp		r15,	zero	;; Check if remainder is 0
	brne	notDivByR		;; 
	add		r10,	r1		;; Add if remainder is 0
	adc		r11,	zero	;; add values
notDivByR:
	dec		count			;; loop counter
	brne	add25Loop
	ret						;; return to calling routine

;**** A P P L I C A T I O N   N O T E   A V R 2 0 0 ************************
;*
;* Title:		Multiply and Divide Routines
;* Version:		1.1
;* Last updated:	97.07.04
;* Target:		AT90Sxxxx (All AVR Devices)
;*
;* Support E-mail:	avr@atmel.com
;* 
;***************************************************************************
;*
;* "div8u" - 8/8 Bit Unsigned Division
;*
;* This subroutine divides the two register variables "dd8u" (dividend) and 
;* "dv8u" (divisor). The result is placed in "dres8u" and the remainder in
;* "drem8u".
;*  
;* Number of words	:14
;* Number of cycles	:97
;* Low registers used	:1 (drem8u)
;* High registers used  :3 (dres8u/dd8u,dv8u,dcnt8u)
;*
;***************************************************************************

;***** Subroutine Register Variables

.def	drem8u	=r15		;remainder
.def	dres8u	=r16		;result
.def	dd8u	=r16		;dividend
.def	dv8u	=r17		;divisor
.def	dcnt8u	=r18		;loop counter

;***** Code

div8u:	sub	drem8u,drem8u	;clear remainder and carry
	ldi	dcnt8u,9	;init loop counter
d8u_1:	rol	dd8u		;shift left dividend
	dec	dcnt8u		;decrement counter
	brne	d8u_2		;if done
	ret			;    return
d8u_2:	rol	drem8u		;shift dividend into remainder
	sub	drem8u,dv8u	;remainder = remainder - divisor
	brcc	d8u_3		;if result negative
	add	drem8u,dv8u	;    restore remainder
	clc			;    clear carry to be shifted into result
	rjmp	d8u_1		;else
d8u_3:	sec			;    set carry to be shifted into result
	rjmp	d8u_1