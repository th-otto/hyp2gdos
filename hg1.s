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

	.globl print_link
print_link:
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

	.ifne 0

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
	bsr        print_link
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
	bsr        print_link
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

	.endc

	.data
	.globl x1910c
x1910c:

x1915c:
