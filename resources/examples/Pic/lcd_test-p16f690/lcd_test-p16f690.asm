;Program compiled by Great Cow BASIC (0.96.<<>> 2016-12-14)
;Need help? See the GCBASIC forums at http://sourceforge.net/projects/gcbasic/forums,
;check the documentation or email w_cholmondeley at users dot sourceforge dot net.

;********************************************************************************

;Set up the assembler options (Chip type, clock source, other bits and pieces)
 LIST p=16F690, r=DEC
#include <P16F690.inc>
 __CONFIG _MCLRE_OFF & _WDTE_OFF & _FOSC_HS

;********************************************************************************

;Set aside memory locations for variables
DELAYTEMP	EQU	112
DELAYTEMP2	EQU	113
LCDBYTE	EQU	32
LCDCOLUMN	EQU	33
LCDCRSR	EQU	34
LCDLINE	EQU	35
LCDREADY	EQU	36
LCDTEMP	EQU	37
LCD_STATE	EQU	38
PRINTLEN	EQU	39
STRINGPOINTER	EQU	40
SYSBYTETEMPA	EQU	117
SYSBYTETEMPB	EQU	121
SYSBYTETEMPX	EQU	112
SYSCALCTEMPA	EQU	117
SYSPRINTDATAHANDLER	EQU	41
SYSPRINTDATAHANDLER_H	EQU	42
SYSPRINTTEMP	EQU	43
SYSREPEATTEMP1	EQU	44
SYSREPEATTEMP2	EQU	45
SYSSTRINGA	EQU	119
SYSSTRINGA_H	EQU	120
SYSSTRINGB	EQU	114
SYSSTRINGB_H	EQU	115
SYSSTRINGLENGTH	EQU	118
SYSSTRINGPARAM1	EQU	345
SYSTEMP1	EQU	46
SYSWAITTEMP10US	EQU	117
SYSWAITTEMPMS	EQU	114
SYSWAITTEMPMS_H	EQU	115
SYSWAITTEMPS	EQU	116
SYSWAITTEMPUS	EQU	117
SYSWAITTEMPUS_H	EQU	118

;********************************************************************************

;Vectors
	ORG	0
	pagesel	BASPROGRAMSTART
	goto	BASPROGRAMSTART
	ORG	4
	retfie

;********************************************************************************

;Start of program memory page 0
	ORG	5
BASPROGRAMSTART
;Call initialisation routines
	call	INITSYS
	call	INITLCD

;Start of the main program
SysDoLoop_S1
	call	CLS

	movlw	1
	movwf	SysWaitTempS
	call	Delay_S
	movlw	low SYSSTRINGPARAM1
	movwf	SysStringB
	movlw	high SYSSTRINGPARAM1
	movwf	SysStringB_H
	movlw	low StringTable1
	movwf	SysStringA
	movlw	high StringTable1
	movwf	SysStringA_H
	call	SysReadString
	movlw	low SYSSTRINGPARAM1
	movwf	SysPRINTDATAHandler
	movlw	high SYSSTRINGPARAM1
	movwf	SysPRINTDATAHandler_H
	call	PRINT84

	movlw	1
	movwf	LCDLINE
	clrf	LCDCOLUMN
	call	LOCATE

	movlw	low SYSSTRINGPARAM1
	movwf	SysStringB
	movlw	high SYSSTRINGPARAM1
	movwf	SysStringB_H
	movlw	low StringTable2
	movwf	SysStringA
	movlw	high StringTable2
	movwf	SysStringA_H
	call	SysReadString
	movlw	low SYSSTRINGPARAM1
	movwf	SysPRINTDATAHandler
	movlw	high SYSSTRINGPARAM1
	movwf	SysPRINTDATAHandler_H
	call	PRINT84

	movlw	3
	movwf	SysWaitTempS
	call	Delay_S
	call	CLS

	clrf	LCDLINE
	clrf	LCDCOLUMN
	call	LOCATE

	movlw	low SYSSTRINGPARAM1
	movwf	SysStringB
	movlw	high SYSSTRINGPARAM1
	movwf	SysStringB_H
	movlw	low StringTable3
	movwf	SysStringA
	movlw	high StringTable3
	movwf	SysStringA_H
	call	SysReadString
	movlw	low SYSSTRINGPARAM1
	movwf	SysPRINTDATAHandler
	movlw	high SYSSTRINGPARAM1
	movwf	SysPRINTDATAHandler_H
	call	PRINT84

	movlw	1
	movwf	LCDLINE
	clrf	LCDCOLUMN
	call	LOCATE

	movlw	10
	movwf	LCDCRSR
	call	LCDCURSOR

	movlw	3
	movwf	SysWaitTempS
	call	Delay_S
	call	CLS

	movlw	13
	movwf	LCDCRSR
	call	LCDCURSOR

	clrf	LCDLINE
	clrf	LCDCOLUMN
	call	LOCATE

	movlw	low SYSSTRINGPARAM1
	movwf	SysStringB
	movlw	high SYSSTRINGPARAM1
	movwf	SysStringB_H
	movlw	low StringTable4
	movwf	SysStringA
	movlw	high StringTable4
	movwf	SysStringA_H
	call	SysReadString
	movlw	low SYSSTRINGPARAM1
	movwf	SysPRINTDATAHandler
	movlw	high SYSSTRINGPARAM1
	movwf	SysPRINTDATAHandler_H
	call	PRINT84

	movlw	3
	movwf	SysWaitTempS
	call	Delay_S
	call	CLS

	clrf	LCDLINE
	clrf	LCDCOLUMN
	call	LOCATE

	movlw	low SYSSTRINGPARAM1
	movwf	SysStringB
	movlw	high SYSSTRINGPARAM1
	movwf	SysStringB_H
	movlw	low StringTable5
	movwf	SysStringA
	movlw	high StringTable5
	movwf	SysStringA_H
	call	SysReadString
	movlw	low SYSSTRINGPARAM1
	movwf	SysPRINTDATAHandler
	movlw	high SYSSTRINGPARAM1
	movwf	SysPRINTDATAHandler_H
	call	PRINT84

	movlw	1
	movwf	LCDLINE
	clrf	LCDCOLUMN
	call	LOCATE

	movlw	10
	movwf	LCDCRSR
	call	LCDCURSOR

	movlw	9
	movwf	LCDCRSR
	call	LCDCURSOR

	movlw	3
	movwf	SysWaitTempS
	call	Delay_S
	call	CLS

	clrf	LCDLINE
	clrf	LCDCOLUMN
	call	LOCATE

	movlw	low SYSSTRINGPARAM1
	movwf	SysStringB
	movlw	high SYSSTRINGPARAM1
	movwf	SysStringB_H
	movlw	low StringTable6
	movwf	SysStringA
	movlw	high StringTable6
	movwf	SysStringA_H
	call	SysReadString
	movlw	low SYSSTRINGPARAM1
	movwf	SysPRINTDATAHandler
	movlw	high SYSSTRINGPARAM1
	movwf	SysPRINTDATAHandler_H
	call	PRINT84

	movlw	1
	movwf	LCDLINE
	clrf	LCDCOLUMN
	call	LOCATE

	movlw	14
	movwf	LCDCRSR
	call	LCDCURSOR

	movlw	3
	movwf	SysWaitTempS
	call	Delay_S
	clrf	LCDLINE
	clrf	LCDCOLUMN
	call	LOCATE

	movlw	low SYSSTRINGPARAM1
	movwf	SysStringB
	movlw	high SYSSTRINGPARAM1
	movwf	SysStringB_H
	movlw	low StringTable7
	movwf	SysStringA
	movlw	high StringTable7
	movwf	SysStringA_H
	call	SysReadString
	movlw	low SYSSTRINGPARAM1
	movwf	SysPRINTDATAHandler
	movlw	high SYSSTRINGPARAM1
	movwf	SysPRINTDATAHandler_H
	call	PRINT84

	movlw	1
	movwf	LCDLINE
	clrf	LCDCOLUMN
	call	LOCATE

	movlw	10
	movwf	LCDCRSR
	call	LCDCURSOR

	movlw	9
	movwf	LCDCRSR
	call	LCDCURSOR

	movlw	3
	movwf	SysWaitTempS
	call	Delay_S
	clrf	LCDLINE
	clrf	LCDCOLUMN
	call	LOCATE

	movlw	low SYSSTRINGPARAM1
	movwf	SysStringB
	movlw	high SYSSTRINGPARAM1
	movwf	SysStringB_H
	movlw	low StringTable8
	movwf	SysStringA
	movlw	high StringTable8
	movwf	SysStringA_H
	call	SysReadString
	movlw	low SYSSTRINGPARAM1
	movwf	SysPRINTDATAHandler
	movlw	high SYSSTRINGPARAM1
	movwf	SysPRINTDATAHandler_H
	call	PRINT84

	movlw	1
	movwf	LCDLINE
	clrf	LCDCOLUMN
	call	LOCATE

	movlw	13
	movwf	LCDCRSR
	call	LCDCURSOR

	movlw	14
	movwf	LCDCRSR
	call	LCDCURSOR

	movlw	3
	movwf	SysWaitTempS
	call	Delay_S
	call	CLS

	clrf	LCDLINE
	movlw	4
	movwf	LCDCOLUMN
	call	LOCATE

	movlw	low SYSSTRINGPARAM1
	movwf	SysStringB
	movlw	high SYSSTRINGPARAM1
	movwf	SysStringB_H
	movlw	low StringTable9
	movwf	SysStringA
	movlw	high StringTable9
	movwf	SysStringA_H
	call	SysReadString
	movlw	low SYSSTRINGPARAM1
	movwf	SysPRINTDATAHandler
	movlw	high SYSSTRINGPARAM1
	movwf	SysPRINTDATAHandler_H
	call	PRINT84

	movlw	1
	movwf	LCDLINE
	movlw	4
	movwf	LCDCOLUMN
	call	LOCATE

	movlw	low SYSSTRINGPARAM1
	movwf	SysStringB
	movlw	high SYSSTRINGPARAM1
	movwf	SysStringB_H
	movlw	low StringTable10
	movwf	SysStringA
	movlw	high StringTable10
	movwf	SysStringA_H
	call	SysReadString
	movlw	low SYSSTRINGPARAM1
	movwf	SysPRINTDATAHandler
	movlw	high SYSSTRINGPARAM1
	movwf	SysPRINTDATAHandler_H
	call	PRINT84

	movlw	244
	movwf	SysWaitTempMS
	movlw	1
	movwf	SysWaitTempMS_H
	call	Delay_MS
	movlw	10
	movwf	SysRepeatTemp1
