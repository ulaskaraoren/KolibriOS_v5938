gouraud_triangle:
;------------------in - eax - x1 shl 16 + y1 ---------
;---------------------- ebx - x2 shl 16 + y2 ---------
;---------------------- ecx - x3 shl 16 + y3 ---------
;---------------------- edi - pointer to screen buffer
;---------------------- stack : colors----------------
;----------------- procedure don't save registers !!--
.col1r equ ebp+4	     ; each color as word
.col1g equ ebp+6
.col1b equ ebp+8
.col2r equ ebp+10
.col2g equ ebp+12
.col2b equ ebp+14
.col3r equ ebp+16
.col3g equ ebp+18
.col3b equ ebp+20

.x1    equ word[ebp-2]
.y1    equ word[ebp-4]
.x2    equ word[ebp-6]
.y2    equ word[ebp-8]
.x3    equ word[ebp-10]
.y3    equ word[ebp-12]

.dc12r equ dword[ebp-16]
.dc12g equ dword[ebp-20]
.dc12b equ dword[ebp-24]
.dc13r equ dword[ebp-28]
.dc13g equ dword[ebp-32]
.dc13b equ dword[ebp-36]
.dc23r equ dword[ebp-40]
.dc23g equ dword[ebp-44]
.dc23b equ dword[ebp-48]

.c1r   equ dword[ebp-52]
.c1g   equ dword[ebp-56]
.c1b   equ dword[ebp-60]
.c2r   equ dword[ebp-64]
.c2g   equ dword[ebp-68]
.c2b   equ dword[ebp-72]

.dx12  equ dword[ebp-76]
.dx13  equ dword[ebp-80]
.dx23  equ dword[ebp-84]



       mov ebp,esp
;       sub esp,72

 .sort3:		  ; sort triangle coordinates...
       cmp ax,bx
       jle .sort1
       xchg eax,ebx
       mov edx,dword[.col1r]
       xchg edx,dword[.col2r]
       mov dword[.col1r],edx
       mov dx,word[.col1b]
       xchg dx,word[.col2b]
       mov word[.col1b],dx
 .sort1:
       cmp bx,cx
       jle .sort2
       xchg ebx,ecx
       mov edx,dword[.col2r]
       xchg edx,dword[.col3r]
       mov dword[.col2r],edx
       mov dx,word[.col2b]
       xchg dx,word[.col3b]
       mov word[.col2b],dx
       jmp .sort3
 .sort2:
       push eax   ;store triangle coordinates in user friendly variables
       push ebx
       push ecx
       sub esp,72 ; set correctly value of esp

       mov edx,eax    ; check only X triangle coordinate
       or edx,ebx
       or edx,ecx
       test edx,80000000h
       jne .gt_loop2_end
       shr eax,16
       cmp ax,SIZE_X-1
       jg .gt_loop2_end
       shr ebx,16
       cmp bx,SIZE_X-1
       jg .gt_loop2_end
       shr ecx,16
       cmp cx,SIZE_X-1
       jg .gt_loop2_end


       mov bx,.y2	; calc deltas
       sub bx,.y1
       jnz .gt_dx12_make
       mov .dx12,0
       mov .dc12r,0
       mov .dc12g,0
       mov .dc12b,0
       jmp .gt_dx12_done
  .gt_dx12_make:

       mov ax,.x2
       sub ax,.x1
       cwde
       movsx ebx,bx
       shl eax,ROUND
       cdq
       idiv ebx
       mov .dx12,eax

       mov ax,word[.col2r]
       sub ax,word[.col1r]
       cwde
       shl eax,ROUND
       cdq
       idiv ebx
       mov .dc12r,eax
       mov ax,word[.col2g]
       sub ax,word[.col1g]
       cwde
       shl eax,ROUND
       cdq
       idiv ebx
       mov .dc12g,eax
       mov ax,word[.col2b]
       sub ax,word[.col1b]
       cwde
       shl eax,ROUND
       cdq
       idiv ebx
       mov .dc12b,eax
.gt_dx12_done:

       mov bx,.y3
       sub bx,.y1
       jnz .gt_dx13_make
       mov .dx13,0
       mov .dc13r,0
       mov .dc13g,0
       mov .dc13b,0
       jmp .gt_dx13_done
.gt_dx13_make:
       mov ax,.x3
       sub ax,.x1
       cwde
       movsx ebx,bx
       shl eax,ROUND
       cdq
       idiv ebx
       mov .dx13,eax

       mov ax,word[.col3r]
       sub ax,word[.col1r]
       cwde
       shl eax,ROUND
       cdq
       idiv ebx
       mov .dc13r,eax
       mov ax,word[.col3g]
       sub ax,word[.col1g]
       cwde
       shl eax,ROUND
       cdq
       idiv ebx
       mov .dc13g,eax
       mov ax,word[.col3b]
       sub ax,word[.col1b]
       cwde
       shl eax,ROUND
       cdq
       idiv ebx
       mov .dc13b,eax
.gt_dx13_done:

       mov bx,.y3
       sub bx,.y2
       jnz .gt_dx23_make
       mov .dx23,0
       mov .dc23r,0
       mov .dc23g,0
       mov .dc23b,0
       jmp .gt_dx23_done
.gt_dx23_make:
       mov ax,.x3
       sub ax,.x2
       cwde
       movsx ebx,bx
       shl eax,ROUND
       cdq
       idiv ebx
       mov .dx23,eax

       mov ax,word[.col3r]
       sub ax,word[.col2r]
       cwde
       shl eax,ROUND
       cdq
       idiv ebx
       mov .dc23r,eax
       mov ax,word[.col3g]
       sub ax,word[.col2g]
       cwde
       shl eax,ROUND
       cdq
       idiv ebx
       mov .dc23g,eax
       mov ax,word[.col3b]
       sub ax,word[.col2b]
       cwde
       shl eax,ROUND
       cdq
       idiv ebx
       mov .dc23b,eax
.gt_dx23_done:

       movsx eax,.x1
       shl eax,ROUND
       mov ebx,eax
       movsx edx,word[.col1r]
       shl edx,ROUND
       mov .c1r,edx
       mov .c2r,edx
       movsx edx,word[.col1g]
       shl edx,ROUND
       mov .c1g,edx
       mov .c2g,edx
       movsx edx,word[.col1b]
       shl edx,ROUND
       mov .c1b,edx
       mov .c2b,edx
       mov cx,.y1
       cmp cx,.y2
       jge .gt_loop1_end
.gt_loop1:
       push eax 		      ; eax - cur x1
       push ebx 		      ; ebx - cur x2
       push cx			      ; cx  - cur y
       push edi
       push ebp

       mov edx,.c2r		 ; c2r,c2g,c2b,c1r,c1g,c1b - current colors
       sar edx,ROUND
       push dx
       mov edx,.c2g
       sar edx,ROUND
       push dx
       mov edx,.c2b
       sar edx,ROUND
       push dx
       mov edx,.c1r
       sar edx,ROUND
       push dx
       mov edx,.c1g
       sar edx,ROUND
       push dx
       mov edx,.c1b
       sar edx,ROUND
       push dx
       push cx
       sar ebx,ROUND
       push bx
       sar eax,ROUND
       push ax
       call gouraud_line

       pop ebp
       pop edi
       pop cx
       pop ebx
       pop eax

       mov edx,.dc13r
       add .c1r,edx
       mov edx,.dc13g
       add .c1g,edx
       mov edx,.dc13b
       add .c1b,edx
       mov edx,.dc12r
       add .c2r,edx
       mov edx,.dc12g
       add .c2g,edx
       mov edx,.dc12b
       add .c2b,edx

       add eax,.dx13
       add ebx,.dx12
       inc cx
       cmp cx,.y2
       jl .gt_loop1
.gt_loop1_end:

       mov cx,.y2
       cmp cx,.y3
       jge .gt_loop2_end
       movsx ebx,.x2
       shl ebx,ROUND

       movsx edx,word[.col2r]
       shl edx,ROUND
       mov .c2r,edx
       movsx edx,word[.col2g]
       shl edx,ROUND
       mov .c2g,edx
       movsx edx,word[.col2b]
       shl edx,ROUND
       mov .c2b,edx
.gt_loop2:
       push eax 		      ; eax - cur x1
       push ebx 		      ; ebx - cur x2
       push cx
       push edi
       push ebp

       mov edx,.c2r
       sar edx,ROUND
       push dx
       mov edx,.c2g
       sar edx,ROUND
       push dx
       mov edx,.c2b
       sar edx,ROUND
       push dx
       mov edx,.c1r
       sar edx,ROUND
       push dx
       mov edx,.c1g
       sar edx,ROUND
       push dx
       mov edx,.c1b
       sar edx,ROUND
       push dx
       push cx
       sar ebx,ROUND
       push bx
       sar eax,ROUND
       push ax
       call gouraud_line

       pop ebp
       pop edi
       pop cx
       pop ebx
       pop eax

       mov edx,.dc13r
       add .c1r,edx
       mov edx,.dc13g
       add .c1g,edx
       mov edx,.dc13b
       add .c1b,edx
       mov edx,.dc23r
       add .c2r,edx
       mov edx,.dc23g
       add .c2g,edx
       mov edx,.dc23b
       add .c2b,edx

       add eax,.dx13
       add ebx,.dx23
       inc cx
       cmp cx,.y3
       jl .gt_loop2
.gt_loop2_end:

      ; add esp,84
      mov esp,ebp
ret 18
gouraud_line:
;-------------in - edi - pointer to screen buffer
;----------------- stack - another parameters
.x1 equ word [ebp+4]
.x2 equ word [ebp+6]
.y equ word [ebp+8]
.col1b equ ebp+10
.col1g equ ebp+12
.col1r equ ebp+14
.col2b equ ebp+16
.col2g equ ebp+18
.col2r equ ebp+20
.dc_r equ dword[ebp-4]
.dc_g equ dword[ebp-8]
.dc_b equ dword[ebp-12]
       mov ebp,esp

       mov ax,.y
       or ax,ax
       jl .gl_quit
       cmp ax,SIZE_Y-1
       jg .gl_quit

       mov ax,.x1
       cmp ax,.x2
       je .gl_quit
       jl .gl_ok

       xchg ax,.x2
       mov .x1,ax
       mov eax,dword[.col1b]
       xchg eax,dword[.col2b]
       mov  dword[.col1b],eax
       mov ax,word[.col1r]
       xchg ax,word[.col2r]
       mov word[.col1r],ax
.gl_ok:
  ;     cmp .x1,SIZE_X-1  ;check
  ;     jg .gl_quit
  ;     cmp .x2,SIZE_X-1
  ;     jl @f
  ;     mov .x2,SIZE_X-1
  ;  @@:
  ;     cmp .x1,0
  ;     jg @f
  ;     mov .x1,0
  ;  @@:
  ;     cmp .x2,0
  ;     jl .gl_quit

       movsx ecx,.y
       mov eax,SIZE_X*3
       mul ecx
       movsx ebx,.x1
       lea ecx,[ebx*2+eax]
       add edi,ecx
       add edi,ebx

       mov ax,word[.col2r]
       sub ax,word[.col1r]
       cwde
       shl eax,ROUND
       cdq
       mov cx,.x2
       sub cx,.x1
       movsx ecx,cx
       idiv ecx
       ;mov .dc_r,eax           ;first delta
       push eax

       mov ax,word[.col2g]
       sub ax,word[.col1g]
       cwde
       shl eax,ROUND
       cdq
       idiv ecx
       ;mov .dc_g,eax
       push eax

       mov ax,word[.col2b]
       sub ax,word[.col1b]
       cwde
       shl eax,ROUND
       cdq
       idiv ecx
      ; mov .dc_b,eax
       push eax

       movsx ebx,word[.col1r]
       shl ebx,ROUND
       movsx edx,word[.col1g]
       shl edx,ROUND
       movsx esi,word[.col1b]
       shl esi,ROUND
.gl_draw:
       mov eax,ebx
       sar eax,ROUND
       stosb
       mov eax,edx
       sar eax,ROUND
       stosb
       mov eax,esi
       sar eax,ROUND
       stosb
       add ebx,.dc_r
       add edx,.dc_g
       add esi,.dc_b
       loop .gl_draw
.gl_quit:
      ; add esp,12
       mov esp,ebp
ret 18