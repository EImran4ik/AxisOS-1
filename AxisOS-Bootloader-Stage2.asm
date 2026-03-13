[bits 32]
global stage2_start
extern _kernel_main

stage2_start:
    ; Обновляем сегменты для 32-бит
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Настройка стека
    mov ebp, 0x90000
    mov esp, ebp

    ; ВЫЗОВ СИ
    call _kernel_main

    jmp $
