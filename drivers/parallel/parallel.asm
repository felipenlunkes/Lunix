[BITS 32]

global initParallel

initParallel:
    
    pusha
    
;; Reiniciar porta através do registrador de controle (base+2)
    
    mov dx, 3BCh
    
    add dx, 2           ;; Registro de controle (base+2)
    
    in al, dx
    
    mov al, 00001100b   
    
;; Bit 2 - Reiniciar porta
;; Bit 3 - Selecionar impressora
;; Bit 5 - Habilitar porta bi-direcional

    out dx, al          ;; Enviar sinal de reinício
    
    popa
        
    ret
