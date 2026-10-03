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
data2_value dw 0

; Element 3
data3_type db 04h      ; TYPE_SHORT
data3_value dw -12345

; Element 4
data4_type db 04h      ; TYPE_SHORT
data4_value dw 12

; Element 5
data5_type db 04h      ; TYPE_SHORT
data5_value dw 32767

; Element 6
data6_type db 04h      ; TYPE_SHORT
data6_value dw -32768

; Element 7
data7_type db 01h      ; TYPE_INT
data7_value dd 0

; Element 8
data8_type db 01h      ; TYPE_INT
data8_value dd 64000

; Element 9
data9_type db 01h      ; TYPE_INT
data9_value dd -2147483648

; Element 10
data10_type db 01h      ; TYPE_INT
data10_value dd 2147483647

; Element 11
data11_type db 05h      ; TYPE_UINT32
data11_value dd 0

; Element 12
data12_type db 05h      ; TYPE_UINT32
data12_value dd 12345

; Element 13
data13_type db 05h      ; TYPE_UINT32
data13_value dd 305419896

; Element 14
data14_type db 05h      ; TYPE_UINT32
data14_value dd 4294967295

; Element 15
data15_type db 02h      ; TYPE_STRING
data15_length dd 5
data15_value db 'Hello'

; Element 16
data16_type db 02h      ; TYPE_STRING
data16_length dd 1
data16_value db 'W'

; Element 17
data17_type db 02h      ; TYPE_STRING
data17_length dd 0
data17_value db 0

.code
start:
call ExitProcess
end start
