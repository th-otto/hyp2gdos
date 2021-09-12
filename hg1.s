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
	bsr        vdi_get_textwidth
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
	bsr        vdi_draw_text
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
	bsr        vdi_draw_text
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
	bsr        vdi_ref
	move.w     2(a3),-(a7)
	clr.w      d2
	moveq.l    #7,d1
	move.w     6(a3),d0
	movea.l    (a2),a0
	bsr        vdi_text_attributes
	addq.w     #2,a7
	pea.l      4(a7)
	lea.l      10(a7),a1
	movea.l    (a2),a0
	bsr        vdi_get_fontwidth
	addq.w     #4,a7
	movea.l    16(a3),a0
	bsr        strlen
	move.l     d0,d3
	tst.w      d3
	ble.s      x1338a
	move.w     d3,d0
	movea.l    16(a3),a1
	movea.l    (a2),a0
	bsr        vdi_get_textwidth
	ext.l      d0
	divs.w     d3,d0
	move.w     d0,6(a7)
x1338a:
	move.w     (a3),-(a7)
	clr.w      d2
	moveq.l    #7,d1
	move.w     6(a3),d0
	movea.l    (a2),a0
	bsr        vdi_text_attributes
	addq.w     #2,a7
	pea.l      (a7)
	lea.l      6(a7),a1
	movea.l    (a2),a0
	bsr        vdi_get_fontwidth
	addq.w     #4,a7
	move.w     (a7),d0
	cmp.w      4(a7),d0
	ble.s      x133b6
	move.w     d0,4(a7)
x133b6:
	movea.l    (a2),a0
	bsr        vdi_unref
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
	bsr        vdi_text_attributes
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
	bsr        vdi_get_textwidth
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
	bsr        vdi_text_attributes
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
	bsr        vdi_ref
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
	bsr        vdi_text_attributes
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
	bsr        vdi_get_textwidth
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
	bsr        vdi_get_textwidth
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
	bsr        vdi_text_attributes
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
	bsr        vdi_text_attributes
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
	bsr        vdi_text_attributes
	addq.w     #2,a7
	movea.l    (a2),a0
	bsr        vdi_unref
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
	bsr        vdi_text_attributes
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
	bsr        vdi_draw_text
	move.w     d3,d0
	movea.l    a4,a1
	movea.l    (a3),a0
	bsr        vdi_get_textwidth
	move.w     d0,d4
	bra.s      x137b0
x13790:
	move.w     d3,d2
	movea.l    a2,a1
	movea.l    42(a7),a0
	move.w     (a0),d1
	move.w     (a5),d0
	movea.l    (a3),a0
	bsr        vdi_draw_text
	move.w     d3,d0
	movea.l    a2,a1
	movea.l    (a3),a0
	bsr        vdi_get_textwidth
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

	.globl print_line
print_line:
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
	bsr        vdi_text_attributes
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
	bsr        vdi_draw_text
	move.w     d6,d0
	movea.l    a5,a1
	movea.l    a4,a0
	bsr        vdi_get_textwidth
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
	bsr        vdi_draw_text
	moveq.l    #1,d0
	movea.l    a2,a1
	movea.l    a4,a0
	bsr        vdi_get_textwidth
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
	bsr        vdi_text_attributes
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
	bsr        vdi_text_attributes
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
	bsr        vdi_text_attributes
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
	bsr        decode_char
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
	bsr        decode_char
	movea.l    a0,a3
	movea.l    a2,a1
	bsr        dec_255_decode
	movea.l    a0,a3
	lea.l      8(a7),a1
	bsr        decode_char
	movea.l    a0,a3
	movea.l    a5,a1
	bsr        decode_char
	movea.l    a0,a3
	lea.l      4(a7),a1
	bsr        decode_char
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
	bsr        decode_char
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
	bsr        vdi_line_attributes
	addq.w     #2,a7
	movea.l    a5,a1
	movea.l    a4,a0
	bsr        vdi_draw_rect
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
	bsr        vdi_draw_bitmap
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
	bsr        vdi_draw_image
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
	bsr        rc_shrink
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
	bsr        vdi_line_attributes
	addq.w     #2,a7
	bra        x14298
x14066:
	lea.l      6(a7),a1
	lea.l      2(a3),a0
	bsr        decode_char
	movea.l    a0,a3
	lea.l      8(a7),a1
	bsr        dec_255_decode
	movea.l    a0,a3
	lea.l      4(a7),a1
	bsr        decode_char
	movea.l    a0,a3
	lea.l      2(a7),a1
	bsr        decode_char
	movea.l    a0,a3
	lea.l      10(a7),a1
	bsr        decode_char
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
	bsr        vdi_line_attributes
	addq.w     #2,a7
	move.w     d6,d0
	lea.l      32(a7),a1
	movea.l    a4,a0
	bsr        vdi_draw_arrowed
	bra        x143bc
x1414e:
	lea.l      6(a7),a1
	lea.l      2(a3),a0
	bsr        decode_char
	movea.l    a0,a3
	lea.l      8(a7),a1
	bsr        dec_255_decode
	movea.l    a0,a3
	lea.l      4(a7),a1
	bsr        decode_char
	movea.l    a0,a3
	lea.l      2(a7),a1
	bsr        decode_char
	movea.l    a0,a3
	lea.l      10(a7),a1
	bsr        decode_char
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
	bsr        vdi_fill_attributes
	movea.l    a5,a1
	movea.l    a4,a0
	bsr        vdi_draw_bar
	moveq.l    #1,d0
	move.w     d0,-(a7)
	move.w     d0,d2
	moveq.l    #7,d1
	movea.l    a4,a0
	bsr        vdi_line_attributes
	addq.w     #2,a7
x14298:
	movea.l    a5,a1
	movea.l    a4,a0
	bsr        vdi_draw_rect
	bra        x143bc
x142a4:
	lea.l      6(a7),a1
	lea.l      2(a3),a0
	bsr        decode_char
	movea.l    a0,a3
	lea.l      8(a7),a1
	bsr        dec_255_decode
	movea.l    a0,a3
	lea.l      4(a7),a1
	bsr        decode_char
	movea.l    a0,a3
	lea.l      2(a7),a1
	bsr        decode_char
	movea.l    a0,a3
	lea.l      10(a7),a1
	bsr        decode_char
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
	bsr        vdi_fill_attributes
	moveq.l    #10,d0
	movea.l    a5,a1
	movea.l    a4,a0
	bsr        vdi_draw_rounded_box
	moveq.l    #1,d0
	move.w     d0,-(a7)
	move.w     d0,d2
	moveq.l    #7,d1
	movea.l    a4,a0
	bsr        vdi_line_attributes
	addq.w     #2,a7
	moveq.l    #10,d0
	movea.l    a5,a1
	movea.l    a4,a0
	bsr        vdi_draw_rounded_rect
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

	.ifne 0

	.endc

	.data
	.globl x1910c
x1910c:

x1915c:
