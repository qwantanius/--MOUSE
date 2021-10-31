;This file is a test of using external assembly language
;routines with Turbo C
;
_TEXT   segment byte public 'CODE'    ;define segment
        assume cs: _TEXT
        public _clear;
        public _gotoyx;
;
;***** The _clear function can be adapted to be called with
;      parameters, to be used with windows.
;      Therefore, it sets up the bp register for accessing parameters.
;
_clear  proc near   ;define procedure
        push bp     ;save old bp
        mov bp,sp   ;load current sp to bp
        mov ah,6    ;window scroll function
        mov al,0    ;code to blank screen
        mov ch,0    ;y value
        mov cl,0    ;x value
        mov dh,24   ;y1
        mov dl,79   ;x1
        mov bh,7    ;blank line attribute
        int 10h     ;video interrupt
        pop bp      ;restore bp
        sub ax,ax   ;zero ax
        push ax     ;push row number
        push ax     ;push column number
        call _gotoyx  ;position cursor at 0,0
        pop cx      ;cleanup parameters off the stack
        pop cx      ; ditto
        ret
_clear  endp        ;end procedure
;
_gotoyx proc near   ;define procedure
        push bp     ;save old bp
        mov bp,sp   ;
        mov ah,2    ;function number
        mov dh,[bp+4]   ;y in dh
        mov dl,[bp+6]   ;x in dl
        mov bh,0    ;current page
        int 10h     ;video interrupt
        pop bp      ;restore bp
        ret         ;near return
_gotoyx endp        ;end procedure
;
_TEXT   ends
end
