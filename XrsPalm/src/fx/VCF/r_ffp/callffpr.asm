;	callffpr.asm	1.3	residentable replacement for DICE float functions
;
;	r_float callffp( (*MatFunc)( r_float, ... ), ... );
;	long	cmpable( r_float );

	xdef	_callffp
	xdef	_cmpable

	; pinon offset	0	4	8	C	10	14
	; pinon arvo	paluuos	kutsuos	param1	param2	param3	...
	; uusi pinon a.	?	paluuos	param1	param2	param3	...

	section	text,code
	ds.l	0

	; a4 on DICE:n osoitin data-muistin alkuun

_callffp
	move.l	(sp)+,callffp_storage(a4) ; paluuosoite pinosta talteen (0)
	move.l	(sp),a0		; haetaan kutsuosoite pinosta (4)
	lea	paluu(pc),a1	; lasketaan matikkafunktiolle paluuosoite
	move.l	a1,(sp)		; ja laitetaan se pinoon (4)
	jmp	(a0)		; hyp‰t‰‰n kutsuosoitteeseen, pino +4

paluu
	subq.l	#4,sp		; korjataan pino; pino +4
	move.l	callffp_storage(a4),a0	; haetaan meid‰n paluuosoitteemme
	jmp	(a0)		; ja palataan k‰sin

_cmpable
	move.l	4(sp),d0
	ror.l	#8,d0
	rts

	section	data,data

	ds.w	0

callffp_storage
	dc.l	$0

	end
