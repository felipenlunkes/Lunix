;; Lunix Kernel

[BITS 32]

global initCOM1

initCOM1:

    mov bx, 0x3F8
    
    pusha
    
    push ds
    
    push cs
    pop ds
    
    mov al, 0
    mov dx, bx
    
    inc dx          ;; Porta + 1
    
    out dx, al      ;; Desativar interrupções

    mov dx, bx

    add dx, 3       ;; Porta + 3

    mov al, 10000000b   
    
    out dx, al      ;; Habilitar o DLAB (bit mais significativo), para que seja possível
                    ;; iniciar a definição do divisor da taxa de transmissão

;; Bits 7-7 : Habilitar DLAB
;; Bits 6-6 : Parar transmissão enquanto 1
;; Bits 3-5 : Paridade (0=nenhum)
;; Bits 2-2 : Contagem de bit de parada (0=1 bit de parada)
;; Bits 0-1 : Tamanho do caractere (5 a 8)

    mov al, 12
    mov dx, bx      ;; Porta + 0
    
    out dx, al      ;; Byte menos significativo do divisor
    
    mov al, 0
    
    mov dx, bx

    add dx, 1       ;; Porta + 1
    
    out dx, al      ;; Byte mais significante do divisor
                    ;; Isto produz uma taxa de 115200/12 = 9600

    mov al, 11000111b
    mov dx, bx

    add dx, 2       ;; Porta + 2
    
    out dx, al      ;; Manipulador de 14 bytes, habilitar FIFOs
                    ;; Limpar FIFO recebido, limpar FIFO transmitido

;; Bits 7-6 : Nível do manipulador de interrupção
;; Bits 5-5 : Habilitar FIFO de 64 bytes
;; Bits 4-4 : Reservado
;; Bits 3-3 : Seletor de modo DNA
;; Bits 2-2 : Limpar FIFO transmitido
;; Bits 1-1 : Limpar FIFO recebido
;; Bits 0-0 : Habilitar FIFOs
                
    mov al, 00000011b
    mov dx, bx

    add dx, 3       ;; Porta + 3
    
    out dx, al      
    
;; Desativar DLAB, e definir:
;;
;;  - Caractere de tamanho de 8 bits
;;  - Sem paridade
;;  - 1 bit de parada
                    
;; Bits 7-7 : Habilitar DLAB
;; Bits 6-6 : Parar transmissão enquanto 1
;; Bits 3-5 : Paridade (0=nenhum)
;; Bits 2-2 : Contagem de bit de parada (0=1 bit de parada)
;; Bits 0-1 : Tamanho do caractere (5 a 8)

    mov al, 00001011b
    mov dx, bx

    add dx, 4       ;; Porta + 4
    
    out dx, al      ;; Habilitar saída auxiliar 2 (também chamado de "ativar IRQ")

;; Bits 7-6 - Reservado
;; Bits 5-5 - Controle de fluxo automático ativado
;; Bits 4-4 - Modo de loopback
;; Bits 3-3 - Saída auxiliar 2
;; Bits 2-2 - Saída auxiliar 1
;; Bits 1-1 - Solicitação para enviar (RTS)
;; Bits 0-0 - Terminal de dados pronto (DTR)
    
    in al, 21h          ;; Ler bits de máscara IRQ do PIC principal
    
    and al, 11101111b   ;; Habilitar IRQ4, mantendo todos os outros IRQs inalterados
    
    out 21h, al         ;; Escrever bits de máscara de IRQ para PIC principal
    
    mov al, 1
    mov dx, bx

    add dx, 1           ;; Porta + 1
    
    out dx, al          ;; Habilitar interrupções
    
    pop ds
    
    popa
    
    ret

;;************************************************************************************

;