.include "xc.inc"
.text                       ;BP (put the following data in ROM(program memory))
; This is a library, thus it can *not* contain a _main function: the C file will
; deine main().  However, we
; we will need a .global statement to make available ASM functions to C code.
; All functions utilized outside of this file will need to have a leading
; underscore (_) and be included in a comment delimited list below.
.global _write_0, _write_1, _wait_100us, _wait_1ms
    
_write_0: ;2
    inc LATA ;1
    repeat #3 ;1
    nop ;4
    clr LATA ;1
    repeat #6 ;1
    nop ;7
    return ;3
    

_write_1: ;2
    inc LATA ;1
    repeat #9 ;1
    nop ;10
    clr LATA ;1
    nop; 1
    nop;1
    return ;3

_wait_100us: ;1600
    repeat #1598 ;1
    nop ;1599
    return 


_wait_1ms: ;16000
    repeat #15998 ;1
    nop ;15999
    return