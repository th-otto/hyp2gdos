	.text

fillbuf:
	lea.l      bitcount,a0
	move.w     -4(a0),d1
	lsl.w      d0,d1
	move.w     d1,-4(a0)
	bra.s      x168d6
x168a4:
	sub.w      (a0),d0
	move.w     -2(a0),d1
	move.w     d0,d2
	lsl.w      d2,d1
	or.w       d1,-4(a0)
	move.l     8(a0),d1
	beq.s      x168ce
	subq.l     #1,8(a0)
	movea.l    4(a0),a1
	addq.l     #1,4(a0)
	clr.w      d1
	move.b     (a1),d1
	move.w     d1,-2(a0)
	bra.s      x168d2
x168ce:
	clr.w      -2(a0)
x168d2:
	move.w     #8,(a0)
x168d6:
	cmp.w      (a0),d0
	bgt.s      x168a4
	sub.w      d0,(a0)
	move.w     -2(a0),d1
	move.w     (a0),d2
	lsr.w      d2,d1
	or.w       d1,-4(a0)
	rts

getbits:
	move.w     d3,-(a7)
	move.w     d4,-(a7)
	move.w     d0,d3
	move.w     bitbuf,d4
	moveq.l    #16,d1
	sub.b      d3,d1
	lsr.w      d1,d4
	bsr.w      fillbuf
	move.w     d4,d0
	move.w     (a7)+,d4
	move.w     (a7)+,d3
	rts

	.globl x16908
x16908:
	movem.l    d3-d7/a2-a5,-(a7)
	lea.l      -112(a7),a7
	move.w     d0,110(a7)
	moveq.l    #1,d2
	lea.l      76(a7),a3
	bra.s      x16926
x1691c:
	move.w     d2,d0
	add.w      d0,d0
	clr.w      0(a3,d0.w)
	addq.w     #1,d2
x16926:
	cmp.w      #$0010,d2
	bls.s      x1691c
	clr.w      d2
	bra.s      x16942
x16930:
	moveq.l    #0,d0
	move.w     d2,d0
	clr.w      d3
	move.b     0(a0,d0.l),d3
	add.w      d3,d3
	addq.w     #1,0(a3,d3.w)
	addq.w     #1,d2
x16942:
	cmp.w      110(a7),d2
	bcs.s      x16930
	lea.l      6(a7),a2
	clr.w      2(a2)
	moveq.l    #1,d2
	bra.s      x16976
x16954:
	move.w     d2,d0
	add.w      d0,d0
	move.w     0(a3,d0.w),d3
	moveq.l    #16,d0
	sub.b      d2,d0
	lsl.w      d0,d3
	move.w     d2,d0
	add.w      d0,d0
	add.w      0(a2,d0.w),d3
	moveq.l    #1,d0
	add.w      d2,d0
	add.w      d0,d0
	move.w     d3,0(a2,d0.w)
	addq.w     #1,d2
x16976:
	cmp.w      #$0010,d2
	bls.s      x16954
	moveq.l    #16,d0
	sub.w      d1,d0
	move.w     d0,4(a7)
	moveq.l    #1,d2
	lea.l      42(a7),a3
	bra.s      x169b0
x1698c:
	move.w     d2,d0
	add.w      d0,d0
	move.w     0(a2,d0.w),d3
	move.b     5(a7),d4
	lsr.w      d4,d3
	move.w     d3,0(a2,d0.w)
	moveq.l    #1,d3
	move.b     d1,d0
	sub.b      d2,d0
	lsl.w      d0,d3
	move.w     d2,d4
	add.w      d4,d4
	move.w     d3,0(a3,d4.w)
	addq.w     #1,d2
x169b0:
	cmp.w      d2,d1
	bcc.s      x1698c
	bra.s      x169c8
x169b6:
	moveq.l    #1,d0
	moveq.l    #16,d3
	sub.b      d2,d3
	lsl.w      d3,d0
	move.w     d2,d4
	add.w      d4,d4
	move.w     d0,0(a3,d4.w)
	addq.w     #1,d2
x169c8:
	cmp.w      #$0010,d2
	bls.s      x169b6
	move.w     d1,d0
	add.w      d0,d0
	move.w     2(a2,d0.w),d2
	move.b     5(a7),d3
	lsr.w      d3,d2
	tst.w      d2
	beq.s      x169f6
	moveq.l    #1,d4
	lsl.w      d1,d4
	bra.s      x169f2
x169e6:
	moveq.l    #0,d0
	move.w     d2,d0
	add.l      d0,d0
	clr.w      0(a1,d0.l)
	addq.w     #1,d2
x169f2:
	cmp.w      d2,d4
	bne.s      x169e6
x169f6:
	move.w     110(a7),d5
	moveq.l    #1,d0
	moveq.l    #15,d3
	sub.b      d1,d3
	lsl.w      d3,d0
	move.w     d0,(a7)
	clr.w      d0
	bra        x16ac8
x16a0a:
	moveq.l    #0,d3
	move.w     d0,d3
	clr.w      d6
	move.b     0(a0,d3.l),d6
	move.w     d6,d3
	beq        x16ac6
	move.w     d3,d6
	add.w      d6,d6
	move.w     0(a2,d6.w),d6
	move.w     d3,d7
	add.w      d7,d7
	add.w      0(a3,d7.w),d6
	move.w     d6,2(a7)
	cmp.w      d3,d1
	bcs.s      x16a4c
	move.w     0(a2,d7.w),d2
	bra.s      x16a44
x16a38:
	moveq.l    #0,d6
	move.w     d2,d6
	add.l      d6,d6
	move.w     d0,0(a1,d6.l)
	addq.w     #1,d2
x16a44:
	cmp.w      2(a7),d2
	bcs.s      x16a38
	bra.s      x16abc
x16a4c:
	move.w     d3,d6
	add.w      d6,d6
	move.w     0(a2,d6.w),d4
	move.w     d4,d7
	move.b     5(a7),d6
	lsr.w      d6,d7
	moveq.l    #0,d6
	move.w     d7,d6
	add.l      d6,d6
	lea.l      0(a1,d6.l),a4
	move.w     d3,d2
	sub.w      d1,d2
	bra.s      x16ab6
x16a6c:
	move.w     (a4),d6
	bne.s      x16a8e
	clr.w      d7
	move.w     d5,d6
	add.w      d6,d6
	lea.l      x1a578,a5
	move.w     d7,0(a5,d6.w)
	lea.l      x1ad6e,a5
	move.w     d7,0(a5,d6.w)
	move.w     d5,(a4)
	addq.w     #1,d5
x16a8e:
	move.w     d4,d6
	and.w      (a7),d6
	beq.s      x16aa4
	move.w     (a4),d7
	add.w      d7,d7
	lea.l      x1ad6e,a5
	adda.w     d7,a5
	movea.l    a5,a4
	bra.s      x16ab2
x16aa4:
	move.w     (a4),d6
	add.w      d6,d6
	lea.l      x1a578,a5
	adda.w     d6,a5
	movea.l    a5,a4
x16ab2:
	add.w      d4,d4
	subq.w     #1,d2
x16ab6:
	tst.w      d2
	bne.s      x16a6c
	move.w     d0,(a4)
x16abc:
	move.w     d3,d6
	add.w      d6,d6
	move.w     2(a7),0(a2,d6.w)
x16ac6:
	addq.w     #1,d0
x16ac8:
	cmp.w      110(a7),d0
	bcs        x16a0a
	lea.l      112(a7),a7
	movem.l    (a7)+,d3-d7/a2-a5
	rts

	.globl x16ada
x16ada:
	movem.l    d3-d7/a2,-(a7)
	subq.w     #2,a7
	move.w     d0,d7
	move.w     d1,d3
	move.w     d2,(a7)
	move.w     d1,d0
	bsr        getbits
	move.w     d0,d4
	lea.l      x1b762,a2
	tst.w      d4
	bne.s      x16b2a
	move.w     d3,d0
	bsr        getbits
	move.w     d0,d5
	clr.w      d6
	bra.s      x16b0a
x16b04:
	clr.b      0(a2,d6.w)
	addq.w     #1,d6
x16b0a:
	cmp.w      d6,d7
	bgt.s      x16b04
	clr.w      d6
	bra.s      x16b20
x16b12:
	move.w     d6,d0
	add.w      d0,d0
	lea.l      8212(a2),a0 /* 0001d776 */
	move.w     d5,0(a0,d0.w)
	addq.w     #1,d6
x16b20:
	cmp.w      #$0100,d6
	blt.s      x16b12
	bra.w      x16b9c
x16b2a:
	clr.w      d6
	bra.s      x16b7e
x16b2e:
	move.w     8724(a2),d5 /* 0001d976 */
	moveq.l    #13,d0
	lsr.w      d0,d5
	cmp.w      #7,d5
	bne.s      x16b4e
	move.w     #$1000,d0
	bra.s      x16b46
x16b42:
	lsr.w      #1,d0
	addq.w     #1,d5
x16b46:
	move.w     d0,d1
	and.w      8724(a2),d1 /* 0001d976 */
	bne.s      x16b42
x16b4e:
	cmp.w      #7,d5
	bge.s      x16b58
	moveq.l    #3,d0
	bra.s      x16b5c
x16b58:
	moveq.l    #-3,d0
	add.w      d5,d0
x16b5c:
	bsr        fillbuf
	move.b     d5,0(a2,d6.w)
	addq.w     #1,d6
	cmp.w      (a7),d6
	bne.s      x16b7e
	moveq.l    #2,d0
	bsr        getbits
	move.w     d0,d5
	bra.s      x16b7a
x16b74:
	clr.b      0(a2,d6.w)
	addq.w     #1,d6
x16b7a:
	subq.w     #1,d5
	bpl.s      x16b74
x16b7e:
	cmp.w      d6,d4
	bgt.s      x16b2e
	bra.s      x16b8a
x16b84:
	clr.b      0(a2,d6.w)
	addq.w     #1,d6
x16b8a:
	cmp.w      d6,d7
	bgt.s      x16b84
	lea.l      8212(a2),a1 /* 0001d776 */
	moveq.l    #8,d1
	movea.l    a2,a0
	move.w     d7,d0
	bsr        x16908
x16b9c:
	addq.w     #2,a7
	movem.l    (a7)+,d3-d7/a2
	rts

	.globl x16ba4
x16ba4:
	movem.l    d3-d5/a2,-(a7)
	moveq.l    #9,d0
	bsr        getbits
	move.w     d0,d3
	lea.l      x1b564,a2
	tst.w      d3
	bne.s      x16bee
	moveq.l    #9,d0
	bsr        getbits
	move.w     d0,d4
	clr.w      d5
	bra.s      x16bcc
x16bc6:
	clr.b      0(a2,d5.w)
	addq.w     #1,d5
x16bcc:
	cmp.w      #$01FE,d5
	blt.s      x16bc6
	clr.w      d5
	bra.s      x16be4
x16bd6:
	move.w     d5,d0
	add.w      d0,d0
	lea.l      530(a2),a0 /* 0001b776 */
	move.w     d4,0(a0,d0.w)
	addq.w     #1,d5
x16be4:
	cmp.w      #$1000,d5
	blt.s      x16bd6
	bra        x16cb0
x16bee:
	clr.w      d5
	bra        x16c8c
x16bf4:
	move.w     9234(a2),d0 /* 0001d976 */
	lsr.w      #8,d0
	add.w      d0,d0
	lea.l      8722(a2),a0 /* 0001d776 */
	move.w     0(a0,d0.w),d4
	cmp.w      #$0013,d4
	blt.s      x16c38
	move.w     #$0080,d0
x16c0e:
	move.w     9234(a2),d1 /* 0001d976 */
	and.w      d0,d1
	beq.s      x16c24
	move.w     d4,d2
	add.w      d2,d2
	lea.l      -2038(a2),a0 /* 0001ad6e */
	move.w     0(a0,d2.w),d4
	bra.s      x16c30
x16c24:
	move.w     d4,d1
	add.w      d1,d1
	lea.l      -4076(a2),a0 /* 0001a578 */
	move.w     0(a0,d1.w),d4
x16c30:
	lsr.w      #1,d0
	cmp.w      #$0013,d4
	bge.s      x16c0e
x16c38:
	lea.l      510(a2),a0 /* 0001b762 */
	clr.w      d0
	move.b     0(a0,d4.w),d0
	bsr        fillbuf
	cmp.w      #2,d4
	bgt.s      x16c80
	tst.w      d4
	bne.s      x16c54
	moveq.l    #1,d4
	bra.s      x16c7a
x16c54:
	cmp.w      #1,d4
	bne.s      x16c66
	moveq.l    #4,d0
	bsr        getbits
	move.w     d0,d4
	addq.w     #3,d4
	bra.s      x16c7a
x16c66:
	moveq.l    #9,d0
	bsr        getbits
	move.w     d0,d4
	add.w      #$0014,d4
	bra.s      x16c7a
x16c74:
	clr.b      0(a2,d5.w)
	addq.w     #1,d5
x16c7a:
	subq.w     #1,d4
	bpl.s      x16c74
	bra.s      x16c8c
x16c80:
	move.b     d4,d0
	add.b      #$FE,d0
	move.b     d0,0(a2,d5.w)
	addq.w     #1,d5
x16c8c:
	cmp.w      d5,d3
	bgt        x16bf4
	bra.s      x16c9a
x16c94:
	clr.b      0(a2,d5.w)
	addq.w     #1,d5
x16c9a:
	cmp.w      #$01FE,d5
	blt.s      x16c94
	lea.l      530(a2),a1 /* 0001b776 */
	moveq.l    #12,d1
	movea.l    a2,a0
	move.w     #$01FE,d0
	bsr        x16908
x16cb0:
	movem.l    (a7)+,d3-d5/a2
	rts

	.globl x16cb6
x16cb6:
	move.w     d3,-(a7)
	move.l     a2,-(a7)
	lea.l      x1d97c,a2
	move.w     (a2),d0 /* 0001d97c */
	bne.s      x16ce4
	moveq.l    #16,d0
	bsr        getbits
	move.w     d0,(a2) /* 0001d97c */
	moveq.l    #3,d2
	moveq.l    #5,d1
	moveq.l    #19,d0
	bsr        x16ada
	bsr        x16ba4
	moveq.l    #-1,d2
	moveq.l    #4,d1
	moveq.l    #14,d0
	bsr        x16ada
x16ce4:
	subq.w     #1,(a2)
	move.w     -6(a2),d0 /* 0001d976 */
	lsr.w      #4,d0
	add.w      d0,d0
	lea.l      -8710(a2),a0 /* 0001b776 */
	move.w     0(a0,d0.w),d3
	cmp.w      #$01FE,d3
	bcs.s      x16d28
	moveq.l    #8,d0
x16cfe:
	move.w     -6(a2),d1 /* 0001d976 */
	and.w      d0,d1
	beq.s      x16d14
	move.w     d3,d2
	add.w      d2,d2
	lea.l      -11278(a2),a0 /* 0001ad6e */
	move.w     0(a0,d2.w),d3
	bra.s      x16d20
x16d14:
	move.w     d3,d1
	add.w      d1,d1
	lea.l      -13316(a2),a0 /* 0001a578 */
	move.w     0(a0,d1.w),d3
x16d20:
	lsr.w      #1,d0
	cmp.w      #$01FE,d3
	bcc.s      x16cfe
x16d28:
	lea.l      -9240(a2),a0 /* 0001b564 */
	clr.w      d0
	move.b     0(a0,d3.w),d0
	bsr        fillbuf
	move.w     d3,d0
	movea.l    (a7)+,a2
	move.w     (a7)+,d3
	rts

	.globl x16d3e
x16d3e:
	move.w     d3,-(a7)
	move.l     a2,-(a7)
	lea.l      x1d776,a2
	move.w     512(a2),d0 /* 0001d976 */
	lsr.w      #8,d0
	add.w      d0,d0
	move.w     0(a2,d0.w),d3
	cmp.w      #14,d3
	bcs.s      x16d88
	move.w     #$0080,d0
