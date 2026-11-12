.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib StdLib.lib

ExitProcess PROTO :DWORD
SetConsoleCP PROTO :DWORD
SetConsoleOutputCP PROTO :DWORD

_std_print_short PROTO :SDWORD
_std_print_char  PROTO :DWORD
_std_print_str   PROTO :DWORD
_std_input_short PROTO
_std_input_char  PROTO
_std_pow         PROTO :SDWORD, :SDWORD
_std_itos        PROTO :SDWORD
_std_topolish    PROTO :DWORD

.const
    EMPTY_STR BYTE 0
    V_L1_3 SDWORD -32768
    V_L2_4 SDWORD 32767
    V_L3_5 SDWORD 0
    V_L4_6 SDWORD -127
    V_L5_7_RAW BYTE 45, 45, 45, 32, 210, 229, 241, 242, 32, 247, 232, 241, 229, 235, 32, 232, 32, 227, 240, 224, 237, 232, 246, 32, 45, 45, 45, 0
    V_L5_7 DWORD OFFSET V_L5_7_RAW
    V_L6_8 SDWORD -5
    V_L7_9 SDWORD 10
    V_L8_10 SDWORD 2
    V_L9_11 SDWORD -3
    V_L11_12 SDWORD 15
    V_L12_13 SDWORD 240
    V_L13_14_RAW BYTE 45, 45, 45, 32, 207, 238, 225, 232, 242, 238, 226, 251, 229, 32, 238, 239, 229, 240, 224, 246, 232, 232, 32, 45, 45, 45, 0
    V_L13_14 DWORD OFFSET V_L13_14_RAW
    V_L15_15 SDWORD -2
    V_L18_16 SDWORD -1
    V_L19_17 SDWORD 1
    V_L22_18 DWORD 90
    V_L23_19_RAW BYTE 209, 232, 236, 226, 238, 235, 58, 32, 0
    V_L23_19 DWORD OFFSET V_L23_19_RAW
    V_L24_20_RAW BYTE 194, 226, 229, 228, 232, 242, 229, 32, 247, 232, 241, 235, 238, 58, 32, 0
    V_L24_20 DWORD OFFSET V_L24_20_RAW
    V_L26_21 SDWORD 5
    V_L27_22_RAW BYTE 50, 32, 226, 32, 241, 242, 229, 239, 229, 237, 232, 32, 53, 32, 61, 32, 0
    V_L27_22 DWORD OFFSET V_L27_22_RAW
    V_L28_23 SDWORD -42
    V_L29_24_RAW BYTE 215, 232, 241, 235, 238, 32, 226, 32, 241, 242, 240, 238, 234, 243, 58, 32, 0
    V_L29_24 DWORD OFFSET V_L29_24_RAW
    V_L30_25 SDWORD 26
    V_L33_26_RAW BYTE 200, 242, 238, 227, 32, 226, 235, 238, 230, 229, 237, 237, 251, 245, 32, 246, 232, 234, 235, 238, 226, 32, 40, 238, 230, 232, 228, 224, 229, 242, 241, 255, 32, 57, 41, 58, 32, 0
    V_L33_26 DWORD OFFSET V_L33_26_RAW
    V_L34_27_RAW BYTE 40, 97, 32, 43, 32, 98, 41, 32, 42, 32, 40, 99, 32, 45, 32, 50, 41, 0
    V_L34_27 DWORD OFFSET V_L34_27_RAW

.data
    V_min_val_29 SDWORD 0
    V_max_val_30 SDWORD 0
    V_zero_val_31 SDWORD 0
    V_hex_val_32 SDWORD 0
    V_calc_33 SDWORD 0
    V_a_35 SDWORD 0
    V_b_36 SDWORD 0
    V_res_and_37 SDWORD 0
    V_res_or_38 SDWORD 0
    V_res_not_39 SDWORD 0
    V_limit_41 SDWORD 0
    V_i_42 SDWORD 0
    V_j_43 SDWORD 0
    V_total_44 SDWORD 0
    V_user_num_45 SDWORD 0
    V_symbol_46 DWORD 0
    V_str_num_47 DWORD OFFSET EMPTY_STR
    V_powered_48 SDWORD 0
    V_r1_49 SDWORD 0
    V_r2_50 SDWORD 0
    V_r3_51 SDWORD 0

