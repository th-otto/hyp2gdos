*      PCXSTART.S
*
*      Pure C Extended Startup Code
*
*      Copyright (c) Borland International 1988/89/90
*      All Rights Reserved.


*>>>>>> Information <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

*        This extended version of PCSTART.S handles I/O redirection
*        commands on the application's command line.
*        Don't use this version if your preferred command line shell
*        implements I/O redirection.



*>>>>>> Export references <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

        .EXPORT exit, __exit

        .EXPORT _BasPag
        .EXPORT _app
        .EXPORT errno
        .EXPORT _AtExitVec, _FilSysVec
        .EXPORT _RedirTab
        .EXPORT _StkLim
        .EXPORT _PgmSize

        .EXPORT __text, __data, __bss

*>>>>>> Import references <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

        .IMPORT main
        .IMPORT _fpuinit
        .IMPORT _StkSize
        .IMPORT _FreeAll




*>>>>>> Data structures <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


* Base page structure

        .OFFSET 0

TpaStart:
        .DS.L   1
TpaEnd:
        .DS.L   1
TextSegStart:
        .DS.L   1
TextSegSize:
        .DS.L   1
DataSegStart:
        .DS.L   1
DataSegSize:
        .DS.L   1
BssSegStart:
        .DS.L   1
BssSegSize:
        .DS.L   1
DtaPtr:
        .DS.L   1
PntPrcPtr:
        .DS.L   1
Reserved0:
        .DS.L   1
EnvStrPtr:
        .DS.L   1
Reserved1:
        .DS.B   7
CurDrv:
        .DS.B   1
Reserved2:
        .DS.L   18
CmdLine:
        .DS.B   128
BasePageSize:
        .DS     0


*>>>>>>> Locals <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    ARGV        EQU     A2
    BASPAGPTR   EQU     A3
    ENVP        EQU     A4

    ARGC        EQU     D3
    REDIRFLAG   EQU     D4
    REDIROHNDL  EQU     D5
    REDIRIHNDL  EQU     D6
    ARGVFLAG    EQU     D7


*>>>>>>> Constants <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    REDOUT      EQU     00
    REDAPP      EQU     01
    REDIN       EQU     02

    stdin       EQU     00
    stdout      EQU     01


*>>>>>>> Data segment <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

        .BSS
__bss:

* Pointer to base page

_BasPag:
        .DS.L   1


* Applikation flag

_app:
        .DS.W   1


* Stack limit

_StkLim:
        .DS.L   1

* Program size

_PgmSize:
        .DS.L   1

* Redirection address table

_RedirTab:
        .DS.L   6


*>>>>>>> Initialized data segment <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

        .DATA
__data:

* Global error variable

errno:
        .DC.W   0


* Vector for atexit

_AtExitVec:
        .DC.L   0


* Vector for file system deinitialization

_FilSysVec:
        .DC.L   0


*>>>>>>> Code segment <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

        .CODE
__text:


******** PcStart ********************************************************

Start:
        BRA.B   Start0



******* Configuration data


* Redirection array pointer

        .DC.L   _RedirTab


* Stack size entry

        .DC.L   _StkSize



******* Pc library copyright note

        .ALIGN  16

        .DC.B   '>>>> PURE-C <<<<'
        .DC.B   '  for ATARI-ST  '
        .DC.B   '  Version  1.0  '
        .DC.B   '(C)88-90 Borland'
        .DC.B   ' International  '

EmpStr:
        .DC.B   $00
        .EVEN



******** Pc startup code

* Setup pointer to base page

Start0:
        MOVE.L  A0, A3
        MOVE.L  A3, D0
        BNE     ACC

        MOVE.L  4(A7), A3       ; BasePagePointer from Stack
        MOVEQ.L #1, D0          ; Program is Application
        BRA     APP
ACC:
        CLR.W   D0              ; Program is DeskAccessory

APP:

        MOVE.L  A3, _BasPag

* Setup applikation flag

        MOVE.W  D0,_app


* Compute size of required memory
* := text segment size + data segment size + bss segment size
*  + stack size + base page size
* (base page size includes stack size)

        MOVE.L  TextSegSize(BASPAGPTR),A0
        ADD.L   DataSegSize(BASPAGPTR),A0
        ADD.L   BssSegSize(BASPAGPTR),A0
        ADD.W   #BasePageSize,A0
        MOVE.L  A0, _PgmSize

* Setup longword aligned application stack

        MOVE.L  BASPAGPTR,D0
        ADD.L   A0,D0
        AND.B   #$FC,D0
        MOVE.L  D0,A7

* check application flag

        TST.W   _app
        BEQ     Start14   * No arguments

* Free not required memory

        MOVE.L  A0,-(A7)
        MOVE.L  BASPAGPTR,-(A7)
        MOVE.W  #0,-(A7)
        MOVE.W  #74,-(A7)
        TRAP    #1
        LEA.L   12(A7),A7

* scan environment
		move.l  a7,d0
        SUB.L   #_StkSize-4, D0
        AND.B   #$FC, D0
        MOVE.L  D0, A1
        MOVE.L  A1, ENVP
        MOVE.L  EnvStrPtr(BASPAGPTR), A2
        move.b  CmdLine(a3),d5
        move.b  #'=',d6
        move.l  #'VGRA',d7
        move.l  a7,a6
Start01:
        MOVE.L  A2,(A1)+
        move.l  a2,a5
        tst.b   (a2)+
        beq.s   Start1_2
Start1:
        TST.B   (A2)+
        BNE     Start1
		move.b     (a5)+,-(a6)
		move.b     (a5)+,-(a6)
		move.b     (a5)+,-(a6)
		move.b     (a5)+,-(a6)
		cmp.l      (a6)+,d7
		bne.s      Start01
		cmp.b      (a5),d6
		bne.s      Start01
		cmp.b      #$7F,d5
		bne.s      Start01
		clr.b      -4(a5)
		clr.l      -4(a1)
		movea.l    a1,a5
		clr.l      ARGC
		move.l     a2,(a1)+
Start1_1:
		tst.b      (a2)+
		bne.s      Start1_1
        MOVE.L  A2, (A1)+
        addq.l  #1, ARGC
        TST.B   (A2)
        BNE     Start1_1
		movea.l    a1,a6
        CLR.L   -(A1)
        bra Start14

Start1_2:
	movea.l    a1,a6
	clr.l      -(a1)
	lea.l      CmdLine(a3),a0
	move.w     #1,ARGC
	move.b     (a0),d1
	ext.w      d1
	lea.l      1(a0,d1.w),a1
	clr.b      (a1)
	clr.l      -(a7)
    bra.s      Start5

Start1_3:

	cmpi.b     #$21,(a1)
	bpl.s      Start2
	clr.b      (a1)
	tst.b      1(a1)
	beq.s      Start2
	pea.l      1(a1)
	addq.w     #1,ARGC
	bra.s      Start5
Start2:
	cmpi.b     #'"',(a1)
	bne.s      Start5
	move.b     #0,(a1)
Start3:
	subq.w     #1,a1
	cmpi.b     #'"',(a1)
Start4:
	dbeq       d1,Start3
	subq.w     #1,d1
	bmi.s      Start6
	pea.l      1(a1)
	addq.w     #1,d3
	clr.b      (a1)
Start5:
	subq.l     #1,a1
	dbf        d1,Start1_3
Start6:
	tst.b      1(a1)
	beq.s      Start7
	pea.l      1(a1)
	addq.w     #1,ARGC
Start7:
	pea EmpStr
	movea.l    a7,a5

* Test if fpu 68881 is present

Start14:
        JSR     _fpuinit

		lea.l      256(a6),a6
		move.l     a6,_StkLim

******* Execute main program *******************************************
*
* Parameter passing:
*   <D0.W> = Command line argument count (argc)
*   <A0.L> = Pointer to command line argument pointer array (argv)
*   <A1.L> = Pointer to tos environment string (env)

        MOVE    ARGC, D0
        MOVE.L  a5, A0
        MOVE.L  ENVP, A1
        JSR     main



******** exit ***********************************************************
*
* Terminate program
*
* Entry parameters:
*   <D0.W> = Termination status : Integer
* Return parameters:
*   Never returns

exit:
        MOVE.W  D0,-(A7)

* Execute all registered atexit procedures

        MOVE.L  _AtExitVec,D0
        BEQ     __exit

        MOVE.L  D0,A0
        JSR     (A0)



* Deinitialize file system
__exit:

        MOVE.L  _FilSysVec,D0
        BEQ     Exit1

        MOVE.L  D0,A0
        JSR     (A0)


* Deallocate all heap blocks

Exit1:
        JSR     _FreeAll


* Program termination with return code

        MOVE.W  #76,-(A7)
        TRAP    #1


******* Module end *****************************************************

        .END
