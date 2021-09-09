	.text

	.globl x132c8
x132c8:
	movem.l    d3-d7/a2-a3,-(a7)
	movea.l    a0,a2
	move.w     d0,d7
	move.w     d1,d4
	move.w     d2,d5
	lea.l      x1915c,a1
	moveq.l    #1,d0
	bsr        x177fc
	move.w     d0,d3
	lea.l      x1910c,a3
	bra.s      x13310
x132ea:
	move.w     d5,d0
	sub.w      d4,d0
	move.w     d0,d6
	ext.l      d6
	divs.w     d3,d6
	cmp.w      #$0050,d6
	ble.s      x132fc
	moveq.l    #80,d6
x132fc:
	move.w     d6,d2
	movea.l    a3,a1
	move.w     d7,d1
	move.w     d4,d0
	movea.l    a2,a0
	bsr        x1777e
	move.w     d6,d0
	muls.w     d3,d0
	add.w      d0,d4
x13310:
	move.w     d4,d0
	add.w      d3,d0
	cmp.w      d0,d5
	bge.s      x132ea
	cmp.w      d4,d5
	ble.s      x1332c
	moveq.l    #1,d2
	movea.l    a3,a1
	move.w     d7,d1
	move.w     d5,d0
	sub.w      d3,d0
	movea.l    a2,a0
	bsr        x1777e
x1332c:
	movem.l    (a7)+,d3-d7/a2-a3
	rts

	.globl x13332
x13332:
	movem.l    d3/a2-a3,-(a7)
	subq.w     #8,a7
	movea.l    a0,a2
	movea.l    20(a2),a3
	movea.l    (a0),a0
	bsr        x172a4
	move.w     2(a3),-(a7)
	clr.w      d2
	moveq.l    #7,d1
	move.w     6(a3),d0
	movea.l    (a2),a0
	bsr        x176ae
	addq.w     #2,a7
	pea.l      4(a7)
	lea.l      10(a7),a1
	movea.l    (a2),a0
	bsr        x17712
	addq.w     #4,a7
	movea.l    16(a3),a0
	bsr        strlen
	move.l     d0,d3
	tst.w      d3
	ble.s      x1338a
	move.w     d3,d0
	movea.l    16(a3),a1
	movea.l    (a2),a0
	bsr        x177fc
	ext.l      d0
	divs.w     d3,d0
	move.w     d0,6(a7)
x1338a:
	move.w     (a3),-(a7)
	clr.w      d2
	moveq.l    #7,d1
	move.w     6(a3),d0
	movea.l    (a2),a0
	bsr        x176ae
	addq.w     #2,a7
	pea.l      (a7)
	lea.l      6(a7),a1
	movea.l    (a2),a0
	bsr        x17712
	addq.w     #4,a7
	move.w     (a7),d0
	cmp.w      4(a7),d0
	ble.s      x133b6
	move.w     d0,4(a7)
x133b6:
	movea.l    (a2),a0
	bsr        x172c8
	move.w     6(a7),12(a2)
	move.w     4(a7),14(a2)
	addq.w     #8,a7
	movem.l    (a7)+,d3/a2-a3
	rts

	.globl x133d0
x133d0:
	movem.l    d3-d5/a2-a5,-(a7)
	lea.l      -10(a7),a7
	move.l     a0,6(a7)
	movea.l    a1,a3
	move.l     d0,d5
	movea.l    42(a7),a4
	movea.l    46(a7),a2
	lea.l      4(a7),a1
	movea.l    a2,a0
	bsr        dec_255_decode
	movea.l    a0,a2
	pea.l      (a7)
	lea.l      6(a7),a1
	move.w     8(a7),d0
	movea.l    20(a3),a0
	movea.l    20(a0),a5
	movea.l    10(a7),a0
	jsr        (a5)
	addq.w     #4,a7
	moveq.l    #-1,d0
	move.w     d0,-(a7)
	move.w     2(a7),d2
	move.w     d0,d1
	movea.l    (a3),a0
	move.w     4(a7),d0
	bsr        x176ae
	addq.w     #2,a7
	move.b     (a2)+,d3
	ext.w      d3
	add.w      #$FFE0,d3
	tst.w      d3
	bgt.s      x13452
	move.w     4(a7),d0
	ext.l      d0
	lsl.l      #2,d0
	movea.l    6(a7),a0
	movea.l    278(a0),a1
	movea.l    0(a1,d0.l),a5
	lea.l      14(a5),a5
	movea.l    a5,a0
	bsr        strlen
	move.l     d0,d3
	bra.s      x13456
x13452:
	movea.l    a2,a5
	adda.w     d3,a2
x13456:
	movea.l    50(a7),a0
	clr.w      (a0)
	clr.w      d4
	bra.s      x1347a
x13460:
	moveq.l    #1,d0
	movea.l    a5,a1
	movea.l    (a3),a0
	bsr        x177fc
	movea.l    50(a7),a0
	add.w      d0,(a0)
	addq.l     #1,(a4)
	cmp.l      (a4),d5
	ble.s      x1347e
	addq.w     #1,a5
	addq.w     #1,d4
x1347a:
	cmp.w      d4,d3
	bgt.s      x13460
x1347e:
	moveq.l    #-1,d0
	move.w     d0,-(a7)
	clr.w      d2
	move.w     d0,d1
	movea.l    20(a3),a0
	move.w     6(a0),d0
	movea.l    (a3),a0
	bsr        x176ae
	addq.w     #2,a7
	movea.l    a2,a0
	lea.l      10(a7),a7
	movem.l    (a7)+,d3-d5/a2-a5
	rts

	.globl 134a2
x134a2:
	movem.l    d3-d7/a2-a5,-(a7)
	lea.l      -12(a7),a7
	move.l     a0,8(a7)
	movea.l    a1,a2
	movea.l    52(a7),a4
	move.l     d0,d6
	movea.l    56(a7),a5
	movea.l    20(a2),a3
	movea.l    (a1),a0
	bsr        x172a4
	clr.w      d3
	movea.l    8(a7),a0
	movea.l    (a0),a1
	cmpi.l     #$48444F43,262(a1)
	beq.s      x134dc
	move.w     12(a3),d0
	beq.s      x134e0
x134dc:
	move.w     (a3),d4
	bra.s      x134e4
x134e0:
	move.w     2(a3),d4
x134e4:
	move.w     d4,-(a7)
	clr.w      d2
	moveq.l    #7,d1
	move.w     6(a3),d0
	movea.l    (a2),a0
	bsr        x176ae
	addq.w     #2,a7
	clr.w      (a5)
	moveq.l    #0,d5
	move.l     d5,(a7)
	bra        x1367a
x13500:
	cmpi.b     #$1B,(a4)
	beq.w      x13578
	move.w     8(a3),d0
	beq.s      x1356a
	cmpi.b     #$09,(a4)
	bne.s      x13538
	addq.w     #1,a4
	move.w     14(a3),d0
	ext.l      d0
	add.l      d5,d0
	move.w     14(a3),d1
	ext.l      d1
	bsr        _ldiv
	move.w     14(a3),d1
	ext.l      d1
	bsr        _lmul
	move.l     d0,d5
	addq.l     #1,(a7)
	bra.s      x13558
x13538:
	cmpi.b     #$20,(a4)
	bne.s      x1356a
	cmpi.b     #$20,1(a4)
	bne.s      x1356a
	bra.s      x1354e
x13548:
	addq.w     #1,a4
	addq.l     #1,d5
	addq.l     #1,(a7)
x1354e:
	cmpi.b     #$20,(a4)
	bne.s      x13558
	cmp.l      (a7),d6
	bgt.s      x13548
x13558:
	move.l     d5,d0
	move.w     12(a2),d1
	ext.l      d1
	bsr        _lmul
	move.w     d0,(a5)
	bra        x1367a
x1356a:
	moveq.l    #1,d0
	movea.l    a4,a1
	movea.l    (a2),a0
	bsr        x177fc
	add.w      d0,(a5)
	bra.s      x135ce
x13578:
	movea.l    8(a7),a0
	movea.l    (a0),a1
	cmpi.l     #$48444F43,262(a1)
	bne.s      x135be
	addq.w     #1,a4
	move.b     (a4),d0
	ext.w      d0
	sub.w      #$001B,d0
	cmp.w      #12,d0
	bhi        x1364a
	add.w      d0,d0
	move.w     x135a4(pc,d0.w),d0
	jmp        x135a4(pc,d0.w)
x135a4:
	.dc.w x135be-x135a4
	.dc.w x1364a-x135a4
	.dc.w x1364a-x135a4
	.dc.w x1364a-x135a4
	.dc.w x1364a-x135a4
	.dc.w x1364a-x135a4
	.dc.w x1364a-x135a4
	.dc.w x1364a-x135a4
	.dc.w x1364a-x135a4
	.dc.w x135d8-x135a4
	.dc.w x135fc-x135a4
	.dc.w x135d8-x135a4
	.dc.w x135fc-x135a4
x135be:
	moveq.l    #1,d0
	movea.l    a4,a1
	movea.l    (a2),a0
	bsr        x177fc
	ext.l      d0
	add.l      d0,d0
	adda.l     d0,a5
x135ce:
	addq.w     #1,a4
	addq.l     #1,d5
	addq.l     #1,(a7)
	bra        x1367a
x135d8:
	move.l     (a7),d7
	pea.l      4(a7)
	pea.l      1(a4)
	pea.l      8(a7)
	move.l     d6,d0
	movea.l    a2,a1
	movea.l    20(a7),a0
	movea.l    (a0),a0
	bsr        x133d0
	lea.l      12(a7),a7
	movea.l    a0,a4
	bra.s      x1362a
x135fc:
	move.l     (a7),d7
	lea.l      6(a7),a1
	lea.l      1(a4),a0
	bsr        dec_255_decode
	movea.l    a0,a4
	pea.l      4(a7)
	move.l     a0,-(a7)
	pea.l      8(a7)
	move.l     d6,d0
	movea.l    a2,a1
	movea.l    20(a7),a0
	movea.l    (a0),a0
	bsr        x133d0
	lea.l      12(a7),a7
	movea.l    a0,a4
x1362a:
	move.l     (a7),d0
	sub.l      d7,d0
	add.l      d0,d5
	move.w     4(a7),d1
	add.w      d1,(a5)
	move.w     d4,-(a7)
	move.w     d3,d2
	moveq.l    #-1,d1
	move.w     6(a3),d0
	movea.l    (a2),a0
	bsr        x176ae
	addq.w     #2,a7
	bra.s      x1367a
x1364a:
	cmpi.b     #$64,(a4)
	bcs.s      x13678
	clr.w      d3
	move.b     (a4),d3
	add.w      #$FF9C,d3
	moveq.l    #64,d0
	and.w      d3,d0
	beq.s      x13664
	move.w     2(a3),d4
	bra.s      x13666
x13664:
	move.w     (a3),d4
x13666:
	move.w     d4,-(a7)
	move.w     d3,d2
	moveq.l    #-1,d1
	move.w     6(a3),d0
	movea.l    (a2),a0
	bsr        x176ae
	addq.w     #2,a7
x13678:
	addq.w     #1,a4
x1367a:
	move.b     (a4),d0
	beq.s      x13684
	cmp.l      (a7),d6
	bgt        x13500
x13684:
	cmp.l      (a7),d6
	ble.s      x1369a
	move.l     d5,d0
	sub.l      (a7),d0
	add.l      d6,d0
	move.w     12(a2),d1
	ext.l      d1
	bsr        _lmul
	move.w     d0,(a5)
