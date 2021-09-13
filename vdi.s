				XREF	_VdiCtrl
				XREF	vdi
				XREF	vdipb
				

VDI_CNTRLMAX    equ		15
VDI_INTINMAX    equ		132
VDI_INTOUTMAX   equ		140
VDI_PTSINMAX    equ		145
VDI_PTSOUTMAX   equ		145

; offsets of VDI control array
				OFFSET	0
v_opcode:		ds.w	1
v_nptsin:		ds.w	1
v_nptsout:		ds.w	1
v_nintin:		ds.w	1
v_nintout:		ds.w	1
v_opcode2:		ds.w	1
v_handle:		ds.w	1
v_param:		ds.l	4

; offsets of VDI parameter block
				OFFSET	0
v_control:		ds.l	1
v_intin:		ds.l	1
v_ptsin:		ds.l	1
v_intout:		ds.l	1
v_ptsout:		ds.l	1


				OFFSET	0

vdi_control:	ds.w	VDI_CNTRLMAX
vdi_intin:		ds.w	VDI_INTINMAX
vdi_intout:		ds.w	VDI_INTOUTMAX
vdi_ptsin:		ds.w	VDI_PTSINMAX
vdi_ptsout:		ds.w	VDI_PTSOUTMAX
_VdiParBSize	equ		(*)

		.bss

	.globl _VDIParBlk
	MODULE		_VDIParBlk
	ds.w	VDI_CNTRLMAX
	ds.w	VDI_INTINMAX
	ds.w	VDI_INTOUTMAX
	ds.w	VDI_PTSOUTMAX
	ds.w	VDI_PTSINMAX
	ENDMOD

	.DATA

    .MODULE VDI_pb

	.dc.l  _VDIParBlk+vdi_control
	.dc.l  _VDIParBlk+vdi_intin
	.dc.l  _VDIParBlk+vdi_ptsin
	.dc.l  _VDIParBlk+vdi_intout
	.dc.l  _VDIParBlk+vdi_ptsout
	
     .ENDMOD

    .MODULE VDI_pb2

	.dc.l  _VDIParBlk+vdi_control
	.dc.l  _VDIParBlk+vdi_intin
	.dc.l  _VDIParBlk+vdi_intout
	.dc.l  _VDIParBlk+vdi_ptsin
	.dc.l  _VDIParBlk+vdi_ptsout
	
     .ENDMOD

	.text

				GLOBL	_VdiCtrl
				MODULE	_VdiCtrl
	LEA.L     _VDIParBlk,A0
	MOVE.W    D0,v_handle(A0)
	MOVE.W    D1,(A0)+
	MOVE.W    D2,4(A0)
	SWAP      D2
	MOVE.W    D2,(A0)
	MOVEQ.L   #$73,D0
	MOVE.L    #VDI_pb,D1
	PEA.L     (A2)
	TRAP      #2
	MOVEA.L   (A7)+,A2
	LEA.L     _VDIParBlk+vdi_intout,A0
	MOVE.W    (A0)+,D0
	RTS
				ENDMOD

				GLOBL	vdi
				MODULE	vdi
				pea		(a2)
				moveq	#$73,d0
				move.l	a0,d1
				trap	#2
				move.l	(a7)+,a2
				rts
				ENDMOD

   GLOBL v_updwk
   .MODULE v_updwk
	MOVEQ.L   #$04,D1
	MOVEQ.L   #$00,D2
	BRA       _VdiCtrl
          .ENDMOD

          .GLOBL v_rbox
          .MODULE v_rbox

	LEA.L     _VDIParBlk+vdi_ptsin,A1
	MOVE.L    (A0)+,(A1)+
	MOVE.L    (A0),(A1)
	MOVEQ.L   #11,D1
	MOVE.L    #$00020000,D2
	MOVE.W    #8,-vdi_ptsin-4+v_opcode2(A1)
	BRA       _VdiCtrl

          .ENDMOD


          .GLOBL v_pline
          .MODULE v_pline

	MOVE.L    A0,VDI_pb+v_ptsin
	MOVE.W    D1,D2
	SWAP      D2
	CLR.W     D2
	MOVEQ.L   #$06,D1
	BSR       _VdiCtrl
	MOVE.L    #_VDIParBlk+vdi_ptsin,VDI_pb+v_ptsin
	RTS

          .ENDMOD


          .GLOBL v_opnwk
          .MODULE v_opnwk

	PEA.L     (A2)
	PEA.L     (A1)
	LEA.L     VDI_pb2,A1
	MOVE.L    A1,D1
	MOVE.L    A0,v_intin(A1)
	MOVEA.L   12(A7),A0
	MOVE.L    A0,v_intout(A1)
	LEA.L     90(A0),A0
	MOVE.L    A0,v_ptsout(A1)
	LEA.L     _VDIParBlk,A0
	MOVE.L    #$00010000,(A0)
	MOVE.W    #11,v_nintin(A0)
	MOVEQ.L   #$73,D0
	TRAP      #2
	MOVEA.L   (A7)+,A0
	MOVE.W    _VDIParBlk+v_handle,(A0)
	MOVEA.L   (A7)+,A2
	RTS

          .ENDMOD


          .GLOBL v_clswk
          .MODULE v_clswk

	MOVEQ.L   #$02,D1
	MOVEQ.L   #$00,D2
	BRA       _VdiCtrl

          .ENDMOD


          .GLOBL v_clrwk
          .MODULE v_clrwk

	MOVEQ.L   #$03,D1
	MOVEQ.L   #$00,D2
	BRA       _VdiCtrl

          .ENDMOD


          .GLOBL v_bar
          .MODULE v_bar

	LEA.L     _VDIParBlk+vdi_ptsin,A1
	MOVE.L    (A0)+,(A1)+
	MOVE.L    (A0),(A1)
	MOVEQ.L   #11,D1
	MOVE.L    #$00020000,D2
	MOVE.W    #1,-vdi_ptsin-4+v_opcode2(A1)
	BRA       _VdiCtrl

          .ENDMOD


          .GLOBL vs_clip
          .MODULE vs_clip

	LEA.L     _VDIParBlk+vdi_ptsin,A1
	MOVE.L    (A0)+,(A1)+
	MOVE.L    (A0),(A1)
	MOVE.W    D1,-vdi_ptsin-4+vdi_intin(A1)
	MOVE.W    #$0081,D1
	MOVE.L    #$00020001,D2
	BRA       _VdiCtrl

          .ENDMOD


		.GLOBL vswr_mode
		.MODULE vswr_mode
	MOVE.W    D1,_VDIParBlk+vdi_intin
	MOVEQ.L   #$20,D1
	MOVEQ.L   #$01,D2
	BRA       _VdiCtrl

          .ENDMOD


          .GLOBL vst_unload_fonts
          .MODULE vst_unload_fonts

	MOVE.W    D1,_VDIParBlk+vdi_intin
	MOVEQ.L   #$78,D1
	MOVEQ.L   #$01,D2
	BRA       _VdiCtrl

          .ENDMOD


          .GLOBL vst_point
          .MODULE vst_point

	PEA.L     (A1)
	PEA.L     (A0)
	MOVE.W    D1,_VDIParBlk+vdi_intin
	MOVEQ.L   #$6B,D1
	MOVEQ.L   #1,D2
	BSR       _VdiCtrl
	LEA.L     vdi_ptsout-vdi_intout-2(A0),A0
	MOVEA.L   (A7)+,A1
	MOVE.W    (A0)+,(A1)
	MOVEA.L   (A7)+,A1
	MOVE.W    (A0)+,(A1)
	MOVEA.L   4(A7),A1
	MOVE.W    (A0)+,(A1)
	MOVEA.L   8(A7),A1
	MOVE.W    (A0),(A1)
	RTS

          .ENDMOD


          .GLOBL v_form_adv
          .MODULE v_form_adv

	MOVEQ.L   #$05,D1
	MOVEQ.L   #$00,D2
	MOVE.W    #$0014,_VDIParBlk+v_opcode2
	BRA       _VdiCtrl

          .ENDMOD


          .GLOBL vst_load_fonts
          .MODULE vst_load_fonts

	MOVE.W    D1,_VDIParBlk+vdi_intin
	MOVEQ.L   #$77,D1
	MOVEQ.L   #$01,D2
	BRA       _VdiCtrl

          .ENDMOD


          .GLOBL vst_font
          .MODULE vst_font

	MOVE.W    D1,_VDIParBlk+vdi_intin
	MOVEQ.L   #$15,D1
	MOVEQ.L   #$01,D2
	BRA       _VdiCtrl

          .ENDMOD


          .GLOBL vst_effects
          .MODULE vst_effects

	MOVE.W    D1,_VDIParBlk+vdi_intin
	MOVEQ.L   #$6A,D1
	MOVEQ.L   #$01,D2
	BRA       _VdiCtrl

          .ENDMOD


          .GLOBL vst_color
          .MODULE vst_color

	MOVE.W    D1,_VDIParBlk+vdi_intin
	MOVEQ.L   #$16,D1
	MOVEQ.L   #$01,D2
	BRA       _VdiCtrl

          .ENDMOD


          .GLOBL vst_alignment
          .MODULE vst_alignment

	PEA.L     (A1)
	PEA.L     (A0)
	MOVEM.W   D1-D2,_VDIParBlk+vdi_intin
	MOVEQ.L   #$27,D1
	MOVEQ.L   #$02,D2
	BSR       _VdiCtrl
	MOVEA.L   (A7)+,A1
	MOVE.W    D0,(A1)
	MOVEA.L   (A7)+,A1
	MOVE.W    (A0),(A1)
	RTS

          .ENDMOD


	.GLOBL vsl_width
	.MODULE vsl_width
	
	MOVE.L    #$00010000,D2
	MOVEM.W   D1-D2,_VDIParBlk+vdi_ptsin
	MOVEQ.L   #$10,D1
	BSR       _VdiCtrl
	MOVE.W    vdi_ptsout-vdi_intout-2(A0),D0
	RTS

          .ENDMOD


          .GLOBL vsl_type
          .MODULE vsl_type

	MOVE.W    D1,_VDIParBlk+vdi_intin
	MOVEQ.L   #$0F,D1
	MOVEQ.L   #$01,D2
	BRA       _VdiCtrl

          .ENDMOD


          .GLOBL vsl_ends
          .MODULE vsl_ends

	MOVEM.W   D1-D2,_VDIParBlk+vdi_intin
	MOVEQ.L   #$6C,D1
	MOVEQ.L   #$02,D2
	BRA       _VdiCtrl

          .ENDMOD


          .GLOBL vsl_color
          .MODULE vsl_color

	MOVE.W    D1,_VDIParBlk+vdi_intin
	MOVEQ.L   #$11,D1
	MOVEQ.L   #$01,D2
	BRA       _VdiCtrl

          .ENDMOD


          .GLOBL vsf_style
          .MODULE vsf_style

	MOVE.W    D1,_VDIParBlk+vdi_intin
	MOVEQ.L   #$18,D1
	MOVEQ.L   #$01,D2
	BRA       _VdiCtrl

          .ENDMOD


          .GLOBL vsf_perimeter
          .MODULE vsf_perimeter

	MOVE.W    D1,_VDIParBlk+vdi_intin
	MOVEQ.L   #$68,D1
	MOVEQ.L   #$01,D2
	BRA       _VdiCtrl

          .ENDMOD


          .GLOBL vsf_interior
          .MODULE vsf_interior

	MOVE.W    D1,_VDIParBlk+vdi_intin
	MOVEQ.L   #$17,D1
	MOVEQ.L   #$01,D2
	BRA       _VdiCtrl

          .ENDMOD


          .GLOBL vsf_color
          .MODULE vsf_color

	MOVE.W    D1,_VDIParBlk+vdi_intin
	MOVEQ.L   #$19,D1
	MOVEQ.L   #$01,D2
	BRA       _VdiCtrl

          .ENDMOD


          .GLOBL vrt_cpyfm
          .MODULE vrt_cpyfm

	MOVE.L    A0,VDI_pb+v_ptsin
	LEA.L     _VDIParBlk+v_param,A0
	MOVE.L    A1,(A0)+
	MOVE.L    4(A7),(A0)
	LEA.L     vdi_intin-v_param-4(A0),A0
	MOVE.W    D1,(A0)+
	MOVEA.L   8(A7),A1
	MOVE.L    (A1),(A0)
	MOVEQ.L   #$79,D1
	MOVE.L    #$00040003,D2
	BSR       _VdiCtrl
	MOVE.L    #_VDIParBlk+vdi_ptsin,VDI_pb+v_ptsin
	RTS

          .ENDMOD


          .GLOBL vro_cpyfm
          .MODULE vro_cpyfm

	MOVE.L    A0,VDI_pb+v_ptsin
	LEA.L     _VDIParBlk+v_param,A0
	MOVE.L    A1,(A0)+
	MOVE.L    4(A7),(A0)
	MOVE.W    D1,vdi_intin-v_param-4(A0)
	MOVEQ.L   #$6D,D1
	MOVE.L    #$00040001,D2
	BSR       _VdiCtrl
	MOVE.L    #_VDIParBlk+vdi_ptsin,VDI_pb+v_ptsin
	RTS

          .ENDMOD


          .GLOBL vq_extnd
          .MODULE vq_extnd

	MOVE.L    A0,VDI_pb+v_intout
	LEA.L     90(A0),A0
	MOVE.L    A0,VDI_pb+v_ptsout
	MOVE.W    D1,_VDIParBlk+vdi_intin
	MOVEQ.L   #$66,D1
	MOVEQ.L   #$01,D2
	BSR       _VdiCtrl
	MOVE.L    #_VDIParBlk+vdi_intout,VDI_pb+v_intout
	MOVE.L    #_VDIParBlk+vdi_ptsout,VDI_pb+v_ptsout
	RTS

          .ENDMOD


          .GLOBL v_gtext
          .MODULE v_gtext

	LEA.L     _VDIParBlk+vdi_intin,A1
	MOVEM.W   D1-D2,vdi_ptsin-vdi_intin(A1)
	MOVE.L    #$00010000,D2
	MOVEQ.L   #$00,D1
	BRA.B     v_gtext2
v_gtext1:
	MOVE.W    D1,(A1)+
	ADDQ.W    #1,D2
v_gtext2:
	MOVE.B    (A0)+,D1
	BNE.B     v_gtext1
	MOVEQ.L   #$08,D1
	BRA       _VdiCtrl

          .ENDMOD


          .GLOBL vqt_name
          .MODULE vqt_name

	PEA.L     (A0)
	MOVE.W    D1,_VDIParBlk+vdi_intin
	MOVE.W    #$0082,D1
	MOVEQ.L   #$01,D2
	BSR       _VdiCtrl
	MOVEA.L   (A7)+,A1
	MOVE.W    -vdi_intout-2+v_nintout(A0),D1
	SUBQ.W    #1,D1
	BRA.B     vqt_name2
vqt_name1:
	MOVE.W    (A0)+,D2
	MOVE.B    D2,(A1)+
vqt_name2:
	DBRA      D1,vqt_name1
	RTS

          .ENDMOD


          .GLOBL vqt_extent
          .MODULE vqt_extent

	MOVE.L    A1,VDI_pb+v_ptsout
	LEA.L     _VDIParBlk+$0000001E,A1
	MOVEQ.L   #$00,D2
	MOVEQ.L   #$00,D1
	BRA.B     vqt_extent2
vqt_extent1:
	MOVE.W    D1,(A1)+
	ADDQ.W    #1,D2
vqt_extent2:
	MOVE.B    (A0)+,D1
	BNE.B     vqt_extent1
	MOVEQ.L   #$74,D1
	BSR       _VdiCtrl
	MOVE.L    #_VDIParBlk+vdi_ptsout,VDI_pb+v_ptsout
	RTS

          .ENDMOD


