.586
.model flat, stdcall
ExitProcess PROTO :DWORD
includelib kernel32.lib

.data

; Element 0
data0_type db 03h      ; TYPE_BOOL
data0_value db 1

; Element 1
data1_type db 03h      ; TYPE_BOOL
data1_value db 0

; Element 2
data2_type db 04h      ; TYPE_SHORT
data2_value dw -12345

; Element 3
data3_type db 04h      ; TYPE_SHORT
data3_value dw 12

; Element 4
data4_type db 01h      ; TYPE_INT
data4_value dd 0

; Element 5
data5_type db 01h      ; TYPE_INT
data5_value dd 64000

; Element 6
data6_type db 02h      ; TYPE_STRING
data6_length dd 5
data6_value db 'Hello'

; Element 7
data7_type db 02h      ; TYPE_STRING
data7_length dd 1
data7_value db 'W'

.code
start:
call ExitProcess
end start