x1369a:
	movea.l    8(a7),a0
	movea.l    (a0),a1
	cmpi.l     #$48444F43,262(a1)
	beq.s      x136b0
	move.w     12(a3),d0
	beq.s      x136b4
x136b0:
	move.w     (a3),d0
	bra.s      x136b8
x136b4:
	move.w     2(a3),d0
x136b8:
	move.w     d0,-(a7)
	clr.w      d2
	moveq.l    #-1,d1
	moveq.l    #-1,d0
	movea.l    (a2),a0
	bsr        x176ae
	addq.w     #2,a7
	movea.l    (a2),a0
	bsr        x172c8
	clr.w      d0
	lea.l      12(a7),a7
	movem.l    (a7)+,d3-d7/a2-a5
	rts

	.globl x136da
x136da:
	move.l     a2,-(a7)
	movea.l    a0,a2
	move.l     8(a7),-(a7)
	lsl.l      #2,d0
	movea.l    8(a2),a0
	move.l     0(a0,d0.l),-(a7)
	movea.l    a2,a0
	move.l     d1,d0
	bsr        x134a2
	addq.w     #8,a7
	movea.l    (a7)+,a2
	rts

	.globl x136fa
x136fa:
	movem.l    d3-d4/a2-a6,-(a7)
	subq.w     #6,a7
	movea.l    a0,a4
	movea.l    a1,a3
	movea.l    38(a7),a5
	movea.l    46(a7),a2
	lea.l      4(a7),a1
	movea.l    a2,a0
	bsr        dec_255_decode
	movea.l    a0,a2
	pea.l      (a7)
	lea.l      6(a7),a1
	move.w     8(a7),d0
	movea.l    20(a3),a0
	movea.l    20(a0),a6
	movea.l    a4,a0
	jsr        (a6)
	addq.w     #4,a7
	moveq.l    #-1,d0
	move.w     d0,-(a7)
	move.w     2(a7),d2
	move.w     d0,d1
	movea.l    (a3),a0
	move.w     4(a7),d0
	bsr        x176ae
	addq.w     #2,a7
	move.b     (a2)+,d3
	ext.w      d3
	add.w      #$FFE0,d3
	tst.w      d3
	bgt.s      x13790
	move.w     4(a7),d0
	ext.l      d0
	lsl.l      #2,d0
	movea.l    278(a4),a0
	movea.l    0(a0,d0.l),a1
	lea.l      14(a1),a1
	movea.l    a1,a4
	movea.l    a4,a0
	bsr        strlen
	move.l     d0,d3
	move.w     d3,d2
	movea.l    a4,a1
	movea.l    42(a7),a0
	move.w     (a0),d1
	move.w     (a5),d0
	movea.l    (a3),a0
	bsr        x1777e
	move.w     d3,d0
	movea.l    a4,a1
	movea.l    (a3),a0
	bsr        x177fc
	move.w     d0,d4
	bra.s      x137b0
x13790:
	move.w     d3,d2
	movea.l    a2,a1
	movea.l    42(a7),a0
	move.w     (a0),d1
	move.w     (a5),d0
	movea.l    (a3),a0
	bsr        x1777e
	move.w     d3,d0
	movea.l    a2,a1
	movea.l    (a3),a0
	bsr        x177fc
	move.w     d0,d4
	adda.w     d3,a2
x137b0:
	add.w      d4,(a5)
	movea.l    50(a7),a0
	add.w      d3,(a0)
	movea.l    a2,a0
	addq.w     #6,a7
	movem.l    (a7)+,d3-d4/a2-a6
	rts

	.globl x137c2
x137c2:
	movem.l    d3-d6/a2-a5,-(a7)
	lea.l      -14(a7),a7
	move.l     a0,10(a7)
	move.l     a1,6(a7)
	move.l     d0,d6
	move.w     d1,4(a7)
	move.w     d2,2(a7)
	movea.l    20(a0),a3
	movea.l    (a0),a4
	move.w     d1,d3
	clr.w      d4
	movea.l    (a1),a0
	cmpi.l     #$48444F43,262(a0)
	beq.s      x137f8
	move.w     12(a3),d2
	beq.s      x137fc
x137f8:
	move.w     (a3),d5
	bra.s      x13800
x137fc:
	move.w     2(a3),d5
x13800:
	move.w     d5,-(a7)
	move.w     d4,d2
	moveq.l    #-1,d1
	move.w     6(a3),d0
	movea.l    a4,a0
	bsr        x176ae
	addq.w     #2,a7
	move.l     d6,d0
	lsl.l      #2,d0
	movea.l    6(a7),a0
	movea.l    8(a0),a1
	movea.l    0(a1,d0.l),a2
	clr.w      (a7)
	bra        x139ec
x13828:
	cmpi.b     #$1B,(a2)
	beq        x138f4
	movea.l    a2,a5
	move.w     8(a3),d0
	beq        x138be
	cmpi.b     #$09,(a2)
	bne.s      x13856
	addq.w     #1,a2
	move.w     (a7),d1
	add.w      14(a3),d1
	ext.l      d1
	divs.w     14(a3),d1
	muls.w     14(a3),d1
	move.w     d1,(a7)
	bra.s      x13870
x13856:
	cmpi.b     #$20,(a2)+
	bne.s      x13898
	cmpi.b     #$20,(a2)
	bne.s      x13898
x13862:
	addq.w     #1,a2
	cmpi.b     #$20,(a2)
	beq.s      x13862
	move.l     a2,d0
	sub.l      a5,d0
	add.w      d0,(a7)
x13870:
	movea.l    10(a7),a0
	move.w     12(a0),d6
	muls.w     (a7),d6
	add.w      d3,d6
	tst.w      d4
	beq.s      x13890
	move.w     d6,d2
	move.w     4(a7),d1
	move.w     2(a7),d0
	movea.l    a4,a0
	bsr        x132c8
x13890:
	move.w     d6,4(a7)
	bra        x139ec
x13898:
	move.b     (a2),d0
	ext.w      d0
	tst.w      d0
	beq.s      x138ca
	sub.w      #9,d0
	beq.s      x138ca
	sub.w      #$0012,d0
	beq.s      x138ca
	subq.w     #5,d0
	beq.s      x138b2
	bra.s      x138ba
x138b2:
	cmpi.b     #$20,1(a2)
	beq.s      x138ca
x138ba:
	addq.w     #1,a2
	bra.s      x13898
x138be:
	addq.w     #1,a2
	move.b     (a2),d0
	beq.s      x138ca
	cmp.b      #$1B,d0
	bne.s      x138be
x138ca:
	move.l     a2,d6
	sub.l      a5,d6
	move.w     d6,d2
	movea.l    a5,a1
	move.w     2(a7),d1
	move.w     4(a7),d0
	movea.l    a4,a0
	bsr        x1777e
	move.w     d6,d0
	movea.l    a5,a1
	movea.l    a4,a0
	bsr        x177fc
	add.w      d0,4(a7)
	add.w      d6,(a7)
	bra        x139ec
x138f4:
	movea.l    6(a7),a0
	movea.l    (a0),a1
	cmpi.l     #$48444F43,262(a1)
	bne.s      x1393a
	addq.w     #1,a2
	move.b     (a2),d0
	ext.w      d0
	sub.w      #$001B,d0
	cmp.w      #12,d0
	bhi        x139bc
	add.w      d0,d0
	move.w     x13920(pc,d0.w),d0
	jmp        x13920(pc,d0.w)
x13920:
	dc.w x1393a-x13920
	dc.w x139bc-x13920
	dc.w x139bc-x13920
	dc.w x139bc-x13920
	dc.w x139bc-x13920
	dc.w x139bc-x13920
	dc.w x139bc-x13920
	dc.w x139bc-x13920
	dc.w x139bc-x13920
	dc.w x13962-x13920
	dc.w x13986-x13920
	dc.w x13962-x13920
	dc.w x13986-x13920
x1393a:
	moveq.l    #1,d2
	movea.l    a2,a1
	move.w     2(a7),d1
	move.w     4(a7),d0
	movea.l    a4,a0
	bsr        x1777e
	moveq.l    #1,d0
	movea.l    a2,a1
	movea.l    a4,a0
	bsr        x177fc
	add.w      d0,4(a7)
	addq.w     #1,a2
	addq.w     #1,(a7)
	bra        x139ec
x13962:
	pea.l      (a7)
	pea.l      1(a2)
	pea.l      10(a7)
	pea.l      16(a7)
	movea.l    26(a7),a1
	movea.l    22(a7),a0
	movea.l    (a0),a0
	bsr        x136fa
	lea.l      16(a7),a7
	movea.l    a0,a2
	bra.s      x139a8
x13986:
	pea.l      (a7)
	pea.l      3(a2)
	pea.l      10(a7)
	pea.l      16(a7)
	movea.l    26(a7),a1
	movea.l    22(a7),a0
	movea.l    (a0),a0
	bsr        x136fa
	lea.l      16(a7),a7
	movea.l    a0,a2
x139a8:
	move.w     d5,-(a7)
	move.w     d4,d2
	moveq.l    #-1,d1
	move.w     6(a3),d0
	movea.l    a4,a0
	bsr        x176ae
	addq.w     #2,a7
	bra.s      x139ec
x139bc:
	cmpi.b     #$64,(a2)
	bcs.s      x139ea
	clr.w      d4
	move.b     (a2),d4
	add.w      #$FF9C,d4
	moveq.l    #64,d0
	and.w      d4,d0
	beq.s      x139d6
	move.w     2(a3),d5
	bra.s      x139d8
x139d6:
	move.w     (a3),d5
x139d8:
	move.w     d5,-(a7)
	move.w     d4,d2
	moveq.l    #-1,d1
	move.w     6(a3),d0
	movea.l    a4,a0
	bsr        x176ae
	addq.w     #2,a7
x139ea:
	addq.w     #1,a2
x139ec:
	move.b     (a2),d0
	bne        x13828
	movea.l    6(a7),a0
	movea.l    (a0),a1
	cmpi.l     #$48444F43,262(a1)
	beq.s      x13a08
	move.w     12(a3),d1
	beq.s      x13a0c
x13a08:
	move.w     (a3),d0
	bra.s      x13a10
x13a0c:
	move.w     2(a3),d0
x13a10:
	move.w     d0,-(a7)
	clr.w      d2
	moveq.l    #-1,d1
	moveq.l    #-1,d0
	movea.l    a4,a0
	bsr        x176ae
	addq.w     #2,a7
	lea.l      14(a7),a7
	movem.l    (a7)+,d3-d6/a2-a5
	rts

	.globl x13a2a
x13a2a:
	movem.l    d3-d7/a2-a6,-(a7)
	lea.l      -28(a7),a7
	movea.l    a0,a4
	move.l     a1,24(a7)
	move.l     d0,d7
	move.l     d1,20(a7)
	move.l     d2,d6
	movea.l    4(a1),a3
	lea.l      14(a3),a3
	movea.l    4(a1),a0
	move.l     6(a0),d0
	lea.l      0(a3,d0.l),a1
	move.l     a1,16(a7)
	movea.l    24(a7),a0
	movea.l    (a0),a1
	cmpi.l     #$48444F43,262(a1)
	bne        x13cec
	lea.l      6(a7),a5
	lea.l      12(a7),a2
	bra        x13cde
x13a76:
	move.b     1(a3),d0
	ext.w      d0
	sub.w      #$0023,d0
	cmp.w      #$0012,d0
	bhi        x13cda
	add.w      d0,d0
	move.w     x13a92(pc,d0.w),d0
	jmp        x13a92(pc,d0.w)
x13a92:
	dc.w $0234   ; x13cc6-x13a92
	dc.w $0248   ; x13cda-x13a92
	dc.w $0248   ; x13cda-x13a92
	dc.w $0248   ; x13cda-x13a92
	dc.w $0248   ; x13cda-x13a92
	dc.w $0228   ; x13cba-x13a92
	dc.w $0228   ; x13cba-x13a92
	dc.w $0228   ; x13cba-x13a92
	dc.w $0228   ; x13cba-x13a92
	dc.w $0228   ; x13cba-x13a92
	dc.w $0228   ; x13cba-x13a92
	dc.w $0228   ; x13cba-x13a92
	dc.w $0228   ; x13cba-x13a92
	dc.w $0228   ; x13cba-x13a92
	dc.w $0242   ; x13cd4-x13a92
	dc.w $0026   ; x13ab8-x13a92
	dc.w $01cc   ; x13c5e-x13a92
	dc.w $01cc   ; x13c5e-x13a92
	dc.w $01cc   ; x13c5e-x13a92
x13ab8:
	lea.l      14(a7),a1
	lea.l      2(a3),a0
	bsr        dec_255_decode
	movea.l    24(a7),a0
	movea.l    4(a0),a1
	movea.l    (a1),a6
	bra.s      x13ad2
x13ad0:
	movea.l    (a6),a6
x13ad2:
	move.l     a6,d0
	beq.s      x13ae0
	move.w     4(a6),d1
	cmp.w      14(a7),d1
	bne.s      x13ad0
x13ae0:
	move.l     a6,d0
	bne.s      x13b1e
	move.w     #1,8(a7)
	move.w     #1,(a5)
	moveq.l    #2,d1
	movea.l    24(a7),a0
	movea.l    (a0),a1
	and.w      (a1),d1
	beq.s      x13b0e
	move.w     8(a7),d3
	muls.w     12(a4),d3
	ext.l      d3
	move.w     (a5),d4
	muls.w     14(a4),d4
	ext.l      d4
	bra.s      x13b72
x13b0e:
	move.w     8(a7),d3
	lsl.w      #3,d3
	ext.l      d3
	move.w     (a5),d4
	lsl.w      #4,d4
	ext.l      d4
	bra.s      x13b72
x13b1e:
	lea.l      14(a6),a6
	moveq.l    #2,d0
	movea.l    24(a7),a0
	movea.l    (a0),a1
	and.w      (a1),d0
	beq.s      x13b68
	move.w     (a6),d3
	ext.l      d3
	divs.w     #8,d3
	muls.w     12(a4),d3
	move.w     (a6),d1
	ext.l      d1
	divs.w     #8,d1
	swap       d1
	add.w      d1,d3
	ext.l      d3
	move.w     2(a6),d4
	ext.l      d4
	divs.w     #16,d4
	muls.w     14(a4),d4
	move.w     2(a6),d2
	ext.l      d2
	divs.w     #16,d2
	swap       d2
	add.w      d2,d4
	ext.l      d4
	bra.s      x13b72
x13b68:
	move.w     (a6),d3
	ext.l      d3
	move.w     2(a6),d4
	ext.l      d4
x13b72:
	move.w     12(a4),d0
	ext.l      d0
	add.l      d3,d0
	subq.l     #1,d0
	move.w     12(a4),d1
	ext.l      d1
	bsr        _ldiv
	move.w     d0,8(a7)
	move.w     14(a4),d0
	ext.l      d0
	add.l      d4,d0
	subq.l     #1,d0
	move.w     14(a4),d1
	ext.l      d1
	bsr        _ldiv
	move.w     d0,(a5)
	lea.l      10(a7),a1
	lea.l      4(a3),a0
	bsr        x1505e
	move.w     10(a7),d0
	bpl.s      x13bf2
	movea.l    24(a7),a0
	movea.l    (a0),a1
	move.w     306(a1),d0
	muls.w     12(a4),d0
	ext.l      d0
	sub.l      d3,d0
	moveq.l    #2,d1
	bsr        _ldiv
	move.l     d0,d5
	tst.l      d5
	bpl.s      x13bd2
	moveq.l    #0,d5
x13bd2:
	move.w     4(a4),d0
	add.w      d5,d0
	move.w     d0,-(a7)
	move.l     22(a7),d0
	move.w     12(a4),d1
	ext.l      d1
	bsr        _lmul
	move.w     (a7)+,d1
	sub.w      d0,d1
	ext.l      d1
	move.l     d1,d5
	bra.s      x13c0e
x13bf2:
	move.w     10(a7),d0
	ext.l      d0
	sub.l      20(a7),d0
	move.w     12(a4),d1
	ext.l      d1
	bsr        _lmul
	move.l     d0,d5
	add.w      4(a4),d5
	ext.l      d5
x13c0e:
	movea.l    a2,a1
	lea.l      5(a3),a0
	bsr        dec_255_decode
	move.w     (a2),d0
	ext.l      d0
	sub.l      d7,d0
	move.w     14(a4),d1
	ext.l      d1
	bsr        _lmul
	add.w      6(a4),d0
	ext.l      d0
	move.l     d0,(a7)
	move.w     (a5),d1
	ext.l      d1
	cmp.l      d1,d6
	blt.s      x13c56
	move.w     (a2),d2
	ext.l      d2
	cmp.l      d2,d7
	bge.s      x13c56
	move.l     d7,d0
	add.l      d6,d0
	cmp.l      d0,d2
	bge.s      x13c56
	move.w     (a2),d1
	add.w      (a5),d1
	ext.l      d1
	cmp.l      d0,d1
	ble.s      x13c56
	move.l     d2,d6
	sub.l      d7,d6
x13c56:
	lea.l      9(a3),a3
	bra        x13cde
x13c5e:
	lea.l      10(a7),a1
	lea.l      2(a3),a0
	bsr        x1505e
	movea.l    a0,a3
	movea.l    a2,a1
	bsr        dec_255_decode
	movea.l    a0,a3
	lea.l      8(a7),a1
	bsr        x1505e
	movea.l    a0,a3
	movea.l    a5,a1
	bsr        x1505e
	movea.l    a0,a3
	lea.l      4(a7),a1
	bsr        x1505e
	movea.l    a0,a3
	addq.w     #1,(a5)
	move.w     (a5),d0
	ext.l      d0
	cmp.l      d0,d6
	blt.s      x13cde
	move.w     (a2),d1
	ext.l      d1
	cmp.l      d1,d7
	bge.s      x13cde
	move.l     d7,d2
	add.l      d6,d2
	cmp.l      d2,d1
	bge.s      x13cde
	move.w     (a2),d0
	add.w      (a5),d0
	ext.l      d0
	cmp.l      d2,d0
	ble.s      x13cde
	move.l     d1,d6
	sub.l      d7,d6
	bra.s      x13cde
x13cba:
	move.b     2(a3),d0
	ext.w      d0
	ext.l      d0
	adda.l     d0,a3
	bra.s      x13cde
x13cc6:
	addq.w     #2,a3
	movea.l    a3,a0
	bsr        strlen
	addq.l     #1,d0
	adda.l     d0,a3
	bra.s      x13cde
x13cd4:
	lea.l      10(a3),a3
	bra.s      x13cde
x13cda:
	movea.l    16(a7),a3
x13cde:
	cmpa.l     16(a7),a3
	bcc.s      x13cec
	cmpi.b     #$1B,(a3)
	beq        x13a76
x13cec:
	move.l     d6,d0
	lea.l      28(a7),a7
	movem.l    (a7)+,d3-d7/a2-a6
	rts

	.globl x13cf8
x13cf8:
	movem.l    d3-d6/a2-a6,-(a7)
	lea.l      -42(a7),a7
	movea.l    a0,a2
	move.l     a1,28(a7)
	move.l     d0,d4
	move.l     d1,d3
	move.l     d2,d5
	move.l     20(a2),24(a7)
	movea.l    (a0),a4
	movea.l    4(a1),a3
	lea.l      14(a3),a3
	movea.l    4(a1),a0
	move.l     6(a0),d0
	lea.l      0(a3,d0.l),a1
	move.l     a1,20(a7)
	movea.l    28(a7),a0
	movea.l    (a0),a1
	cmpi.l     #$48444F43,262(a1)
	bne        x143ca
	lea.l      12(a7),a5
	bra        x143bc
x13d46:
	move.b     1(a3),d0
	ext.w      d0
	sub.w      #$0023,d0
	cmp.w      #$0012,d0
	bhi        x143b8
	add.w      d0,d0
	move.w     x13d62(pc,d0.w),d0
	jmp        x13d62(pc,d0.w)
x13d62:
	.dc.w x143a4-x13d62
	.dc.w x143b8-x13d62
	.dc.w x143b8-x13d62
	.dc.w x143b8-x13d62
	.dc.w x143b8-x13d62
	.dc.w x14398-x13d62
	.dc.w x14398-x13d62
	.dc.w x14398-x13d62
	.dc.w x14398-x13d62
	.dc.w x14398-x13d62
	.dc.w x14398-x13d62
	.dc.w x14398-x13d62
	.dc.w x14398-x13d62
	.dc.w x14398-x13d62
	.dc.w x143b2-x13d62
	.dc.w x13d88-x13d62
	.dc.w x14066-x13d62
	.dc.w x1414e-x13d62
	.dc.w x142a4-x13d62
x13d88:
	lea.l      36(a7),a1
	lea.l      2(a3),a0
	bsr        dec_255_decode
	movea.l    28(a7),a0
	movea.l    4(a0),a1
	move.l     (a1),32(a7)
	bra.s      x13daa
x13da2:
	movea.l    32(a7),a0
	move.l     (a0),32(a7)
x13daa:
	move.l     32(a7),d0
	beq.s      x13dbc
	movea.l    d0,a0
	move.w     4(a0),d1
	cmp.w      36(a7),d1
	bne.s      x13da2
x13dbc:
	move.l     32(a7),d0
	bne.s      x13e0c
	move.w     #1,4(a7)
	move.w     #1,2(a7)
	moveq.l    #2,d1
	movea.l    28(a7),a0
	movea.l    (a0),a1
	and.w      (a1),d1
	beq.s      x13df6
	move.w     4(a7),d2
	muls.w     12(a2),d2
	move.w     d2,4(a5)
	move.w     2(a7),d0
	muls.w     14(a2),d0
	move.w     d0,6(a5)
	bra.w      x13e74
x13df6:
	move.w     4(a7),d0
	lsl.w      #3,d0
	move.w     d0,4(a5)
	move.w     2(a7),d1
	lsl.w      #4,d1
	move.w     d1,6(a5)
	bra.s      x13e74
x13e0c:
	movea.l    32(a7),a0
	lea.l      14(a0),a0
	move.l     a0,38(a7)
	moveq.l    #2,d0
	movea.l    28(a7),a1
	movea.l    (a1),a0
	and.w      (a0),d0
	beq.s      x13e66
	movea.l    38(a7),a1
	move.w     (a1),d1
	ext.l      d1
	divs.w     #8,d1
	muls.w     12(a2),d1
	move.w     (a1),d2
	ext.l      d2
	divs.w     #8,d2
	swap       d2
	add.w      d2,d1
	move.w     d1,4(a5)
	move.w     2(a1),d0
	ext.l      d0
	divs.w     #16,d0
	muls.w     14(a2),d0
	move.w     2(a1),d1
	ext.l      d1
	divs.w     #16,d1
	swap       d1
	add.w      d1,d0
	move.w     d0,6(a5)
	bra.s      x13e74
x13e66:
	movea.l    38(a7),a0
	move.w     (a0),4(a5)
	move.w     2(a0),6(a5)
x13e74:
	move.w     4(a5),d0
	add.w      12(a2),d0
	subq.w     #1,d0
	ext.l      d0
	divs.w     12(a2),d0
	move.w     d0,4(a7)
	move.w     6(a5),d1
	add.w      14(a2),d1
	subq.w     #1,d1
	ext.l      d1
	divs.w     14(a2),d1
	move.w     d1,2(a7)
	lea.l      6(a7),a1
	lea.l      4(a3),a0
	bsr        x1505e
	move.w     6(a7),d0
	bpl.s      x13eea
	movea.l    28(a7),a0
	movea.l    (a0),a1
	move.w     306(a1),d6
	muls.w     12(a2),d6
	sub.w      4(a5),d6
	ext.l      d6
	divs.w     #2,d6
	ext.l      d6
	tst.l      d6
	bpl.s      x13ece
	moveq.l    #0,d6
x13ece:
	move.w     4(a2),d0
	add.w      d6,d0
	move.w     d0,-(a7)
	move.l     d3,d0
	move.w     12(a2),d1
	ext.l      d1
	bsr        _lmul
	move.w     (a7)+,d1
	sub.w      d0,d1
	move.w     d1,(a5)
	bra.s      x13f02
x13eea:
	move.w     6(a7),d0
	ext.l      d0
	sub.l      d3,d0
	move.w     12(a2),d1
	ext.l      d1
	bsr        _lmul
	add.w      4(a2),d0
	move.w     d0,(a5)
x13f02:
	lea.l      8(a7),a1
	lea.l      5(a3),a0
	bsr        dec_255_decode
	move.w     8(a7),d0
	ext.l      d0
	sub.l      d4,d0
	move.w     14(a2),d1
	ext.l      d1
	bsr        _lmul
	add.w      6(a2),d0
	move.w     d0,2(a5)
	move.w     8(a7),d1
	ext.l      d1
	move.l     d4,d2
	add.l      d5,d2
	cmp.l      d2,d1
	bge        x13ffa
	move.w     8(a7),d6
	add.w      2(a7),d6
	ext.l      d6
	cmp.l      d6,d4
	bgt        x13ffa
	move.l     32(a7),d0
	bne.s      x13f6c
	moveq.l    #1,d1
	move.w     d1,-(a7)
	movea.l    a4,a0
	move.w     d1,d0
	moveq.l    #7,d1
	move.w     d0,d2
	bsr        x172de
	addq.w     #2,a7
	movea.l    a5,a1
	movea.l    a4,a0
	bsr        x17482
	bra        x13ffa
x13f6c:
	movea.l    38(a7),a0
	cmpi.b     #$01,4(a0)
	bne.s      x13fa8
	clr.w      -(a7)
	moveq.l    #1,d0
	move.w     d0,-(a7)
	moveq.l    #7,d1
	move.w     d1,-(a7)
	move.w     d0,-(a7)
	move.w     2(a0),-(a7)
	move.w     (a0),-(a7)
	move.w     6(a5),-(a7)
	move.w     4(a5),d2
	movea.l    a0,a1
	addq.w     #8,a1
	movea.l    a4,a0
	move.w     (a5),d0
	move.w     2(a5),d1
	bsr        x17914
	lea.l      14(a7),a7
	bra.s      x13ffa
x13fa8:
	movea.l    32(a7),a0
	move.l     10(a0),d0
	beq.s      x13fb6
	movea.l    d0,a1
	bra.s      x13fc0
x13fb6:
	movea.l    28(a7),a0
	movea.l    (a0),a1
	movea.l    298(a1),a1
x13fc0:
	move.l     a1,-(a7)
	clr.w      -(a7)
	moveq.l    #1,d0
	move.w     d0,-(a7)
	moveq.l    #7,d1
	move.w     d1,-(a7)
	movea.l    48(a7),a0
	move.b     4(a0),d2
	ext.w      d2
	move.w     d2,-(a7)
	move.w     2(a0),-(a7)
	move.w     (a0),-(a7)
	move.w     6(a5),-(a7)
	movea.l    a0,a1
	addq.w     #8,a1
	movea.l    a4,a0
	move.w     (a5),d0
	move.w     2(a5),d1
	move.w     4(a5),d2
	bsr        x17fb8
	lea.l      18(a7),a7
x13ffa:
	lea.l      9(a3),a3
	cmpi.b     #$1B,(a3)
	bne        x143bc
	cmpi.b     #$28,1(a3)
	bne        x143bc
	movea.l    24(a7),a0
	move.w     10(a0),d0
	beq        x143bc
	lea.l      (a7),a1
	lea.l      4(a3),a0
	bsr        dec_255_decode
	moveq.l    #-2,d1
	moveq.l    #-2,d0
	movea.l    a5,a0
	bsr        x18436
	pea.l      32(a7)
	lea.l      38(a7),a1
	move.w     4(a7),d0
	movea.l    32(a7),a0
	movea.l    (a0),a0
	movea.l    28(a7),a6
	movea.l    20(a6),a6
	jsr        (a6)
	addq.w     #4,a7
	moveq.l    #1,d0
	move.w     d0,-(a7)
	move.w     d0,d2
	moveq.l    #7,d1
	movea.l    a4,a0
	move.w     36(a7),d0
	bsr        x172de
	addq.w     #2,a7
	bra        x14298
x14066:
	lea.l      6(a7),a1
	lea.l      2(a3),a0
	bsr        x1505e
	movea.l    a0,a3
	lea.l      8(a7),a1
	bsr        dec_255_decode
	movea.l    a0,a3
	lea.l      4(a7),a1
	bsr        x1505e
	movea.l    a0,a3
	lea.l      2(a7),a1
	bsr        x1505e
	movea.l    a0,a3
	lea.l      10(a7),a1
	bsr        x1505e
	movea.l    a0,a3
	move.w     6(a7),d0
	sub.w      d3,d0
	move.w     d0,(a5)
	move.w     8(a7),d1
	sub.w      d4,d1
	move.w     d1,2(a5)
	moveq.l    #-127,d2
	add.w      4(a7),d2
	move.w     d2,4(a5)
	move.w     2(a7),6(a5)
	move.w     (a5),d6
	muls.w     12(a2),d6
	move.w     d6,(a5)
	move.w     2(a5),d0
	muls.w     14(a2),d0
	move.w     d0,2(a5)
	move.w     4(a5),d6
	muls.w     12(a2),d6
	move.w     d6,4(a5)
	move.w     6(a5),d0
	muls.w     14(a2),d0
	move.w     d0,6(a5)
	move.w     4(a2),d6
	add.w      d6,(a5)
	move.w     6(a2),d0
	add.w      d0,2(a5)
	move.w     (a5),32(a7)
	move.w     2(a5),34(a7)
	move.w     (a5),d1
	add.w      4(a5),d1
	move.w     d1,36(a7)
	move.w     2(a5),d2
	add.w      6(a5),d2
	move.w     d2,38(a7)
	moveq.l    #3,d6
	and.w      10(a7),d6
	move.w     10(a7),d0
	asr.w      #3,d0
	move.w     d0,10(a7)
	addq.w     #1,10(a7)
	moveq.l    #1,d0
	move.w     d0,-(a7)
	movea.l    a4,a0
	moveq.l    #7,d1
	move.w     12(a7),d2
	bsr        x172de
	addq.w     #2,a7
	move.w     d6,d0
	lea.l      32(a7),a1
	movea.l    a4,a0
	bsr        x173da
	bra        x143bc
x1414e:
	lea.l      6(a7),a1
	lea.l      2(a3),a0
	bsr        x1505e
	movea.l    a0,a3
	lea.l      8(a7),a1
	bsr        dec_255_decode
	movea.l    a0,a3
	lea.l      4(a7),a1
	bsr        x1505e
	movea.l    a0,a3
	lea.l      2(a7),a1
	bsr        x1505e
	movea.l    a0,a3
	lea.l      10(a7),a1
	bsr        x1505e
	movea.l    a0,a3
	move.w     6(a7),(a5)
	move.w     8(a7),d0
	sub.w      d4,d0
	move.w     d0,2(a5)
	moveq.l    #1,d1
	add.w      4(a7),d1
	move.w     d1,4(a5)
	moveq.l    #1,d2
	add.w      2(a7),d2
	move.w     d2,6(a5)
	move.w     2(a5),d6
	add.w      d2,d6
	bmi        x143bc
	subq.w     #1,d2
	bne.s      x14210
	pea.l      34(a7)
	move.w     (a5),d1
	ext.l      d1
	move.w     12(a7),d0
	ext.l      d0
	movea.l    a2,a1
	movea.l    32(a7),a0
	bsr        x136da
	addq.w     #4,a7
	pea.l      32(a7)
	move.w     (a5),d1
	add.w      4(a5),d1
	ext.l      d1
	move.w     12(a7),d0
	ext.l      d0
	movea.l    a2,a1
	movea.l    32(a7),a0
	bsr        x136da
	addq.w     #4,a7
	move.w     34(a7),(a5)
	move.w     32(a7),d0
	sub.w      34(a7),d0
	move.w     d0,4(a5)
	move.w     2(a5),d1
	muls.w     14(a2),d1
	move.w     d1,2(a5)
	move.w     14(a2),6(a5)
	bra.s      x1423c
x14210:
	move.w     (a5),d0
	muls.w     12(a2),d0
	move.w     d0,(a5)
	move.w     2(a5),d1
	muls.w     14(a2),d1
	move.w     d1,2(a5)
	move.w     4(a5),d0
	muls.w     12(a2),d0
	move.w     d0,4(a5)
	move.w     6(a5),d1
	muls.w     14(a2),d1
	move.w     d1,6(a5)
x1423c:
	move.w     4(a2),d0
	move.w     d3,d1
	muls.w     12(a2),d1
	sub.w      d1,d0
	add.w      d0,(a5)
	move.w     6(a2),d2
	add.w      d2,2(a5)
	cmpi.w     #7,10(a7)
	blt.s      x1426e
	cmpi.w     #7,10(a7)
	bne.s      x14266
	moveq.l    #8,d6
	bra.s      x14268
x14266:
	moveq.l    #7,d6
x14268:
	move.w     d6,10(a7)
	bra.s      x14272
x1426e:
	tst.w      10(a7)
x14272:
	move.w     10(a7),d2
	moveq.l    #7,d1
	moveq.l    #1,d0
	movea.l    a4,a0
	bsr        x17574
	movea.l    a5,a1
	movea.l    a4,a0
	bsr        x175b2
	moveq.l    #1,d0
	move.w     d0,-(a7)
	move.w     d0,d2
	moveq.l    #7,d1
	movea.l    a4,a0
	bsr        x172de
	addq.w     #2,a7
x14298:
	movea.l    a5,a1
	movea.l    a4,a0
	bsr        x17482
	bra        x143bc
x142a4:
	lea.l      6(a7),a1
	lea.l      2(a3),a0
	bsr        x1505e
	movea.l    a0,a3
	lea.l      8(a7),a1
	bsr        dec_255_decode
	movea.l    a0,a3
	lea.l      4(a7),a1
	bsr        x1505e
	movea.l    a0,a3
	lea.l      2(a7),a1
	bsr        x1505e
	movea.l    a0,a3
	lea.l      10(a7),a1
	bsr        x1505e
	movea.l    a0,a3
	move.w     6(a7),d0
	sub.w      d3,d0
	move.w     d0,(a5)
	move.w     8(a7),d1
	sub.w      d4,d1
	move.w     d1,2(a5)
	moveq.l    #1,d2
	add.w      4(a7),d2
	move.w     d2,4(a5)
	moveq.l    #1,d6
	add.w      2(a7),d6
	move.w     d6,6(a5)
	move.w     2(a5),d0
	add.w      d6,d0
	bmi        x143bc
	move.w     (a5),d1
	muls.w     12(a2),d1
	move.w     d1,(a5)
	move.w     2(a5),d0
	muls.w     14(a2),d0
	move.w     d0,2(a5)
	move.w     4(a5),d1
	muls.w     12(a2),d1
	move.w     d1,4(a5)
	move.w     6(a5),d0
	muls.w     14(a2),d0
	move.w     d0,6(a5)
	move.w     4(a2),d1
	add.w      d1,(a5)
	move.w     6(a2),d0
	add.w      d0,2(a5)
	cmpi.w     #7,10(a7)
	blt.s      x14360
	cmpi.w     #7,10(a7)
	bne.s      x14358
	moveq.l    #8,d6
	bra.s      x1435a
x14358:
	moveq.l    #7,d6
x1435a:
	move.w     d6,10(a7)
	bra.s      x14364
x14360:
	tst.w      10(a7)
x14364:
	move.w     10(a7),d2
	moveq.l    #7,d1
	moveq.l    #1,d0
	movea.l    a4,a0
	bsr        x17574
	moveq.l    #10,d0
	movea.l    a5,a1
	movea.l    a4,a0
	bsr        x17630
	moveq.l    #1,d0
	move.w     d0,-(a7)
	move.w     d0,d2
	moveq.l    #7,d1
	movea.l    a4,a0
	bsr        x172de
	addq.w     #2,a7
	moveq.l    #10,d0
	movea.l    a5,a1
	movea.l    a4,a0
	bsr        x174d6
	bra.s      x143bc
x14398:
	move.b     2(a3),d0
	ext.w      d0
	ext.l      d0
	adda.l     d0,a3
	bra.s      x143bc
x143a4:
	addq.w     #2,a3
	movea.l    a3,a0
	bsr        strlen
	addq.l     #1,d0
	adda.l     d0,a3
	bra.s      x143bc
x143b2:
	lea.l      10(a3),a3
	bra.s      x143bc
x143b8:
	movea.l    20(a7),a3
x143bc:
	cmpa.l     20(a7),a3
	bcc.s      x143ca
	cmpi.b     #$1B,(a3)
	beq        x13d46
x143ca:
	lea.l      42(a7),a7
	movem.l    (a7)+,d3-d6/a2-a6
	rts

	.globl x143d4
x143d4:
	movem.l    d3-d4/a2-a6,-(a7)
	lea.l      -86(a7),a7
	move.l     a0,82(a7)
	movea.l    a1,a4
	movea.l    118(a7),a2
	move.w     d0,d4
	movea.l    a0,a3
	bra        x145f6
x143ee:
	cmpi.b     #$40,(a4)
	beq.s      x143fa
	move.b     (a4)+,(a3)+
	bra        x145f6
x143fa:
	movea.l    a4,a5
	addq.w     #1,a4
x143fe:
	move.b     (a4),d0
	beq.s      x14408
	cmpi.b     #$40,(a4)+
	bne.s      x143fe
x14408:
	move.l     a4,d0
	sub.l      a5,d0
	moveq.l    #2,d1
	cmp.l      d0,d1
	blt.s      x1441a
	move.b     #$40,(a3)+
	bra        x145f6
x1441a:
	moveq.l    #11,d3
	bra.s      x1443c
x1441e:
	move.l     a4,d0
	sub.l      a5,d0
	movea.l    a5,a1
	move.w     d3,d1
	lsl.w      #2,d1
	lea.l      x1915e,a0
	movea.l    0(a0,d1.w),a0
	bsr        strnicmp
	tst.w      d0
	beq.s      x14440
	subq.w     #1,d3
x1443c:
	tst.w      d3
	bpl.s      x1441e
x14440:
	move.w     d3,d0
	cmp.w      #11,d0
	bhi        x145e8
	add.w      d0,d0
	move.w     x14454(pc,d0.w),d0
	jmp        x14454(pc,d0.w)
x14454:
	.dc.w x14484-x14454
	.dc.w x14496-x14454
	.dc.w x144a8-x14454
	.dc.w x144ba-x14454
	.dc.w x144f6-x14454
	.dc.w x1450c-x14454
	.dc.w x14528-x14454
	.dc.w x14544-x14454
	.dc.w x145a4-x14454
	.dc.w x145b8-x14454
	.dc.w x1446c-x14454
	.dc.w x144e0-x14454
x1446c:
	move.b     x1a076,d0
	beq.s      x14484
	lea.l      x1a076,a1
	movea.l    a3,a0
	bsr        strcpy
	bra.w      x14500
x14484:
	moveq.l    #1,d0
	movea.l    (a2),a0
	bsr        hyp_find_extheader
	movea.l    a0,a6
	move.l     a6,d0
	bne.s      x144ca
	bra        x145f6
x14496:
	moveq.l    #8,d0
	movea.l    (a2),a0
	bsr        hyp_find_extheader
	movea.l    a0,a6
	move.l     a6,d0
	bne.s      x144ca
	bra        x145f6
x144a8:
	moveq.l    #6,d0
	movea.l    (a2),a0
	bsr        hyp_find_extheader
	movea.l    a0,a6
	move.l     a6,d0
	bne.s      x144ca
	bra        x145f6
x144ba:
	moveq.l    #5,d0
	movea.l    (a2),a0
	bsr        hyp_find_extheader
	movea.l    a0,a6
	move.l     a6,d0
	beq        x145f6
x144ca:
	movea.l    a6,a1
	movea.l    a3,a0
	bsr        strcpy
	movea.l    a3,a0
	movea.l    (a2),a1
	move.b     273(a1),d0
	bsr        conv_nodename
	bra.s      x14500
x144e0:
	move.b     x1a0f6,d0
	beq.s      x144f6
	lea.l      x1a0f6,a1
	movea.l    a3,a0
	bsr        strcpy
	bra.s      x14500
x144f6:
	movea.l    18(a2),a1
	movea.l    a3,a0
	bsr        strcpy
x14500:
	movea.l    a3,a0
	bsr        strlen
	adda.l     d0,a3
	bra        x145f6
x1450c:
	movea.l    4(a2),a0
	move.w     4(a0),d0
	ext.l      d0
	lsl.l      #2,d0
	movea.l    (a2),a1
	movea.l    278(a1),a5
	movea.l    0(a5,d0.l),a0
	move.w     10(a0),d3
	bra.s      x1455e
x14528:
	movea.l    4(a2),a0
	move.w     4(a0),d0
	ext.l      d0
	lsl.l      #2,d0
	movea.l    (a2),a1
	movea.l    278(a1),a5
	movea.l    0(a5,d0.l),a0
	move.w     8(a0),d3
	bra.s      x1455e
x14544:
	movea.l    4(a2),a0
	move.w     4(a0),d0
	ext.l      d0
	lsl.l      #2,d0
	movea.l    (a2),a1
	movea.l    278(a1),a5
	movea.l    0(a5,d0.l),a0
	move.w     12(a0),d3
x1455e:
	tst.w      d3
	bmi        x145f6
	movea.l    4(a2),a0
	cmp.w      4(a0),d3
	beq        x145f6
	clr.w      d2
	moveq.l    #16,d1
	moveq.l    #8,d0
	movea.l    (a2),a1
	lea.l      (a7),a0
	bsr        hyp_init_pageinfo
	move.w     d3,d0
	lea.l      (a7),a0
	bsr        hyp_get_window_title
	movea.l    a0,a6
	move.l     a6,d0
	beq.s      x1459c
	movea.l    a6,a1
	movea.l    a3,a0
	bsr        strcpy
	movea.l    a3,a0
	bsr        strlen
	adda.l     d0,a3
x1459c:
	lea.l      (a7),a0
	bsr        hyp_free_pageinfo
	bra.s      x145f6
x145a4:
	move.w     d4,-(a7)
	lea.l      x191fd,a1
	movea.l    a3,a0
	bsr        sprintf
	addq.w     #2,a7
	adda.w     d0,a3
	bra.s      x145f6
x145b8:
	pea.l      76(a7)
	lea.l      82(a7),a1
	lea.l      84(a7),a0
	bsr        x186e4
	addq.w     #4,a7
	move.w     76(a7),-(a7)
	move.w     80(a7),-(a7)
	move.w     84(a7),-(a7)
	lea.l      x19200,a1
	movea.l    a3,a0
	bsr        sprintf
	addq.w     #6,a7
	adda.w     d0,a3
	bra.s      x145f6
x145e8:
	lea.l      x19209,a1
	movea.l    a3,a0
	bsr        sprintf
	adda.w     d0,a3
x145f6:
	move.b     (a4),d0
	bne        x143ee
	clr.b      (a3)
	movea.l    82(a7),a0
	lea.l      86(a7),a7
	movem.l    (a7)+,d3-d4/a2-a6
	rts

	.globl x1460c
x1460c:
	movem.l    d3-d6/a2-a4,-(a7)
	movea.l    a0,a3
	lea.l      x1a076,a2
	clr.b      (a2)
	clr.b      128(a2)
	moveq.l    #0,d3
	move.w     layout+52,d0 /* skip_udo_header */
	beq        x14720
	move.w     12(a3),d1
	ble        x14720
	movea.l    8(a3),a0
	movea.l    (a0),a4
	movea.l    a4,a0
	bsr        strlen
	cmpi.b     #$1B,(a4)
	bne        x14720
	cmpi.b     #$6C,1(a4)
	bne        x14720
	cmpi.b     #$1B,-2(a4,d0.w)
	bne        x14720
	cmpi.b     #$64,-1(a4,d0.w)
	bne        x14720
	moveq.l    #2,d3
	moveq.l    #0,d0
	movea.l    a3,a0
	bsr        hyp_get_linewidth
	move.l     d0,d4
	moveq.l    #-1,d5
	add.w      d4,d5
	bra        x1471a
x14678:
	move.w     d5,d1
	ext.l      d1
	moveq.l    #0,d0
	movea.l    a3,a0
	bsr        x1665a
	cmp.b      #$20,d0
	bne        x14718
	moveq.l    #-1,d1
	add.w      d5,d1
	ext.l      d1
	moveq.l    #0,d0
	movea.l    a3,a0
	bsr        x1665a
	cmp.b      #$20,d0
	beq.s      x146c2
	cmp.b      #$2E,d0
	bne.w      x14718
	cmp.w      #1,d5
	ble.s      x14718
	moveq.l    #-2,d1
	add.w      d5,d1
	ext.l      d1
	moveq.l    #0,d0
	movea.l    a3,a0
	bsr        x1665a
	cmp.b      #$2E,d0
	bne.s      x14718
x146c2:
	movea.l    a2,a4
	moveq.l    #1,d6
	add.w      d5,d6
	bra.s      x146da
x146ca:
	move.w     d6,d1
	ext.l      d1
	moveq.l    #0,d0
	movea.l    a3,a0
	bsr        x1665a
	move.b     d0,(a4)+
	addq.w     #1,d6
x146da:
	cmp.w      d6,d4
	bgt.s      x146ca
	clr.b      (a4)+
x146e0:
	subq.w     #1,d5
	tst.w      d5
	bmi.s      x146f8
	move.w     d5,d1
	ext.l      d1
	moveq.l    #0,d0
	movea.l    a3,a0
	bsr        x1665a
	cmp.b      #$20,d0
	beq.s      x146e0
x146f8:
	lea.l      128(a2),a4
	clr.w      d6
	bra.s      x14710
x14700:
	move.w     d6,d1
	ext.l      d1
	moveq.l    #0,d0
	movea.l    a3,a0
	bsr        x1665a
	move.b     d0,(a4)+
	addq.w     #1,d6
x14710:
	cmp.w      d6,d5
	bge.s      x14700
	clr.b      (a4)+
	bra.s      x14720
x14718:
	subq.w     #1,d5
x1471a:
	tst.w      d5
	bgt        x14678
x14720:
	move.l     d3,d0
	movem.l    (a7)+,d3-d6/a2-a4
	rts

	.globl x14728
x14728:
	movem.l    d3/a2-a4,-(a7)
	subq.w     #4,a7
	movea.l    a0,a3
	movea.l    a1,a2
	move.w     d0,d3
	movea.l    (a0),a0
	bsr        x1726c
	movea.l    a2,a0
	bsr        x1814c
	lea.l      8(a2),a0
	move.l     a0,-(a7)
	movea.l    (a3),a0
	bsr        x1726c
	movea.l    (a7)+,a0
	bsr        x1811c
	lea.l      (a7),a1
	lea.l      2(a7),a0
	move.l     a1,-(a7)
	move.l     a0,-(a7)
	movea.l    (a3),a0
	bsr        x1726c
	movea.l    (a7)+,a0
	movea.l    (a7)+,a1
	bsr        x181b8
	lea.l      layout,a4
	move.l     2(a4),d0
	move.w     2(a7),d1
	ext.l      d1
	bsr        _lmul
	move.l     #25400,d1
	bsr        _ldiv
	move.w     d0,16(a2)
	move.l     6(a4),d0
	move.w     (a7),d1
	ext.l      d1
	bsr        _lmul
	move.l     #25400,d1
	bsr        _ldiv
	move.w     d0,18(a2)
	move.l     10(a4),d0
	move.w     2(a7),d1
	ext.l      d1
	bsr        _lmul
	move.l     #25400,d1
	bsr        _ldiv
	move.w     d0,20(a2)
	move.l     14(a4),d0
	move.w     (a7),d1
	ext.l      d1
	bsr        _lmul
	move.l     #25400,d1
	bsr        _ldiv
	move.w     d0,22(a2)
	move.w     (a2),d1
	add.w      16(a2),d1
	move.w     d1,24(a2)
	move.w     2(a2),d0
	add.w      18(a2),d0
	move.w     d0,26(a2)
	move.w     16(a2),d1
	add.w      20(a2),d1
	move.w     4(a2),d2
	sub.w      d1,d2
	move.w     d2,28(a2)
	move.w     18(a2),d0
	add.w      22(a2),d0
	move.w     6(a2),d1
	sub.w      d0,d1
	move.w     d1,30(a2)
	lea.l      24(a2),a1
	lea.l      8(a2),a0
	bsr        x183b8
	lea.l      24(a2),a0
	lea.l      32(a2),a1
	move.l     (a0)+,(a1)+
	move.l     (a0)+,(a1)+
	move.w     18(a4),d0
	beq.s      x14840
	move.w     d3,d1
	add.w      d1,d1
	add.w      d1,34(a2)
	sub.w      d1,38(a2)
x14840:
	move.w     34(a4),d0
	beq.s      x1484e
	move.w     d3,d1
	add.w      d1,d1
	sub.w      d1,38(a2)
x1484e:
	lea.l      32(a2),a0
	lea.l      4(a3),a1
	move.l     (a0)+,(a1)+
	move.l     (a0)+,(a1)+
	addq.w     #4,a7
	movem.l    (a7)+,d3/a2-a4
	rts

	.globl x14862
x14862:
	move.l     a2,-(a7)
	move.l     a3,-(a7)
	movea.l    a0,a3
	movea.l    a1,a2
	move.w     layout+50,d1 /* swap_layout */
	beq.s      x148b6
	tst.w      d0
	beq.s      x1487c
	move.w     20(a2),d2
	bra.s      x14880
x1487c:
	move.w     16(a2),d2
x14880:
	add.w      (a2),d2
	move.w     d2,24(a2)
	move.w     16(a2),d0
	add.w      20(a2),d0
	move.w     4(a2),d1
	sub.w      d0,d1
	move.w     d1,28(a2)
	lea.l      24(a2),a1
	lea.l      8(a2),a0
	bsr        x183b8
	move.w     24(a2),32(a2)
	lea.l      32(a2),a0
	lea.l      4(a3),a1
	move.l     (a0)+,(a1)+
	move.l     (a0)+,(a1)+
x148b6:
	movea.l    (a7)+,a3
	movea.l    (a7)+,a2
	rts

	.globl x148bc
x148bc:
	movem.l    d3-d5/a2-a5,-(a7)
	lea.l      -400(a7),a7
	move.l     a1,396(a7)
	move.w     d0,d5
	move.w     d1,d4
	movea.l    432(a7),a5
	movea.l    (a0),a2
	move.l     20(a0),392(a7)
	move.w     14(a0),d3
	movea.l    16(a0),a1
	move.w     72(a1),d2
	bne        x14a44
	move.w     layout+18,d1 /* add_head */
	beq        x14a44
	move.l     396(a7),-(a7)
	tst.w      d4
	beq.s      x14902
	movea.l    layout+28,a1 /* head_right_str */
	bra.s      x14908
x14902:
	movea.l    layout+20,a1 /* head_left_str */
x14908:
	lea.l      268(a7),a0
	move.w     d5,d0
	bsr        x143d4
	addq.w     #4,a7
	lea.l      136(a7),a4
	move.l     396(a7),-(a7)
	move.w     d5,d0
	movea.l    layout+24,a1 /* head_center_str */
	movea.l    a4,a0
	bsr        x143d4
	addq.w     #4,a7
	lea.l      8(a7),a3
	move.l     396(a7),-(a7)
	move.w     d5,d0
	move.w     d0,-(a7)
	tst.w      d4
	beq.s      x14944
	movea.l    layout+20,a1 /* head_left_str */
	bra.s      x1494a
x14944:
	movea.l    layout+28,a1 /* head_right_str */
x1494a:
	movea.l    a3,a0
	move.w     (a7)+,d0
	bsr        x143d4
	addq.w     #4,a7
	lea.l      24(a5),a1
	movea.l    a2,a0
	bsr        x17900
	move.w     26(a5),d4
	movea.l    392(a7),a0
	move.w     (a0),-(a7)
	clr.w      d2
	moveq.l    #3,d1
	move.w     6(a0),d0
	movea.l    a2,a0
	bsr        x176ae
	addq.w     #2,a7
	lea.l      264(a7),a0
	bsr        strlen
	move.l     d0,d2
	lea.l      264(a7),a1
	move.w     d4,d1
	move.w     24(a5),d0
	movea.l    a2,a0
	bsr        x1777e
	movea.l    a3,a0
	bsr        strlen
	movea.l    a3,a1
	movea.l    a2,a0
	bsr        x177fc
	move.w     d0,d5
	movea.l    a3,a0
	bsr        strlen
	move.l     d0,d2
	movea.l    a3,a1
	move.w     d4,d1
	move.w     28(a5),d0
	sub.w      d5,d0
	add.w      24(a5),d0
	movea.l    a2,a0
	bsr        x1777e
	movea.l    a4,a0
	bsr        strlen
	movea.l    a4,a1
	movea.l    a2,a0
	bsr        x177fc
	move.w     d0,d5
	movea.l    a4,a0
	bsr        strlen
	move.l     d0,d2
	movea.l    a4,a1
	move.w     d4,d1
	move.w     28(a5),d0
	sub.w      d5,d0
	ext.l      d0
	divs.w     #2,d0
	add.w      24(a5),d0
	movea.l    a2,a0
	bsr        x1777e
	add.w      d3,d4
	move.w     layout+32,d0 /* head_sep */
	beq.s      x14a44
	move.w     24(a5),(a7)
	move.w     d3,d1
	ext.l      d1
	divs.w     #2,d1
	add.w      d4,d1
	subq.w     #1,d1
	move.w     d1,2(a7)
	move.w     24(a5),d2
	add.w      28(a5),d2
	move.w     d2,4(a7)
	move.w     2(a7),6(a7)
	moveq.l    #1,d3
	move.w     d3,-(a7)
	movea.l    394(a7),a0
	move.w     6(a0),d0
	movea.l    a2,a0
	moveq.l    #7,d1
	move.w     layout+32,d2
	bsr        x172de
	addq.w     #2,a7
	lea.l      (a7),a1
	movea.l    a2,a0
	bsr        x173d2
x14a44:
	lea.l      400(a7),a7
	movem.l    (a7)+,d3-d5/a2-a5
	rts

	.globl x14a4e
x14a4e:
	movem.l    d3-d5/a2-a5,-(a7)
	lea.l      -400(a7),a7
	move.l     a1,396(a7)
	move.w     d0,d5
	move.w     d1,d4
	movea.l    432(a7),a5
	movea.l    (a0),a2
	move.l     20(a0),392(a7)
	move.w     14(a0),d3
	movea.l    16(a0),a1
	move.w     72(a1),d2
	bne        x14bda
	move.w     layout+34,d0 /* add_foot */
	beq        x14bda
	move.l     396(a7),-(a7)
	tst.w      d1
	beq.s      x14a94
	movea.l    layout+44,a1 /* foot_right_str */
	bra.s      x14a9a
x14a94:
	movea.l    layout+36,a1 /* foot_left_str */
x14a9a:
	lea.l      268(a7),a0
	move.w     d5,d0
	bsr        x143d4
	addq.w     #4,a7
	lea.l      136(a7),a4
	move.l     396(a7),-(a7)
	move.w     d5,d0
	movea.l    layout+40,a1 /* foot_center_str */
	movea.l    a4,a0
	bsr        x143d4
	addq.w     #4,a7
	lea.l      8(a7),a3
	move.l     396(a7),-(a7)
	move.w     d5,d0
	move.w     d0,-(a7)
	tst.w      d4
	beq.s      x14ad6
	movea.l    layout+36,a1
	bra.s      x14adc
x14ad6:
	movea.l    layout+44,a1
x14adc:
	movea.l    a3,a0
	move.w     (a7)+,d0
	bsr        x143d4
	addq.w     #4,a7
	lea.l      24(a5),a1
	movea.l    a2,a0
	bsr        x17900
	move.w     26(a5),d4
	add.w      30(a5),d4
	sub.w      d3,d4
	movea.l    392(a7),a0
	move.w     (a0),-(a7)
	clr.w      d2
	moveq.l    #3,d1
	move.w     6(a0),d0
	movea.l    a2,a0
	bsr        x176ae
	addq.w     #2,a7
	lea.l      264(a7),a0
	bsr        strlen
	move.l     d0,d2
	lea.l      264(a7),a1
	move.w     d4,d1
	move.w     24(a5),d0
	movea.l    a2,a0
	bsr        x1777e
	movea.l    a3,a0
	bsr        strlen
	movea.l    a3,a1
	movea.l    a2,a0
	bsr        x177fc
	move.w     d0,d5
	movea.l    a3,a0
	bsr        strlen
	move.l     d0,d2
	movea.l    a3,a1
	move.w     d4,d1
	move.w     28(a5),d0
	sub.w      d5,d0
	add.w      24(a5),d0
	movea.l    a2,a0
	bsr        x1777e
	movea.l    a4,a0
	bsr        strlen
	movea.l    a4,a1
	movea.l    a2,a0
	bsr        x177fc
	move.w     d0,d5
	movea.l    a4,a0
	bsr        strlen
	move.l     d0,d2
	movea.l    a4,a1
	move.w     d4,d1
	move.w     28(a5),d0
	sub.w      d5,d0
	ext.l      d0
	divs.w     #2,d0
	add.w      24(a5),d0
	movea.l    a2,a0
	bsr        x1777e
	move.w     layout+48,d0 /* foot_sep */
	beq.s      x14bda
	move.w     24(a5),(a7)
	move.w     d4,d1
	move.w     d3,d2
	ext.l      d2
	divs.w     #2,d2
	sub.w      d2,d1
	move.w     d1,2(a7)
	move.w     24(a5),d3
	add.w      28(a5),d3
	move.w     d3,4(a7)
	move.w     2(a7),6(a7)
	moveq.l    #1,d4
	move.w     d4,-(a7)
	movea.l    394(a7),a0
	move.w     6(a0),d0
	movea.l    a2,a0
	moveq.l    #7,d1
	move.w     layout+48,d2
	bsr        x172de
	addq.w     #2,a7
	lea.l      (a7),a1
	movea.l    a2,a0
	bsr        x173d2
x14bda:
	lea.l      400(a7),a7
	movem.l    (a7)+,d3-d5/a2-a5
	rts

	.globl x14be4
x14be4:
	movem.l    d3-d6/a2-a6,-(a7)
	lea.l      -12(a7),a7
	movea.l    a0,a2
	movea.l    a1,a3
	movea.l    52(a7),a6
	move.l     d0,d4
	movea.l    56(a7),a5
	move.l     (a0),(a7)
	movea.l    20(a2),a4
	move.w     38(a5),d3
	ext.l      d3
	divs.w     14(a2),d3
	ext.l      d3
	move.l     d3,d2
	moveq.l    #0,d1
	move.l     (a6),d0
	bsr        x13a2a
	move.l     d0,d3
	move.l     d4,d1
	sub.l      (a6),d1
	cmp.l      d1,d0
	ble.s      x14c22
	move.l     d1,d3
x14c22:
	movea.l    16(a2),a0
	move.w     72(a0),d0
	bne.s      x14c68
	lea.l      32(a5),a1
	lea.l      4(a7),a0
	move.l     (a1)+,(a0)+
	move.l     (a1)+,(a0)+
	move.l     d3,d0
	move.w     14(a2),d1
	ext.l      d1
	bsr        _lmul
	move.w     d0,10(a7)
	lea.l      4(a7),a1
	movea.l    (a7),a0
	bsr        x17900
	move.l     d3,d2
	moveq.l    #0,d1
	move.l     (a6),d0
	movea.l    a3,a1
	movea.l    a2,a0
	bsr        x13cf8
	bsr        should_abort
	tst.w      d0
	bne.s      x14cd8
x14c68:
	lea.l      32(a5),a1
	movea.l    (a7),a0
	bsr        x17900
	movea.l    (a3),a0
	cmpi.l     #$48444F43,262(a0)
	beq.s      x14c84
	move.w     12(a4),d0
	beq.s      x14c88
x14c84:
	move.w     (a4),d0
	bra.s      x14c8c
x14c88:
	move.w     2(a4),d0
x14c8c:
	move.w     d0,-(a7)
	clr.w      d2
	moveq.l    #7,d1
	move.w     6(a4),d0
	movea.l    2(a7),a0
	bsr        x176ae
	addq.w     #2,a7
	move.w     32(a5),d4
	move.w     34(a5),d5
	moveq.l    #0,d6
	bra.s      x14cd4
x14cac:
	bsr        should_abort
	tst.w      d0
	bne.s      x14cd8
	movea.l    16(a2),a0
	move.w     72(a0),d0
	bne.s      x14ccc
	move.w     d5,d2
	move.w     d4,d1
	move.l     (a6),d0
	movea.l    a3,a1
	movea.l    a2,a0
	bsr        x137c2
x14ccc:
	add.w      14(a2),d5
	addq.l     #1,(a6)
	addq.l     #1,d6
x14cd4:
	cmp.l      d6,d3
	bgt.s      x14cac
x14cd8:
	lea.l      12(a7),a7
	movem.l    (a7)+,d3-d6/a2-a6
	rts

	.globl x14ce2
x14ce2:
	move.l     a2,-(a7)
	move.l     a3,-(a7)
	movea.l    a1,a3
	movea.l    (a0),a2
	movea.l    16(a0),a1
	move.w     72(a1),d0
	bne.s      x14d3e
	move.w     layout+54,d1 /* show_borders */
	beq.s      x14d3e
	lea.l      8(a3),a1
	movea.l    a2,a0
	bsr        x17900
	moveq.l    #1,d0
	move.w     d0,-(a7)
	moveq.l    #3,d2
	moveq.l    #3,d1
	movea.l    a2,a0
	bsr        x172de
	addq.w     #2,a7
	moveq.l    #1,d0
	and.w      layout+54,d0
	beq.s      x14d2a
	lea.l      8(a3),a1
	movea.l    a2,a0
	bsr        x17482
x14d2a:
	moveq.l    #2,d0
	and.w      layout+54,d0
	beq.s      x14d3e
	lea.l      24(a3),a1
	movea.l    a2,a0
	bsr        x17482
x14d3e:
	movea.l    (a7)+,a3
	movea.l    (a7)+,a2
	rts

	.globl x14d44
x14d44:
	move.l     a2,-(a7)
	movea.l    a0,a2
	movea.l    16(a2),a1
	move.w     72(a1),d0
	bne.s      x14d6c
	move.w     x19d24,d1
	beq.s      x14d64
	movea.l    (a0),a0
	bsr        x1726c
	bsr        v_clrwk
x14d64:
	move.w     #1,x19d24
x14d6c:
	bsr        should_abort
	movea.l    (a2),a0
	bsr        x172a4
	moveq.l    #1,d0
	movea.l    (a7)+,a2
	rts

	.globl x14d7c
x14d7c:
	move.l     a2,-(a7)
	movea.l    a0,a2
	movea.l    (a0),a0
	bsr        x172c8
	movea.l    16(a2),a0
	move.w     72(a0),d0
	bne.s      x14dae
	bsr        should_abort
	tst.w      d0
	beq.s      x14da4
	movea.l    (a2),a0
	bsr        x1726c
	bsr        x182b0
	bra.s      x14dae
x14da4:
	movea.l    (a2),a0
	bsr        x1726c
	bsr        v_updwk
x14dae:
	bsr        should_abort
	movea.l    (a7)+,a2
	rts

	.globl x14db6
x14db6:
	movem.l    d3-d4/a2-a6,-(a7)
	lea.l      -72(a7),a7
	movea.l    a0,a6
	movea.l    a1,a2
	bsr        x17270
	move.l     a0,68(a7)
	move.l     a0,d0
	bne.s      x14dd4
	moveq.l    #2,d0
	bra        x14f2e
x14dd4:
	lea.l      44(a7),a3
	move.l     68(a7),(a3)
	move.l     104(a7),20(a3)
	lea.l      layout,a4
	move.l     a4,16(a3)
	movea.l    a3,a0
	bsr        x13332
	lea.l      4(a7),a5
	move.w     14(a3),d0
	movea.l    a5,a1
	movea.l    a3,a0
	bsr        x14728
	movea.l    a6,a0
	bsr        x1460c
	move.l     d0,(a7)
	move.l     64(a4),d1
	beq.s      x14e1a
	moveq.l    #-1,d2
	add.l      d1,d2
	cmp.l      d2,d0
	bge.s      x14e1a
	move.l     d2,(a7)
x14e1a:
	move.l     68(a4),d0
	beq.s      x14e24
	move.l     d0,d3
	bra.s      x14e2a
x14e24:
	move.w     12(a6),d3
	ext.l      d3
x14e2a:
	bra        x14f0e
x14e2e:
	addq.w     #1,(a2)
	move.w     (a2),d0
	cmp.w      58(a4),d0
	blt.s      x14e52
	cmp.w      60(a4),d0
	bgt.s      x14e52
	move.w     62(a4),d1
	beq.s      x14e56
	moveq.l    #1,d2
	and.w      d0,d2
	moveq.l    #1,d1
	and.w      62(a4),d1
	cmp.w      d1,d2
	beq.s      x14e56
x14e52:
	moveq.l    #1,d0
	bra.s      x14e58
x14e56:
	clr.w      d0
x14e58:
	movea.l    16(a3),a0
	move.w     d0,72(a0)
	move.l     18(a6),-(a7)
	move.w     (a2),-(a7)
	tst.w      d0
	beq.s      x14e72
	lea.l      x1921d,a0
	bra.s      x14e78
x14e72:
	lea.l      x19222,a0
x14e78:
	move.l     a0,-(a7)
	lea.l      x1920d,a0
	bsr        verboseout
	lea.l      10(a7),a7
	move.w     50(a4),d0
	beq.s      x14e98
	moveq.l    #1,d1
	and.w      (a2),d1
	bne.s      x14e98
	moveq.l    #1,d4
	bra.s      x14e9a
x14e98:
	clr.w      d4
x14e9a:
	move.w     d4,d0
	movea.l    a5,a1
	movea.l    a3,a0
	bsr        x14862
	movea.l    a3,a0
	bsr        x14d44
	bsr        should_abort
	tst.w      d0
	bne.s      x14f08
	movea.l    a5,a1
	movea.l    a3,a0
	bsr        x14ce2
	bsr        should_abort
	tst.w      d0
	bne.s      x14f08
	pea.l      (a5)
	move.w     d4,d1
	move.w     (a2),d0
	movea.l    a6,a1
	movea.l    a3,a0
	bsr        x148bc
	addq.w     #4,a7
	bsr        should_abort
	tst.w      d0
	bne.s      x14f08
	pea.l      (a5)
	move.w     d4,d1
	move.w     (a2),d0
	movea.l    a6,a1
	movea.l    a3,a0
	bsr        x14a4e
	addq.w     #4,a7
	bsr        should_abort
	tst.w      d0
	bne.s      x14f08
	pea.l      (a5)
	pea.l      4(a7)
	move.l     d3,d0
	movea.l    a6,a1
	movea.l    a3,a0
	bsr        x14be4
	addq.w     #8,a7
	bsr        should_abort
x14f08:
	movea.l    a3,a0
	bsr        x14d7c
x14f0e:
	cmp.l      (a7),d3
	ble.s      x14f24
	bsr        should_abort
	tst.w      d0
	bne.s      x14f24
	move.w     (a2),d0
	cmp.w      60(a4),d0
	blt        x14e2e
x14f24:
	movea.l    68(a7),a0
	bsr        x1729e
	clr.w      d0
x14f2e:
	lea.l      72(a7),a7
	movem.l    (a7)+,d3-d4/a2-a6
	rts

x14f38:
	move.l     a2,-(a7)
	move.l     a4,-(a7)
	lea.l      -512(a7),a7
	movea.l    a0,a2
	bsr        is_relative_path
	tst.w      d0
	beq.s      x14f64
	lea.l      (a7),a1
	movea.l    a2,a0
	bsr        get_basename
	move.l     a2,-(a7)
	lea.l      4(a7),a1
	lea.l      hypfold,a0
	bsr        append_path
	addq.w     #4,a7
x14f64:
	lea.l      x1922a,a1
	movea.l    a2,a0
	bsr        myfopen
	movea.l    a0,a4
	move.l     a4,d0
	bne.s      x14fb2
	lea.l      256(a7),a1
	movea.l    a2,a0
	bsr        get_dirname
	lea.l      (a7),a1
	movea.l    a2,a0
	bsr        get_basename
	lea.l      x1922d,a1
	lea.l      (a7),a0
	bsr        strcat
	move.l     a2,-(a7)
	lea.l      4(a7),a1
	lea.l      260(a7),a0
	bsr        append_path
	addq.w     #4,a7
	lea.l      x1922a,a1
	movea.l    a2,a0
	bsr        myfopen
	movea.l    a0,a4
x14fb2:
	movea.l    a4,a0
	lea.l      512(a7),a7
	movea.l    (a7)+,a4
	movea.l    (a7)+,a2
	rts

	.globl conv_nodename
conv_nodename:
	move.l     a2,-(a7)
	movea.l    a0,a2
	move.b     d0,d1
	ext.w      d1
	subq.w     #3,d1
	beq.s      x14fcc
	bra.s      x14fd8
x14fcc:
	movea.l    a2,a1
	movea.l    a2,a0
	bsr        conv_macroman
	ext.l      d0
	bra.s      x14fde
x14fd8:
	movea.l    a2,a0
	bsr        strlen
x14fde:
	movea.l    (a7)+,a2
	rts

	.globl hyp_find_pagename
hyp_find_pagename:
	movem.l    d3/a2-a3,-(a7)
	movea.l    a0,a3
	movea.l    a1,a2
	move.b     (a1),d0
	bne.s      x14ff2
	moveq.l    #-1,d0
	bra.s      x15034
x14ff2:
	clr.w      d3
	bra.s      x15018
x14ff6:
	movea.l    a2,a1
	move.w     d3,d0
	ext.l      d0
	lsl.l      #2,d0
	movea.l    278(a3),a0
	movea.l    0(a0,d0.l),a0
	lea.l      14(a0),a0
	bsr        strcmp
	tst.w      d0
	bne.s      x15016
	move.w     d3,d0
	bra.s      x15034
x15016:
	addq.w     #1,d3
x15018:
	cmp.w      270(a3),d3
	blt.s      x14ff6
	movea.l    a2,a1
	lea.l      x19232,a0
	bsr        strcmp
	tst.w      d0
	bne.s      x15032
	clr.w      d0
	bra.s      x15034
x15032:
	moveq.l    #-1,d0
x15034:
	movem.l    (a7)+,d3/a2-a3
	rts

	.globl hyp_find_extheader
hyp_find_extheader:
	movea.l    282(a0),a0
	move.l     a0,d1
	bne.s      x15056
	bra.s      x1505a
x15044:
	cmp.w      (a0),d0
	bne.s      x1504c
	addq.w     #4,a0
	rts
x1504c:
	move.w     2(a0),d1
	lea.l      4(a0,d1.w),a1
	movea.l    a1,a0
x15056:
	move.w     (a0),d1
	bne.s      x15044
x1505a:
	suba.l     a0,a0
	rts

	.globl x1505e
x1505e:
	clr.w      d0
	move.b     (a0)+,d0
	subq.w     #1,d0
	move.w     d0,(a1)
	rts

	.globl dec_255_decode
dec_255_decode:
	clr.w      d0
	move.b     (a0)+,d0
	subq.w     #1,d0
	clr.w      d2
	move.b     (a0)+,d2
	subq.w     #1,d2
	move.w     d2,d1
	lsl.w      #8,d1
	sub.w      d2,d1
	add.w      d1,d0
	move.w     d0,(a1)
	rts

	.globl dec_255_encode
dec_255_encode:
	move.w     d0,d1
	ext.l      d1
	divs.w     #$00FF,d1
	addq.w     #1,d1
	move.w     d0,d2
	ext.l      d2
	divs.w     #$00FF,d2
	swap       d2
	addq.w     #1,d2
	move.w     d2,d0
	move.b     d0,(a0)+
	move.b     d1,(a0)+
	rts

	.globl x1509e
x1509e:
	move.l     a2,-(a7)
	movea.l    a1,a2
	move.l     a2,d0
	beq.s      x150ca
	move.l     (a1),d1
	beq.s      x150ca
	movea.l    d1,a0
	clr.l      (a0)
	movea.l    (a1),a0
	clr.l      6(a0)
	movea.l    (a1),a0
	move.w     #$FFFF,4(a0)
	movea.l    (a1),a0
	clr.l      10(a0)
	movea.l    (a1),a0
	bsr        free
	clr.l      (a2)
x150ca:
	movea.l    (a7)+,a2
	rts

	.globl x150ce
x150ce:
	movem.l    a2-a4,-(a7)
	lea.l      14(a1),a2
	move.l     6(a1),d0
	lea.l      0(a2,d0.l),a3
	movea.l    a2,a4
	bra.s      x1511a
x150e2:
	cmpi.b     #$0D,(a2)
	bne.s      x1510c
	move.b     273(a0),d0
	bne.s      x15100
	cmpi.b     #$0A,1(a2)
	bne.s      x150fa
	moveq.l    #2,d1
	bra.s      x150fc
x150fa:
	moveq.l    #3,d1
x150fc:
	move.b     d1,273(a0)
x15100:
	cmpi.b     #$0A,1(a2)
	bne.s      x15112
	addq.w     #1,a2
	bra.s      x15112
x1510c:
	cmpi.b     #$0A,(a2)
	bne.s      x15118
x15112:
	clr.b      (a4)+
	addq.w     #1,a2
	bra.s      x1511a
x15118:
	move.b     (a2)+,(a4)+
x1511a:
	cmpa.l     a2,a3
	bhi.s      x150e2
	clr.b      (a4)
	movea.l    a4,a0
	lea.l      14(a1),a2
	suba.l     a2,a0
	move.l     a0,6(a1)
	movem.l    (a7)+,a2-a4
	rts

	.globl x15132
x15132:
	movem.l    d3-d6/a2-a6,-(a7)
	movea.l    a0,a4
	movea.l    a1,a6
	move.w     d0,d3
	suba.l     a2,a2
	movea.l    a2,a3
	lea.l      hyp_errno,a5
	tst.w      d3
	bmi.s      x15150
	cmp.w      270(a4),d3
	blt.s      x15158
x15150:
	move.w     #$07CF,(a5)
	bra        x152b0
x15158:
	move.w     d3,d0
	ext.l      d0
	lsl.l      #2,d0
	movea.l    278(a4),a0
	movea.l    0(a0,d0.l),a1
	move.l     2(a1),d4
	tst.l      d4
	bpl.s      x15176
	move.w     #$07CE,(a5)
	bra        x152b0
x15176:
	move.w     d3,d0
	ext.l      d0
	lsl.l      #2,d0
	movea.l    278(a4),a0
	movea.l    4(a0,d0.l),a1
	move.l     2(a1),d5
	sub.l      d4,d5
	movea.l    0(a0,d0.l),a1
	moveq.l    #0,d0
	move.w     6(a1),d0
	cmpi.b     #$03,1(a1)
	bne.s      x151a8
	move.w     8(a1),d1
	ext.l      d1
	moveq.l    #16,d2
	lsl.l      d2,d1
	or.l       d1,d0
x151a8:
	move.l     d5,d6
	add.l      d0,d6
x151ac:
	cmp.l      d5,d6
	bgt.s      x151bc
	moveq.l    #15,d0
	add.l      d5,d0
	bsr        malloc
	movea.l    a0,a2
	bra.s      x151c6
x151bc:
	moveq.l    #15,d0
	add.l      d5,d0
	bsr        x186bc
	movea.l    a0,a2
x151c6:
	move.l     a2,d0
	beq.s      x15234
	move.l     258(a4),d1
	beq.s      x151d4
	movea.l    d1,a3
	bra.s      x151f2
x151d4:
	lea.l      x1922a,a1
	lea.l      2(a4),a0
	bsr        myfopen
	movea.l    a0,a3
	move.l     a3,d0
	beq.s      x15212
	moveq.l    #64,d1
	and.w      (a4),d1
	beq.s      x151f2
	move.l     d0,258(a4)
x151f2:
	clr.w      d1
	move.l     d4,d0
	movea.l    a3,a0
	bsr        fseek
	tst.w      d0
	bne.s      x15212
	movea.l    a3,a1
	move.l     d5,d1
	moveq.l    #1,d0
	lea.l      14(a2),a0
	bsr        fread
	cmp.l      d0,d5
	beq.s      x1521c
x15212:
	move.w     errno,(a5)
	bra        x152b0
x1521c:
	cmp.l      d5,d6
	bgt.s      x15226
	move.l     a2,(a6)
	suba.l     a2,a2
	bra.s      x15284
x15226:
	moveq.l    #15,d0
	add.l      d6,d0
	bsr        malloc
	move.l     a0,(a6)
	move.l     a0,d0
	bne.s      x1523a
x15234:
	move.w     #2,(a5)
	bra.s      x152b0
x1523a:
	lea.l      14(a2),a0
	move.l     a0,x1d97e
	move.l     d5,x1d982
	movea.l    (a6),a1
	lea.l      14(a1),a1
	move.l     a1,x1d986
	move.l     d6,x1d98a
	moveq.l    #1,d0
	bsr        x16db6
	move.l     x1d982,d0
	ble.s      x15284
	movea.l    (a6),a0
	bsr        free
	clr.l      (a6)
	movea.l    a2,a0
	bsr        x186c2
	suba.l     a2,a2
	add.l      #$10000,d6
	bra        x151ac
x15284:
	movea.l    (a6),a0
	clr.l      (a0)
	movea.l    (a6),a0
	move.l     d6,6(a0)
	movea.l    (a6),a0
	move.w     d3,4(a0)
	movea.l    (a6),a0
	clr.l      10(a0)
	cmpi.l     #$48444F43,262(a4)
	beq.s      x152ac
	movea.l    (a6),a1
	movea.l    a4,a0
	bsr        x150ce
x152ac:
	clr.w      (a5)
	bra.s      x152b8
x152b0:
	movea.l    a6,a1
	movea.l    a4,a0
	bsr        x1509e
x152b8:
	move.l     258(a4),d0
	bne.s      x152c8
	move.l     a3,d1
	beq.s      x152c8
	movea.l    a3,a0
	bsr        fclose
x152c8:
	move.l     a2,d0
	beq.s      x152d2
	movea.l    a2,a0
	bsr        x186c2
x152d2:
	move.w     (a5),d0
	movem.l    (a7)+,d3-d6/a2-a6
	rts

	.globl x177fc
x177fc:
	.globl x1777e
x1777e:
	.globl x172a4
x172a4:
	.globl x176ae
x176ae:
	.globl x17712
x17712:
	.globl x172c8
x172c8:
	.globl x172de
x172de:
	.globl x17482
x17482:
	.globl x17914
x17914:
	.globl x17fb8
x17fb8:
	.globl x18436
x18436:
	.globl x173da
x173da:
	.globl x17574
x17574:
	.globl x175b2
x175b2:
	.globl x17630
x17630:
	.globl x174d6
x174d6:
	.globl x1a076
x1a076:
	.globl x1a0f6
x1a0f6:
	.globl x186e4
x186e4:
	.globl x1665a
x1665a:
	.globl x1726c
x1726c:
	.globl x1814c
x1814c:
	.globl x1811c
x1811c:
	.globl x181b8
x181b8:
	.globl x183b8
x183b8:
	.globl x17900
x17900:
	.globl x173d2
x173d2:
	.globl x182b0
x182b0:
	.globl x17270
x17270:
	.globl x1729e
x1729e:
	.globl x186bc
x186bc:
	.globl x1d97e
x1d97e:
	.globl x1d982
x1d982:
	.globl x1d986
x1d986:
	.globl x1d98a
x1d98a:
	.globl x186c2
x186c2:


	.data
	.globl x1910c
x1910c:

x1915c:

x1915e:

x191fd:

x19200:

x19209:

x1920d:

x1921d:

x19222:

x1922a:

x1922d:

x19232:
