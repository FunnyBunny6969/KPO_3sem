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
    V_L1_2 SDWORD -32768
    V_L2_3 SDWORD 32767
    V_L3_4 SDWORD 0
    V_L4_5 SDWORD -127
    V_L5_6_RAW BYTE 45, 45, 45, 32, 210, 229, 241, 242, 32, 247, 232, 241, 229, 235, 32, 232, 32, 227, 240, 224, 237, 232, 246, 32, 45, 45, 45, 0
    V_L5_6 DWORD OFFSET V_L5_6_RAW
    V_L6_7 SDWORD -5
    V_L7_8 SDWORD 10
    V_L8_9 SDWORD 2
    V_L9_10 SDWORD -3
    V_L11_11 SDWORD 15
    V_L12_12 SDWORD 240
    V_L13_13_RAW BYTE 45, 45, 45, 32, 207, 238, 225, 232, 242, 238, 226, 251, 229, 32, 238, 239, 229, 240, 224, 246, 232, 232, 32, 45, 45, 45, 0
    V_L13_13 DWORD OFFSET V_L13_13_RAW
    V_L15_14 SDWORD -2
    V_L18_15 SDWORD -1
    V_L19_16 SDWORD 1
    V_L22_17 DWORD 90
    V_L23_18_RAW BYTE 209, 232, 236, 226, 238, 235, 58, 32, 0
    V_L23_18 DWORD OFFSET V_L23_18_RAW
    V_L24_19_RAW BYTE 194, 226, 229, 228, 232, 242, 229, 32, 247, 232, 241, 235, 238, 58, 32, 0
    V_L24_19 DWORD OFFSET V_L24_19_RAW
    V_L26_20 SDWORD 5
    V_L27_21_RAW BYTE 50, 32, 226, 32, 241, 242, 229, 239, 229, 237, 232, 32, 53, 32, 61, 32, 0
    V_L27_21 DWORD OFFSET V_L27_21_RAW
    V_L28_22 SDWORD -42
    V_L29_23_RAW BYTE 215, 232, 241, 235, 238, 32, 226, 32, 241, 242, 240, 238, 234, 243, 58, 32, 0
    V_L29_23 DWORD OFFSET V_L29_23_RAW
    V_L30_24 SDWORD 26
    V_L33_25_RAW BYTE 200, 242, 238, 227, 32, 226, 235, 238, 230, 229, 237, 237, 251, 245, 32, 246, 232, 234, 235, 238, 226, 32, 40, 238, 230, 232, 228, 224, 229, 242, 241, 255, 32, 57, 41, 58, 32, 0
    V_L33_25 DWORD OFFSET V_L33_25_RAW

.data
    V_min_val_27 SDWORD 0
    V_max_val_28 SDWORD 0
    V_zero_val_29 SDWORD 0
    V_hex_val_30 SDWORD 0
    V_calc_31 SDWORD 0
    V_a_33 SDWORD 0
    V_b_34 SDWORD 0
    V_res_and_35 SDWORD 0
    V_res_or_36 SDWORD 0
    V_res_not_37 SDWORD 0
    V_limit_39 SDWORD 0
    V_i_40 SDWORD 0
    V_j_41 SDWORD 0
    V_total_42 SDWORD 0
    V_user_num_43 SDWORD 0
    V_symbol_44 DWORD 0
    V_str_num_45 DWORD OFFSET EMPTY_STR
    V_powered_46 SDWORD 0
    V_r1_47 SDWORD 0
    V_r2_48 SDWORD 0
    V_r3_49 SDWORD 0

.code

F_test_numbers_26 PROC

    mov eax, V_L1_2
    movsx eax, ax
    mov V_min_val_27, eax
    mov eax, V_L2_3
    movsx eax, ax
    mov V_max_val_28, eax
    mov eax, V_L3_4
    movsx eax, ax
    mov V_zero_val_29, eax
    mov eax, V_L4_5
    movsx eax, ax
    mov V_hex_val_30, eax
    mov eax, V_L5_6
    push eax
    call _std_print_str
    mov eax, V_min_val_27
    push eax
    call _std_print_short
    mov eax, V_max_val_28
    push eax
    call _std_print_short
    mov eax, V_zero_val_29
    push eax
    call _std_print_short
    mov eax, V_hex_val_30
    push eax
    call _std_print_short
    mov eax, V_L6_7
    push eax
    mov eax, V_L7_8
    mov ebx, eax
    pop eax
    add ax, bx
    movsx eax, ax
    push eax
    mov eax, V_L8_9
    push eax
    mov eax, V_L9_10
    mov ebx, eax
    pop eax
    imul bx
    movsx eax, ax
    mov ebx, eax
    pop eax
    sub ax, bx
    movsx eax, ax
    movsx eax, ax
    mov V_calc_31, eax
    mov eax, V_calc_31
    push eax
    call _std_print_short
    mov eax, V_max_val_28
    push eax
    mov eax, V_L2_3
    mov ebx, eax
    pop eax
    sub ax, bx
    movsx eax, ax
    movsx eax, ax
    jmp EXIT_F_test_numbers_26_1
EXIT_F_test_numbers_26_1:
    ret
F_test_numbers_26 ENDP

F_test_bitwise_32 PROC P_ARG_0:SDWORD, P_ARG_1:SDWORD
    mov eax, P_ARG_0
    mov V_a_33, eax
    mov eax, P_ARG_1
    mov V_b_34, eax

    mov eax, V_a_33
    push eax
    mov eax, V_L11_11
    mov ebx, eax
    pop eax
    and ax, bx
    movsx eax, ax
    movsx eax, ax
    mov V_res_and_35, eax
    mov eax, V_b_34
    push eax
    mov eax, V_L12_12
    mov ebx, eax
    pop eax
    or ax, bx
    movsx eax, ax
    movsx eax, ax
    mov V_res_or_36, eax
    mov eax, V_a_33
    not ax
    movsx eax, ax
    movsx eax, ax
    mov V_res_not_37, eax
    mov eax, V_L13_13
    push eax
    call _std_print_str
    mov eax, V_res_and_35
    push eax
    call _std_print_short
    mov eax, V_res_or_36
    push eax
    call _std_print_short
    mov eax, V_res_not_37
    push eax
    call _std_print_short
    mov eax, V_res_and_35
    push eax
    mov eax, V_res_or_36
    mov ebx, eax
    pop eax
    or ax, bx
    movsx eax, ax
    push eax
    mov eax, V_L3_4
    not ax
    movsx eax, ax
    mov ebx, eax
    pop eax
    and ax, bx
    movsx eax, ax
    movsx eax, ax
    jmp EXIT_F_test_bitwise_32_2
EXIT_F_test_bitwise_32_2:
    ret
F_test_bitwise_32 ENDP

F_test_nested_while_38 PROC P_ARG_0:SDWORD
    mov eax, P_ARG_0
    mov V_limit_39, eax

    mov eax, V_L15_14
    movsx eax, ax
    mov V_i_40, eax
    mov eax, V_L3_4
    movsx eax, ax
    mov V_total_42, eax
WHILE_START_4:
    mov eax, V_i_40
    push eax
    mov eax, V_limit_39
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
    mov eax, V_L8_9
    movsx eax, ax
    mov V_j_41, eax
WHILE_START_8:
    mov eax, V_j_41
    push eax
    mov eax, V_L18_15
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
    mov eax, V_total_42
    push eax
    mov eax, V_L19_16
    mov ebx, eax
    pop eax
    add ax, bx
    movsx eax, ax
    movsx eax, ax
    mov V_total_42, eax
    mov eax, V_j_41
    push eax
    mov eax, V_L19_16
    mov ebx, eax
    pop eax
    sub ax, bx
    movsx eax, ax
    movsx eax, ax
    mov V_j_41, eax
    jmp WHILE_START_8
WHILE_END_9:
    mov eax, V_i_40
    push eax
    mov eax, V_L19_16
    mov ebx, eax
    pop eax
    add ax, bx
    movsx eax, ax
    movsx eax, ax
    mov V_i_40, eax
    jmp WHILE_START_4
WHILE_END_5:
    mov eax, V_total_42
    movsx eax, ax
    jmp EXIT_F_test_nested_while_38_3
EXIT_F_test_nested_while_38_3:
    ret
F_test_nested_while_38 ENDP

main PROC
    push 1251
    call SetConsoleCP
    push 1251
    call SetConsoleOutputCP

    mov eax, V_L22_17
    mov V_symbol_44, eax
    mov eax, V_L23_18
    push eax
    call _std_print_str
    mov eax, V_symbol_44
    push eax
    call _std_print_char
    mov eax, V_L24_19
    push eax
    call _std_print_str
    call _std_input_short
    movsx eax, ax
    mov V_user_num_43, eax
    mov eax, V_L26_20
    push eax
    mov eax, V_L8_9
    push eax
    call _std_pow
    movsx eax, ax
    mov V_powered_46, eax
    mov eax, V_L27_21
    push eax
    call _std_print_str
    mov eax, V_powered_46
    push eax
    call _std_print_short
    mov eax, V_L28_22
    push eax
    call _std_itos
    mov V_str_num_45, eax
    mov eax, V_L29_23
    push eax
    call _std_print_str
    mov eax, V_str_num_45
    push eax
    call _std_print_str
    call F_test_numbers_26
    movsx eax, ax
    mov V_r1_47, eax
    mov eax, V_L26_20
    push eax
    mov eax, V_L30_24
    push eax
    call F_test_bitwise_32
    movsx eax, ax
    mov V_r2_48, eax
    mov eax, V_L19_16
    push eax
    call F_test_nested_while_38
    movsx eax, ax
    mov V_r3_49, eax
    mov eax, V_L33_25
    push eax
    call _std_print_str
    mov eax, V_r3_49
    push eax
    call _std_print_short
    mov eax, V_L3_4
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