SysRepeatLoop1
	movlw	11
	movwf	LCDCRSR
	call	LCDCURSOR

	movlw	244
	movwf	SysWaitTempMS
	movlw	1
	movwf	SysWaitTempMS_H
	call	Delay_MS
	movlw	12
	movwf	LCDCRSR
	call	LCDCURSOR

	movlw	244
	movwf	SysWaitTempMS
	movlw	1
	movwf	SysWaitTempMS_H
	call	Delay_MS
	decfsz	SysRepeatTemp1,F
	goto	SysRepeatLoop1
SysRepeatLoopEnd1
	call	CLS

	clrf	LCDLINE
	clrf	LCDCOLUMN
	call	LOCATE

	movlw	low SYSSTRINGPARAM1
	movwf	SysStringB
	movlw	high SYSSTRINGPARAM1
	movwf	SysStringB_H
	movlw	low StringTable11
	movwf	SysStringA
	movlw	high StringTable11
	movwf	SysStringA_H
	call	SysReadString
	movlw	low SYSSTRINGPARAM1
	movwf	SysPRINTDATAHandler
	movlw	high SYSSTRINGPARAM1
	movwf	SysPRINTDATAHandler_H
	call	PRINT84

	movlw	1
	movwf	LCDLINE
	clrf	LCDCOLUMN
	call	LOCATE

	movlw	low SYSSTRINGPARAM1
	movwf	SysStringB
	movlw	high SYSSTRINGPARAM1
	movwf	SysStringB_H
	movlw	low StringTable12
	movwf	SysStringA
	movlw	high StringTable12
	movwf	SysStringA_H
	call	SysReadString
	movlw	low SYSSTRINGPARAM1
	movwf	SysPRINTDATAHandler
	movlw	high SYSSTRINGPARAM1
	movwf	SysPRINTDATAHandler_H
	call	PRINT84

	movlw	2
	movwf	SysWaitTempS
	call	Delay_S
	movlw	11
	movwf	LCDCRSR
	call	LCDCURSOR

	movlw	5
	movwf	SysWaitTempS
	call	Delay_S
	call	CLS

	clrf	LCDLINE
	clrf	LCDCOLUMN
	call	LOCATE

	movlw	12
	movwf	LCDCRSR
	call	LCDCURSOR

	movlw	low SYSSTRINGPARAM1
	movwf	SysStringB
	movlw	high SYSSTRINGPARAM1
	movwf	SysStringB_H
	movlw	low StringTable13
	movwf	SysStringA
	movlw	high StringTable13
	movwf	SysStringA_H
	call	SysReadString
	movlw	low SYSSTRINGPARAM1
	movwf	SysPRINTDATAHandler
	movlw	high SYSSTRINGPARAM1
	movwf	SysPRINTDATAHandler_H
	call	PRINT84

	movlw	3
	movwf	SysWaitTempS
	call	Delay_S
	goto	SysDoLoop_S1
SysDoLoop_E1
BASPROGRAMEND
	sleep
	goto	BASPROGRAMEND

;********************************************************************************

CLS
	bcf	PORTB,7
	movlw	1
	movwf	LCDBYTE
	call	LCDNORMALWRITEBYTE

	movlw	4
	movwf	SysWaitTempMS
	clrf	SysWaitTempMS_H
	call	Delay_MS
	movlw	128
	movwf	LCDBYTE
	call	LCDNORMALWRITEBYTE

	movlw	12
	movwf	SysWaitTemp10US
	goto	Delay_10US

;********************************************************************************

Delay_10US
D10US_START
	movlw	15
	movwf	DELAYTEMP
DelayUS0
	decfsz	DELAYTEMP,F
	goto	DelayUS0
	nop
	decfsz	SysWaitTemp10US, F
	goto	D10US_START
	return

;********************************************************************************

Delay_MS
	incf	SysWaitTempMS_H, F
DMS_START
	movlw	227
	movwf	DELAYTEMP2
DMS_OUTER
	movlw	6
	movwf	DELAYTEMP
DMS_INNER
	decfsz	DELAYTEMP, F
	goto	DMS_INNER
	decfsz	DELAYTEMP2, F
	goto	DMS_OUTER
	decfsz	SysWaitTempMS, F
	goto	DMS_START
	decfsz	SysWaitTempMS_H, F
	goto	DMS_START
	return

;********************************************************************************

Delay_S
DS_START
	movlw	232
	movwf	SysWaitTempMS
	movlw	3
	movwf	SysWaitTempMS_H
	call	Delay_MS

	decfsz	SysWaitTempS, F
	goto	DS_START
	return

;********************************************************************************

INITLCD
	banksel	TRISB
	bcf	TRISB,7
	bcf	TRISC,7
	movlw	10
	movwf	SysWaitTempMS
	clrf	SysWaitTempMS_H
	banksel	STATUS
	call	Delay_MS
SysWaitLoop1
	call	FN_LCDREADY
	movf	LCDREADY,F
	btfsc	STATUS,Z
	goto	SysWaitLoop1
	bcf	PORTB,7
	banksel	TRISC
	bcf	TRISC,6
	bcf	TRISC,3
	bcf	TRISC,4
	bcf	TRISC,5
	movlw	15
	movwf	SysWaitTempMS
	clrf	SysWaitTempMS_H
	banksel	STATUS
	call	Delay_MS
	bsf	PORTC,6
	bsf	PORTC,3
	bcf	PORTC,4
	bcf	PORTC,5
	movlw	3
	movwf	DELAYTEMP
DelayUS1
	decfsz	DELAYTEMP,F
	goto	DelayUS1
	bsf	PORTC,7
	movlw	3
	movwf	DELAYTEMP
DelayUS2
	decfsz	DELAYTEMP,F
	goto	DelayUS2
	bcf	PORTC,7

	movlw	5
	movwf	SysWaitTempMS
	clrf	SysWaitTempMS_H
	call	Delay_MS
	movlw	3
	movwf	SysRepeatTemp2
SysRepeatLoop2
	bsf	PORTC,7
	movlw	3
	movwf	DELAYTEMP
DelayUS3
	decfsz	DELAYTEMP,F
	goto	DelayUS3
	bcf	PORTC,7

	movlw	1
	movwf	DELAYTEMP2
DelayUSO4
	clrf	DELAYTEMP
DelayUS4
	decfsz	DELAYTEMP,F
	goto	DelayUS4
	decfsz	DELAYTEMP2,F
	goto	DelayUSO4
	movlw	76
	movwf	DELAYTEMP
DelayUS5
	decfsz	DELAYTEMP,F
	goto	DelayUS5
	decfsz	SysRepeatTemp2,F
	goto	SysRepeatLoop2
SysRepeatLoopEnd2
	movlw	5
	movwf	SysWaitTempMS
	clrf	SysWaitTempMS_H
	call	Delay_MS
	bcf	PORTC,6
	bsf	PORTC,3
	bcf	PORTC,4
	bcf	PORTC,5
	movlw	3
	movwf	DELAYTEMP
DelayUS6
	decfsz	DELAYTEMP,F
	goto	DelayUS6
	bsf	PORTC,7
	movlw	3
	movwf	DELAYTEMP
DelayUS7
	decfsz	DELAYTEMP,F
	goto	DelayUS7
	bcf	PORTC,7

	movlw	83
	movwf	DELAYTEMP
DelayUS8
	decfsz	DELAYTEMP,F
	goto	DelayUS8
	movlw	40
	movwf	LCDBYTE
	call	LCDNORMALWRITEBYTE

	movlw	6
	movwf	LCDBYTE
	call	LCDNORMALWRITEBYTE

	movlw	12
	movwf	LCDBYTE
	call	LCDNORMALWRITEBYTE

	call	CLS

	movlw	12
	movwf	LCD_STATE
	return

;********************************************************************************

INITSYS
	nop
	bcf	ADCON0,ADFM
	bcf	ADCON0,ADON
	banksel	ANSEL
	clrf	ANSEL
	clrf	ANSELH
	bcf	CM2CON0,C2ON
	bcf	CM1CON0,C1ON
	banksel	PORTA
	clrf	PORTA
	clrf	PORTB
	clrf	PORTC
	return

;********************************************************************************

LCDCURSOR
	bcf	PORTB,7
	decf	LCDCRSR,W
	btfss	STATUS, Z
	goto	ENDIF15
	movlw	12
	iorwf	LCD_STATE,W
	movwf	LCDTEMP
ENDIF15
	movlw	12
	subwf	LCDCRSR,W
	btfss	STATUS, Z
	goto	ENDIF16
	movlw	12
	iorwf	LCD_STATE,W
	movwf	LCDTEMP
ENDIF16
	movf	LCDCRSR,F
	btfss	STATUS, Z
	goto	ENDIF17
	movlw	11
	andwf	LCD_STATE,W
	movwf	LCDTEMP
ENDIF17
	movlw	11
	subwf	LCDCRSR,W
	btfss	STATUS, Z
	goto	ENDIF18
	movlw	11
	andwf	LCD_STATE,W
	movwf	LCDTEMP
ENDIF18
	movlw	10
	subwf	LCDCRSR,W
	btfss	STATUS, Z
	goto	ENDIF19
	movlw	10
	iorwf	LCD_STATE,W
	movwf	LCDTEMP
ENDIF19
	movlw	13
	subwf	LCDCRSR,W
	btfss	STATUS, Z
	goto	ENDIF20
	movlw	13
	andwf	LCD_STATE,W
	movwf	LCDTEMP
ENDIF20
	movlw	9
	subwf	LCDCRSR,W
	btfss	STATUS, Z
	goto	ENDIF21
	movlw	9
	iorwf	LCD_STATE,W
	movwf	LCDTEMP
ENDIF21
	movlw	9
	subwf	LCDCRSR,W
	btfss	STATUS, Z
	goto	ENDIF22
	movlw	9
	iorwf	LCD_STATE,W
	movwf	LCDTEMP
ENDIF22
	movlw	14
	subwf	LCDCRSR,W
	btfss	STATUS, Z
	goto	ENDIF23
	movlw	14
	andwf	LCD_STATE,W
	movwf	LCDTEMP
ENDIF23
	movf	LCDTEMP,W
	movwf	LCDBYTE
	call	LCDNORMALWRITEBYTE

	movf	LCDTEMP,W
	movwf	LCD_STATE
	return

;********************************************************************************

LCDNORMALWRITEBYTE
	banksel	TRISC
	bcf	TRISC,6
	bcf	TRISC,3
	bcf	TRISC,4
	bcf	TRISC,5
	banksel	PORTC
	bcf	PORTC,6
	bcf	PORTC,3
	bcf	PORTC,4
	bcf	PORTC,5
	btfsc	LCDBYTE,7
	bsf	PORTC,5
	btfsc	LCDBYTE,6
	bsf	PORTC,4
	btfsc	LCDBYTE,5
	bsf	PORTC,3
	btfsc	LCDBYTE,4
	bsf	PORTC,6
	movlw	1
	movwf	DELAYTEMP
DelayUS9
	decfsz	DELAYTEMP,F
	goto	DelayUS9
	nop
	bsf	PORTC,7
	movlw	3
	movwf	DELAYTEMP
DelayUS10
	decfsz	DELAYTEMP,F
	goto	DelayUS10
	bcf	PORTC,7

	movlw	3
	movwf	DELAYTEMP
DelayUS11
	decfsz	DELAYTEMP,F
	goto	DelayUS11
	bcf	PORTC,6
	bcf	PORTC,3
	bcf	PORTC,4
	bcf	PORTC,5
	btfsc	LCDBYTE,3
	bsf	PORTC,5
	btfsc	LCDBYTE,2
	bsf	PORTC,4
	btfsc	LCDBYTE,1
	bsf	PORTC,3
	btfsc	LCDBYTE,0
	bsf	PORTC,6
	movlw	1
	movwf	DELAYTEMP
DelayUS12
	decfsz	DELAYTEMP,F
	goto	DelayUS12
	nop
	bsf	PORTC,7
	movlw	3
	movwf	DELAYTEMP
DelayUS13
	decfsz	DELAYTEMP,F
	goto	DelayUS13
	bcf	PORTC,7

	movlw	3
	movwf	DELAYTEMP
DelayUS14
	decfsz	DELAYTEMP,F
	goto	DelayUS14
	bcf	PORTC,5
	bcf	PORTC,4
	bcf	PORTC,3
	bcf	PORTC,6
	movlw	133
	movwf	DELAYTEMP
DelayUS15
	decfsz	DELAYTEMP,F
	goto	DelayUS15
	movlw	16
	subwf	LCDBYTE,W
	btfsc	STATUS, C
	goto	ENDIF13
	movf	LCDBYTE,W
	sublw	7
	btfsc	STATUS, C
	goto	ENDIF14
	movf	LCDBYTE,W
	movwf	LCD_STATE
ENDIF14
ENDIF13
	return

;********************************************************************************

FN_LCDREADY
	movlw	10
	movwf	SysWaitTempMS
	clrf	SysWaitTempMS_H
	call	Delay_MS
	movlw	255
	movwf	LCDREADY
	return
	return

;********************************************************************************

LOCATE
	bcf	PORTB,7
	movf	LCDLINE,W
	sublw	1
	btfsc	STATUS, C
	goto	ENDIF1
	movlw	2
	subwf	LCDLINE,F
	movlw	20
	addwf	LCDCOLUMN,F
ENDIF1
	movf	LCDLINE,W
	movwf	SysBYTETempA
	movlw	64
	movwf	SysBYTETempB
	call	SysMultSub
	movf	LCDCOLUMN,W
	addwf	SysBYTETempX,W
	movwf	SysTemp1
	movlw	128
	iorwf	SysTemp1,W
	movwf	LCDBYTE
	call	LCDNORMALWRITEBYTE

	movlw	5
	movwf	SysWaitTemp10US
	goto	Delay_10US

;********************************************************************************

;Overloaded signature: STRING:
PRINT84
	movf	SysPRINTDATAHandler,W
	movwf	FSR
	bcf	STATUS, IRP
	btfsc	SysPRINTDATAHandler_H,0
	bsf	STATUS, IRP
	movf	INDF,W
	movwf	PRINTLEN
	movf	PRINTLEN,F
	btfsc	STATUS, Z
	return
	bsf	PORTB,7
	clrf	SYSPRINTTEMP
	movlw	1
	subwf	PRINTLEN,W
	btfss	STATUS, C
	goto	SysForLoopEnd1
SysForLoop1
	incf	SYSPRINTTEMP,F
	movf	SYSPRINTTEMP,W
	addwf	SysPRINTDATAHandler,W
	movwf	FSR
	bcf	STATUS, IRP
	btfsc	SysPRINTDATAHandler_H,0
	bsf	STATUS, IRP
	movf	INDF,W
	movwf	LCDBYTE
	call	LCDNORMALWRITEBYTE

	movf	PRINTLEN,W
	subwf	SYSPRINTTEMP,W
	btfss	STATUS, C
	goto	SysForLoop1
SysForLoopEnd1
	return

;********************************************************************************

SYSMULTSUB
	clrf	SYSBYTETEMPX
MUL8LOOP
	movf	SYSBYTETEMPA, W
	btfsc	SYSBYTETEMPB, 0
	addwf	SYSBYTETEMPX, F
	bcf	STATUS, C
	rrf	SYSBYTETEMPB, F
	bcf	STATUS, C
	rlf	SYSBYTETEMPA, F
	movf	SYSBYTETEMPB, F
	btfss	STATUS, Z
	goto	MUL8LOOP
	return

;********************************************************************************

SYSREADSTRING
	movf	SYSSTRINGB, W
	movwf	FSR
	bcf	STATUS, IRP
	btfsc	SYSSTRINGB_H, 0
	bsf	STATUS, IRP
	call	SYSSTRINGTABLES
	movwf	SYSCALCTEMPA
	movwf	INDF
	addwf	SYSSTRINGB, F
	goto	SYSSTRINGREADCHECK
SYSREADSTRINGPART
	movf	SYSSTRINGB, W
	movwf	FSR
	bcf	STATUS, IRP
	btfsc	SYSSTRINGB_H, 0
	bsf	STATUS, IRP
	call	SYSSTRINGTABLES
	movwf	SYSCALCTEMPA
	addwf	SYSSTRINGLENGTH,F
	addwf	SYSSTRINGB,F
SYSSTRINGREADCHECK
	movf	SYSCALCTEMPA,F
	btfsc	STATUS,Z
	return
SYSSTRINGREAD
	call	SYSSTRINGTABLES
	incf	FSR, F
	movwf	INDF
	decfsz	SYSCALCTEMPA, F
	goto	SYSSTRINGREAD
	return

;********************************************************************************

SysStringTables
	movf	SysStringA_H,W
	movwf	PCLATH
	movf	SysStringA,W
	incf	SysStringA,F
	btfsc	STATUS,Z
	incf	SysStringA_H,F
	movwf	PCL

StringTable1
	retlw	10
	retlw	83	;S
	retlw	84	;T
	retlw	65	;A
	retlw	82	;R
	retlw	84	;T
	retlw	32	; 
	retlw	84	;T
	retlw	69	;E
	retlw	83	;S
	retlw	84	;T


StringTable2
	retlw	10
	retlw	68	;D
	retlw	73	;I
	retlw	83	;S
	retlw	80	;P
	retlw	76	;L
	retlw	65	;A
	retlw	89	;Y
	retlw	32	; 
	retlw	79	;O
	retlw	78	;N


StringTable3
	retlw	9
	retlw	67	;C
	retlw	117	;u
	retlw	114	;r
	retlw	115	;s
	retlw	111	;o
	retlw	114	;r
	retlw	32	; 
	retlw	79	;O
	retlw	78	;N


StringTable4
	retlw	10
	retlw	67	;C
	retlw	117	;u
	retlw	114	;r
	retlw	115	;s
	retlw	111	;o
	retlw	114	;r
	retlw	32	; 
	retlw	79	;O
	retlw	70	;F
	retlw	70	;F


StringTable5
	retlw	8
	retlw	70	;F
	retlw	76	;L
	retlw	65	;A
	retlw	83	;S
	retlw	72	;H
	retlw	32	; 
	retlw	79	;O
	retlw	78	;N


StringTable6
	retlw	9
	retlw	70	;F
	retlw	76	;L
	retlw	65	;A
	retlw	83	;S
	retlw	72	;H
	retlw	32	; 
	retlw	79	;O
	retlw	70	;F
	retlw	70	;F


StringTable7
	retlw	15
	retlw	67	;C
	retlw	85	;U
	retlw	82	;R
	retlw	83	;S
	retlw	82	;R
	retlw	32	; 
	retlw	38	;&
	retlw	32	; 
	retlw	70	;F
	retlw	76	;L
	retlw	83	;S
	retlw	72	;H
	retlw	32	; 
	retlw	79	;O
	retlw	78	;N


StringTable8
	retlw	16
	retlw	67	;C
	retlw	85	;U
	retlw	82	;R
	retlw	83	;S
	retlw	82	;R
	retlw	32	; 
	retlw	38	;&
	retlw	32	; 
	retlw	70	;F
	retlw	76	;L
	retlw	83	;S
	retlw	72	;H
	retlw	32	; 
	retlw	79	;O
	retlw	70	;F
	retlw	70	;F


StringTable9
	retlw	8
	retlw	70	;F
	retlw	108	;l
	retlw	97	;a
	retlw	115	;s
	retlw	104	;h
	retlw	105	;i
	retlw	110	;n
	retlw	103	;g


StringTable10
	retlw	7
	retlw	68	;D
	retlw	105	;i
	retlw	115	;s
	retlw	112	;p
	retlw	108	;l
	retlw	97	;a
	retlw	121	;y


StringTable11
	retlw	11
	retlw	68	;D
	retlw	73	;I
	retlw	83	;S
	retlw	80	;P
	retlw	76	;L
	retlw	65	;A
	retlw	89	;Y
	retlw	32	; 
	retlw	79	;O
	retlw	70	;F
	retlw	70	;F


StringTable12
	retlw	9
	retlw	70	;F
	retlw	79	;O
	retlw	82	;R
	retlw	32	; 
	retlw	53	;5
	retlw	32	; 
	retlw	83	;S
	retlw	69	;E
	retlw	67	;C


StringTable13
	retlw	8
	retlw	69	;E
	retlw	78	;N
	retlw	68	;D
	retlw	32	; 
	retlw	84	;T
	retlw	69	;E
	retlw	83	;S
	retlw	84	;T


;********************************************************************************

;Start of program memory page 1
	ORG	2048

 END
