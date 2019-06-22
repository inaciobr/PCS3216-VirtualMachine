        @   /0000     ; Loader e armazenado na posicao 0x0000
        
                      ; Referente aos primeiros bytes de informacao do arquivo binario
INIT    IO  /1        ; Obtem dados de um arquivo informado a VM
        MM  ADDR      ; Salva o primeiro byte do endereco onde o programa sera carregado
        IO  /1
        MM  ADDR+1    ; Salva o segundo byte
        IO  /1
        MM  SIZE      ; Salva o tamanho do programa, em bytes

                      ; Loop de leitura do programa
LOOP    IO  /1        ; Le um byte de dados do programa
        CN  /2        ; Ativa o modo indireto
        MM  ADDR      ; Salva o byte no endereco apontado por ADDR

                      ; Atualiza endereco onde o programa sera armazenado.
        LD  ADDR+1    ; Obtem o endereco atual
        +   ONE       ; Adiciona 1 ao valor do endereco
        MM  ADDR+1    ; Salva o endereco atualizado

                      ; Atualiza o numero de bytes restantes.
        LD  SIZE      ; Obtem o tamanho do programa, em bytes
        -   ONE       ; Subtrai 1
        MM  SIZE      ; Salva o novo tamanho do programa

        JZ  END       ; Quando todos os bytes tiverem sido lidos, pula para END.
        JP  LOOP      ; Caso contrario, pula de volta para LOOP.

END     OS  /F        ; Finaliza o programa
                
                      ; Posicoes onde os valores das informacoes do programa serao armazenados.
ADDR    K   0
        K   0
SIZE    K   0
ONE     K   1
        # INIT        ; Finaliza o loader.
