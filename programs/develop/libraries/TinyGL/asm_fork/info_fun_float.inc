;
; �㭪樨 ��� ࠡ��� � �᫠�� float
;

; ������⢮ ������ �᫠ ��᫥ ����⮩ (1-17)
NumberSymbolsAD DW 5
; ����⠭�� (10 � �⥯��� N)
MConst DQ 1.0E1,1.0E2,1.0E3,1.0E4,1.0E5
       DQ 1.0E6,1.0E7,1.0E8,1.0E9,1.0E10
       DQ 1.0E11,1.0E12,1.0E13,1.0E14,1.0E15
       DQ 1.0E16,1.0E17,1.0E18,1.0E19,1.0E20
       DQ 1.0E21,1.0E22,1.0E23,1.0E24,1.0E25
       DQ 1.0E26,1.0E27,1.0E28,1.0E29,1.0E30
       DQ 1.0E31,1.0E32,1.0E33,1.0E34,1.0E35
       DQ 1.0E36,1.0E37,1.0E38,1.0E39,1.0E40
       DQ 1.0E41,1.0E42,1.0E43,1.0E44,1.0E45
       DQ 1.0E46,1.0E47,1.0E48,1.0E49,1.0E50
       DQ 1.0E51,1.0E52,1.0E53,1.0E54,1.0E55
       DQ 1.0E56,1.0E57,1.0E58,1.0E59,1.0E60
       DQ 1.0E61,1.0E62,1.0E63,1.0E64,1.0E65
       DQ 1.0E66,1.0E67,1.0E68,1.0E69,1.0E70
       DQ 1.0E71,1.0E72,1.0E73,1.0E74,1.0E75
       DQ 1.0E76,1.0E77,1.0E78,1.0E79,1.0E80
       DQ 1.0E81,1.0E82,1.0E83,1.0E84,1.0E85
       DQ 1.0E86,1.0E87,1.0E88,1.0E89,1.0E90
       DQ 1.0E91,1.0E92,1.0E93,1.0E94,1.0E95
       DQ 1.0E96,1.0E97,1.0E98,1.0E99,1.0E100
       DQ 1.0E101,1.0E102,1.0E103,1.0E104,1.0E105
       DQ 1.0E106,1.0E107,1.0E108,1.0E109,1.0E110
       DQ 1.0E111,1.0E112,1.0E113,1.0E114,1.0E115
       DQ 1.0E116,1.0E117,1.0E118,1.0E119,1.0E120
       DQ 1.0E121,1.0E122,1.0E123,1.0E124,1.0E125
       DQ 1.0E126,1.0E127,1.0E128
; ��᫮ � ������饩 ����⮩ ������� �筮��
Data_Double   DQ ?
; ��᫮ � BCD-�ଠ� 
Data_BCD      DT ?
; �ᯮ����⥫�� 䫠�
Data_Flag     DB ?
; ���� १���� (�᫨ �� 0 - ����⥫쭮� �᫮)
Data_Sign     DB ?


db 0 ;㪠��⥫� �� ᤢ�� � �����
; ��ப� ��� �࠭���� �᫠ � ���� ASCII
Data_String   DB 32 DUP (?)



;*******************************************************
;*  �������������� ����� � ��������� ������� � ������  *
;* ��᫮ ����� �ଠ� � 㤢������ �筮����, १���� *
;* �뤠���� � �����筮� ����, � "��⮢��" �ଠ� �   *
;* 䨪�஢���� ������⢮� ������ ��᫥ ����⮩.     *
;* �室�� ��ࠬ����:                                  *
;* Data_Double - �८�ࠧ㥬�� �᫮;                  *
;* NumberSymbolsAD - ������⢮ ������ ��᫥           *
;*                   ����⮩ (0-17).                   *
;* ��室�� ��ࠬ����:                                 *
;* Data_String - ��ப�-१����.                     *
;*******************************************************
DoubleFloat_to_String:
	pushad
	; ������� �����뢠�� � ��ப� Data_String
	lea EDI, [Data_String]

	; �������� �᫮ ����� �� NumberSymbolsAD
	; �������� ࠧ�冷�
	fninit		       ;��� ᮯ�����
	fld	[Data_Double]  ;����㧨�� �᫮
	xor ebx,ebx
	mov	BX,[NumberSymbolsAD]
	cmp	BX, 0
	je	.NoShifts     ;��� ��� ��᫥ ����⮩
	jl	.Error	      ;�訡��
	dec	BX
	shl	BX, 3		;㬭����� �� 8
	lea eax,[MConst]
	add	EBX, eax
	fmul	qword [EBX] ;㬭����� �� ����⠭��
.NoShifts:
	; ������� �᫮ � ���� BCD
	fbstp	[Data_BCD]
; �஢���� १���� �� ��९�������
	mov	AX,word [Data_BCD + 8]
	cmp	AX,0FFFFh  ;"�����筮�" ��९�������?
	je	.Overflow
; �뤥���� ���� �᫠ � ������� ��� � ASCII-����
	mov	AL, byte [Data_BCD + 9]
	and	AL,AL
	jz	.NoSign
	mov	AL,'-'
	stosb
.NoSign:
; ��ᯠ������ �᫮ � ��� ASCII
	mov	ebx,8	 ;ᬥ饭�� ��᫥���� ���� ���
	mov	ecx,9	 ;���稪 ��� ���
	; ��।����� ������ �����筮� �窨 � �᫥
	mov	DX,18
	sub	DX,[NumberSymbolsAD]
	js	.Error	;�訡��, �᫨ ����⥫쭠�
	jz	.Error	;��� �㫥��� ������
.NextPair:
	; ����㧨�� ��।��� ���� ࠧ�冷�
	mov	AL, byte [ebx + Data_BCD]
	mov	AH,AL
	; �뤥����, ��ॢ��� � ASCII �
	; ��࠭��� ������ ��ࠤ�
	shr	AL,4
	add	AL,'0'
	stosb
	dec	DX
	jnz	.N0
	mov	AL,'.'
	stosb
.N0:   ; �뤥����, ��ॢ��� � ASCII �
	; ��࠭��� ������� ��ࠤ�
	mov	AL,AH
	and	AL,0Fh
	add	AL,'0'
	stosb
	dec	DX
	jnz	.N1
	mov	AL,'.'
	stosb
.N1:
	dec  BX
	loop .NextPair
	mov  AL,0
	stosb

; ����� ������騥 �㫨 ᫥��
	lea	EDI, [Data_String]
	lea	ESI, [Data_String]
	; �ய����� ���� �᫠, �᫨ �� ����
	cmp	byte [ESI],'-'
	jne	.N2
	inc	ESI
	inc	EDI
.N2:   ; ����㧨�� � ���稪 横�� ������⢮ ࠧ�冷�
	; �᫠ ���� 1 (���� �����筮� �窨)
	mov	ecx,18+1+1
	; �ய����� ������騥 �㫨
.N3:
	cmp byte [ESI],'0'
	jne .N4
	cmp byte [ESI+1],'.'
	je .N4
	inc ESI
	loop .N3
	; �訡�� - ��� ������ ���
	jmp	.Error
; �����஢��� ������� ���� �᫠ � ��砫� ��ப�
.N4:	rep movsb
	jmp    .End

; �訡��
.Error:
	mov	AL,'E'
	stosb
	mov	AL,'R'
	stosb
	mov	AL,'R'
	stosb
	xor	AL,AL
	stosb
	jmp	.End
; ��९������� ࠧ�來�� �⪨
.Overflow:
	mov	AL,'#'
	stosb
	xor	AL,AL
	stosb
; ����� ��楤���
.End:
	popad
	ret

;****************************************************
;* ������������� ������ � ����� � ��������� ������� *
;*      (�᫮ ����� �����, "��⮢��" �ଠ�)     *
;* �室�� ��ࠬ����:                               *
;* Data_String - �᫮ � ���� ASCII.                *
;* ��室�� ��ࠬ����:                              *
;* Data_Double - �᫮ � ����筮� ����.             *
;****************************************************
String_to_DoubleFloat:
	pushad
	cld
	; ��頥� Data_BCD 
	mov dword [Data_BCD],0
	mov dword [Data_BCD+4],0
	mov  word [Data_BCD+8],0
	; ��頥� ���� �����
	mov	[Data_Sign],0
	; ����ᨬ � SI 㪠��⥫� �� ��ப�
	lea	ESI, [Data_String]
	; �ய�᪠�� �஡��� ��। �᫮�
	mov	ecx,64 ;���� �� ��横�������
.ShiftIgnore:
	lodsb
	cmp	AL,' '
	jne	.ShiftIgnoreEnd
	loop	.ShiftIgnore
	jmp	.Error
.ShiftIgnoreEnd:
	; �஢��塞 ���� �᫠
	cmp	AL,'-'
	jne	.Positive
	mov	[Data_Sign],80h
	lodsb
.Positive:
	mov	[Data_Flag],0 ;�ਧ��� ������ �窨
	mov	DX,0	      ;������ �窨
	mov	ecx,18	      ;����. �᫮ ࠧ�冷�
.ASCIItoBCDConversion:
	cmp	AL,'.'	      ;�窠?
	jne	.NotDot
	cmp	[Data_Flag],0 ;�窠 �� ����砫���?
	jne	.Error
	mov	[Data_Flag],1
	lodsb
	cmp	AL,0	      ;����� ��ப�?
	jne	.NotDot
	jmp	.ASCIItoBCDConversionEnd
.NotDot:
	; �������� �� 1 ���祭�� ����樨 �窨,
	; �᫨ ��� �� �� ����砫���
	cmp	[Data_Flag],0
	jnz	.Figures
	inc	DX
.Figures:
	; ������� �᫠ ������ ���� ��ࠬ�
	cmp	AL,'0'
	jb	.Error
	cmp	AL,'9'
	ja	.Error
	; ��襬 ��।��� ���� � ������� ��ࠤ� BCD
	and	AL,0Fh
	or	byte [Data_BCD],AL
	; �஢�ઠ �� ����� ��ப�
	cmp	byte [ESI],0
	je	.ASCIItoBCDConversionEnd
	; �������� BCD �� 4 ࠧ�鸞 �����
	; (ᤢ����� ���訥 2 ����)
	mov	AX,word [Data_BCD+6]
	shld	word [Data_BCD+8],AX,4
	; (ᤢ����� �।��� 4 ����)
	mov	EAX, dword [Data_BCD]
	shld	dword [Data_BCD+4],EAX,4
	; (ᤢ����� ����訥 4 ����)
	shl	dword [Data_BCD],4
	; ����㦠�� ᫥���騩 ᨬ��� � AL
	lodsb
	loop	.ASCIItoBCDConversion
	; �᫨ 19-� ᨬ��� �� 0 � �� �窠,
	; � �訡�� ��९�������
	cmp	AL,'.'
	jne	.NotDot2
	inc	ecx
	lodsb
.NotDot2:
	cmp	AL,0
	jne	.Error ;��९������� ࠧ�來�� �⪨

; ������������� ����� �� ���� BCD � ������������ �����
.ASCIItoBCDConversionEnd:
	; ������ ���� � ���訩 ����
	mov	AL,[Data_Sign]
	mov	byte [Data_BCD+9],AL
	; ������ ॣ����� ᮯ�����
	fninit
	; ����㧨�� � ᮯ����� �᫮ � BCD-�ଠ�
	fbld	[Data_BCD]
	; ���᫨�� ����� ����⥫�
	mov	EBX,18+1
	sub	BX,CX
	sub	BX,DX
	cmp	EBX,0
	je	.NoDiv
	dec	EBX
	shl	EBX,3		;㬭����� �� 8
	lea eax,[MConst]
	add	EBX,eax
	fdiv	qword [EBX] ;ࠧ������ �� ����⠭��
.NoDiv:; ���㧨�� �᫮ � ����筮� �ଠ�
	fstp	[Data_Double]
	jmp	.End

.Error:; �� �� �訡�� ���㫨�� १����
	fldz	;������ ���� � �⥪ ᮯ�����
	fstp	[Data_Double]
.End:
	popad
	ret

align 4
proc str_cat, str1:dword, str2:dword
	push eax ecx edi esi
	mov esi,dword[str2]
	stdcall str_len,esi
	mov ecx,eax
	inc ecx
	mov edi,dword[str1]
	stdcall str_len,edi
	add edi,eax
	cld
	repne movsb
	pop esi edi ecx eax
	ret
endp

;output:
; eax = strlen
align 4
proc str_len, str1:dword
	mov eax,[str1]
	@@:
		cmp byte[eax],0
		je @f
		inc eax
		jmp @b
	@@:
	sub eax,[str1]
	ret
endp