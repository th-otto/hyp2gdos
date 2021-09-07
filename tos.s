				MACRO	CALLDOS
				trap	#1
				ENDM
				
				GLOBL	Kbshift
				MODULE	Kbshift
				pea     (a2)
				move.w	d0,-(a7)
				move.w	#11,-(a7)
				trap #13
				addq.w	#4,a7
				movea.l	(a7)+,a2
				rts
				ENDMOD

				GLOBL	Tgetdate
				MODULE	Tgetdate
				pea		(a2)
				move.w	#$2A,-(a7)
				CALLDOS
				addq.w	#2,a7
				move.l	(a7)+,a2
				rts
				ENDMOD

				GLOBL	Mfree
				MODULE	Mfree
				pea		(a2)
				pea		(a0)
				move.w	#$49,-(a7)
				CALLDOS
				addq.w	#6,a7
				move.l	(a7)+,a2
				rts
				ENDMOD

				GLOBL	Malloc
				MODULE	Malloc
				pea		(a2)
				move.l	d0,-(a7)
				move.w	#$48,-(a7)
				CALLDOS
				addq.w	#6,a7
				movea.l	d0,a0
				move.l	(a7)+,a2
				rts
				ENDMOD

				GLOBL	Dgetdrv
				MODULE	Dgetdrv
				pea		(a2)
				move.w	#$19,-(a7)
				CALLDOS
				addq.w	#2,a7
				move.l	(a7)+,a2
				rts
				ENDMOD

				GLOBL	Pdomain
				MODULE	Pdomain
				pea		(a2)
				move.w	d0,-(a7)
				move.w	#$119,-(a7)
				CALLDOS
				addq.w	#4,a7
				move.l	(a7)+,a2
				rts
				ENDMOD
				
				GLOBL	Dgetpath
				MODULE	Dgetpath
				pea		(a2)
				move.w	d0,-(a7)
				pea		(a0)
				move.w	#$47,-(a7)
				CALLDOS
				addq.w	#8,a7
				move.l	(a7)+,a2
				rts
				ENDMOD

				GLOBL	Supexec
				MODULE	Supexec				
				pea     (a2)
				pea     (a0)
				move.w	#$26,-(a7)
				trap #14
				addq.w	#6,a7
				movea.l	(a7)+,a2
				rts
				ENDMOD