x16d5e:
	move.w     512(a2),d1 /* 0001d976 */
	and.w      d0,d1
	beq.s      x16d74
	move.w     d3,d2
	add.w      d2,d2
	lea.l      -10760(a2),a0 /* 0001ad6e */
	move.w     0(a0,d2.w),d3
	bra.s      x16d80
x16d74:
	move.w     d3,d1
	add.w      d1,d1
	lea.l      -12798(a2),a0 /* 0001a578 */
	move.w     0(a0,d1.w),d3
x16d80:
	lsr.w      #1,d0
	cmp.w      #14,d3
	bcc.s      x16d5e
x16d88:
	lea.l      -8212(a2),a0 /* 0001b762 */
	clr.w      d0
	move.b     0(a0,d3.w),d0
	bsr        fillbuf
	tst.w      d3
	beq.s      x16dae
	moveq.l    #-1,d0
	add.w      d3,d0
	bsr        getbits
	moveq.l    #1,d1
	moveq.l    #-1,d2
	add.b      d3,d2
	lsl.w      d2,d1
	add.w      d1,d0
	move.w     d0,d3
x16dae:
	move.w     d3,d0
	movea.l    (a7)+,a2
	move.w     (a7)+,d3
	rts

	.globl x16db6
x16db6:
	move.l     d3,-(a7)
	move.l     a2,-(a7)
	moveq.l    #0,d3
	lea.l      x1d990,a2
	tst.w      d0
	beq.s      x16dfc
	clr.w      -26(a2) /* 0001d976 */
	clr.w      -24(a2) /* 0001d978 */
	clr.w      -22(a2) /* 0001d97a */
	moveq.l    #16,d0
	bsr        fillbuf
	clr.w      -20(a2) /* 0001d97c */
	clr.w      -2(a2) /* 0001d98e */
	bra.s      x16e02
x16de2:
	move.l     (a2),d0 /* 0001d990 */
	movea.l    -10(a2),a0 /* 0001d986 */
	movea.l    -10(a2),a1 /* 0001d986 */
	move.b     0(a0,d0.l),0(a1,d3.l)
	addq.l     #1,(a2) /* 0001d990 */
	addq.l     #1,d3
	cmp.l      -6(a2),d3 /* 0001d98a */
	beq.s      x16e5c
x16dfc:
	subq.w     #1,-2(a2) /* 0001d98e */
	bpl.s      x16de2
x16e02:
	bsr        x16cb6
	cmp.w      #$00FF,d0
	bhi.s      x16e1e
	movea.l    -10(a2),a0 /* 0001d986 */
	move.b     d0,0(a0,d3.l)
	addq.l     #1,d3
	cmp.l      -6(a2),d3 /* 0001d98a */
	beq.s      x16e5c
	bra.s      x16e02
x16e1e:
	move.w     d0,d1
	add.w      #$FF03,d1
	move.w     d1,-2(a2) /* 0001d98e */
	bsr        x16d3e
	moveq.l    #0,d1
	move.w     d0,d1
	move.l     d3,d0
	sub.l      d1,d0
	subq.l     #1,d0
	move.l     d0,(a2) /* 0001d990 */
	bra.s      x16e54
x16e3a:
	move.l     (a2),d0 /* 0001d990 */
	movea.l    -10(a2),a0 /* 0001d986 */
	movea.l    -10(a2),a1 /* 0001d986 */
	move.b     0(a0,d0.l),0(a1,d3.l)
	addq.l     #1,(a2) /* 0001d990 */
	addq.l     #1,d3
	cmp.l      -6(a2),d3 /* 0001d98a */
	beq.s      x16e5c
x16e54:
	subq.w     #1,-2(a2) /* 0001d98e */
	bpl.s      x16e3a
	bra.s      x16e02
x16e5c:
	movea.l    (a7)+,a2
	move.l     (a7)+,d3
	rts

	.bss
	
x1a578:
x1ad6e:
x1b762:
x1b564:

x1d776:

bitbuf: .ds.w 1
subbitbuf: .ds.w 1

bitcount: .ds.w 1
x1d97c:

dec_j: .ds.w 1
x1d990:
