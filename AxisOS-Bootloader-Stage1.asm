[bits 16]
[org 0x7c00]

start:
    mov [BOOT_DRIVE], dl
    
    ; Печатаем сообщение
    mov si, msg_loading
    call print_string
    
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00

    ; Грузим Stage 2
    mov ah, 0x02
    mov al, 60
    mov ch, 0
    mov dh, 0
    mov cl, 2
    mov dl, [BOOT_DRIVE]
    mov bx, 0x8000      ; Загружаем по адресу 0x8000
    int 0x13
    jc disk_error

    ; ПЕРЕХОДИМ В PROTECTED MODE
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp 0x08:0x8000

print_string:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0e
    int 0x10
    jmp print_string
.done:
    ret

disk_error:
    mov si, msg_error
    call print_string
    jmp $

msg_loading db "Loading AxisOS kernel (60 sectors)...", 13, 10, 0
msg_error db "Disk Error! System halted.", 13, 10, 0

; Минимальная GDT для прыжка в Protected Mode
gdt_start:
    dd 0, 0
gdt_code:
    dw 0xffff, 0x0000
    db 0x00, 10011010b, 11001111b, 0x00
gdt_data:
    dw 0xffff, 0x0000
    db 0x00, 10010010b, 11001111b, 0x00
gdt_end:
gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

BOOT_DRIVE db 0
times 510-($-$$) db 0
dw 0xaa55