.code

F_test_numbers_28 PROC

    mov eax, V_L1_3
    movsx eax, ax
    mov V_min_val_29, eax
    mov eax, V_L2_4
    movsx eax, ax
    mov V_max_val_30, eax
    mov eax, V_L3_5
    movsx eax, ax
    mov V_zero_val_31, eax
    mov eax, V_L4_6
    movsx eax, ax
    mov V_hex_val_32, eax
    mov eax, V_L5_7
    push eax
    call _std_print_str
    mov eax, V_min_val_29
    push eax
    call _std_print_short
    mov eax, V_max_val_30
    push eax
    call _std_print_short
    mov eax, V_zero_val_31
    push eax
    call _std_print_short
    mov eax, V_hex_val_32
    push eax
    call _std_print_short
    mov eax, V_L6_8
    push eax
    mov eax, V_L7_9
    mov ebx, eax
    pop eax
    add ax, bx
    movsx eax, ax
    push eax
    mov eax, V_L8_10
    push eax
    mov eax, V_L9_11
    mov ebx, eax
    pop eax
    imul bx
    movsx eax, ax
    mov ebx, eax
    pop eax
    sub ax, bx
    movsx eax, ax
    movsx eax, ax
    mov V_calc_33, eax
    mov eax, V_calc_33
    push eax
    call _std_print_short
    mov eax, V_max_val_30
    push eax
    mov eax, V_L2_4
    mov ebx, eax
    pop eax
    sub ax, bx
    movsx eax, ax
    movsx eax, ax
    jmp EXIT_F_test_numbers_28_1
EXIT_F_test_numbers_28_1:
    ret
F_test_numbers_28 ENDP

F_test_bitwise_34 PROC P_ARG_0:SDWORD, P_ARG_1:SDWORD
    mov eax, P_ARG_0
    mov V_a_35, eax
    mov eax, P_ARG_1
    mov V_b_36, eax

    mov eax, V_a_35
    push eax
    mov eax, V_L11_12
    mov ebx, eax
    pop eax
    and ax, bx
    movsx eax, ax
    movsx eax, ax
    mov V_res_and_37, eax
    mov eax, V_b_36
    push eax
    mov eax, V_L12_13
    mov ebx, eax
    pop eax
    or ax, bx
    movsx eax, ax
    movsx eax, ax
    mov V_res_or_38, eax
    mov eax, V_a_35
    not ax
    movsx eax, ax
    movsx eax, ax
    mov V_res_not_39, eax
    mov eax, V_L13_14
    push eax
    call _std_print_str
    mov eax, V_res_and_37
    push eax
    call _std_print_short
    mov eax, V_res_or_38
    push eax
    call _std_print_short
    mov eax, V_res_not_39
    push eax
    call _std_print_short
    mov eax, V_res_and_37
    push eax
    mov eax, V_res_or_38
    mov ebx, eax
    pop eax
    or ax, bx
    movsx eax, ax
    push eax
    mov eax, V_L3_5
    not ax
    movsx eax, ax
    mov ebx, eax
    pop eax
    and ax, bx
    movsx eax, ax
    movsx eax, ax
    jmp EXIT_F_test_bitwise_34_2
EXIT_F_test_bitwise_34_2:
    ret
F_test_bitwise_34 ENDP

F_test_nested_while_40 PROC P_ARG_0:SDWORD
    mov eax, P_ARG_0
    mov V_limit_41, eax

    mov eax, V_L15_15
    movsx eax, ax
    mov V_i_42, eax
    mov eax, V_L3_5
    movsx eax, ax
    mov V_total_44, eax
WHILE_START_4:
    mov eax, V_i_42
    push eax
    mov eax, V_limit_41
    mov ebx, eax
    pop eax
    cmp ax, bx
    jl CMP_TRUE_6
    mov eax, 0
    jmp CMP_DONE_7
CMP_TRUE_6:
    mov eax, 1
CMP_DONE_7:
    cmp ax, 0
    je WHILE_END_5
    mov eax, V_L8_10
    movsx eax, ax
    mov V_j_43, eax
WHILE_START_8:
    mov eax, V_j_43
    push eax
    mov eax, V_L18_16
    mov ebx, eax
    pop eax
    cmp ax, bx
    jg CMP_TRUE_10
    mov eax, 0
    jmp CMP_DONE_11
CMP_TRUE_10:
    mov eax, 1
CMP_DONE_11:
    cmp ax, 0
    je WHILE_END_9
    mov eax, V_total_44
    push eax
    mov eax, V_L19_17
    mov ebx, eax
    pop eax
    add ax, bx
    movsx eax, ax
    movsx eax, ax
    mov V_total_44, eax
    mov eax, V_j_43
    push eax
    mov eax, V_L19_17
    mov ebx, eax
    pop eax
    sub ax, bx
    movsx eax, ax
    movsx eax, ax
    mov V_j_43, eax
    jmp WHILE_START_8
WHILE_END_9:
    mov eax, V_i_42
    push eax
    mov eax, V_L19_17
    mov ebx, eax
    pop eax
    add ax, bx
    movsx eax, ax
    movsx eax, ax
    mov V_i_42, eax
    jmp WHILE_START_4
WHILE_END_5:
    mov eax, V_total_44
    movsx eax, ax
    jmp EXIT_F_test_nested_while_40_3
EXIT_F_test_nested_while_40_3:
    ret
F_test_nested_while_40 ENDP

main PROC
    push 1251
    call SetConsoleCP
    push 1251
    call SetConsoleOutputCP

    mov eax, V_L22_18
    mov V_symbol_46, eax
    mov eax, V_L23_19
    push eax
    call _std_print_str
    mov eax, V_symbol_46
    push eax
    call _std_print_char
    mov eax, V_L24_20
    push eax
    call _std_print_str
    call _std_input_short
    movsx eax, ax
    mov V_user_num_45, eax
    mov eax, V_L26_21
    push eax
    mov eax, V_L8_10
    push eax
    call _std_pow
    movsx eax, ax
    mov V_powered_48, eax
    mov eax, V_L27_22
    push eax
    call _std_print_str
    mov eax, V_powered_48
    push eax
    call _std_print_short
    mov eax, V_L28_23
    push eax
    call _std_itos
    mov V_str_num_47, eax
    mov eax, V_L29_24
    push eax
    call _std_print_str
    mov eax, V_str_num_47
    push eax
    call _std_print_str
    call F_test_numbers_28
    movsx eax, ax
    mov V_r1_49, eax
    mov eax, V_L26_21
    push eax
    mov eax, V_L30_25
    push eax
    call F_test_bitwise_34
    movsx eax, ax
    mov V_r2_50, eax
    mov eax, V_L19_17
    push eax
    call F_test_nested_while_40
    movsx eax, ax
    mov V_r3_51, eax
    mov eax, V_L33_26
    push eax
    call _std_print_str
    mov eax, V_r3_51
    push eax
    call _std_print_short
    mov eax, V_L34_27
    push eax
    call _std_topolish
    push eax
    call _std_print_str
    mov eax, V_L3_5
    movsx eax, ax
    jmp EXIT_main_12
EXIT_main_12:
    push 0
    call ExitProcess
main ENDP

RUNTIME_DIV_ZERO:
    push -1
    call ExitProcess

end main
