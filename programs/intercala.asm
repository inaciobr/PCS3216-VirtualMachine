        @   /0100
                
INIT
LOOP
        LD  COUNT     ; Carrega o valor de COUNT
        /   TWO       ; Resto da divisão
        *   TWO
        -   COUNT
        MM  ARR+COUNT ; Salva o valor na array.

        LD  COUNT     ; Obtem o valor da variável COUNT
        -   ONE       ; Subtrai 1
        MM  COUNT     ; Salva o novo valor da variável COUNT

        JZ  END       ; Quando o loop terminar, pula para a label END
        JP  LOOP      ; Caso contrario, pula de volta para LOOP

END     OS  /F        ; Finaliza o programa


                      ; Variáveis
COUNT   K   /A

ARR     $   300
                      ; Definição de valores.
ONE     K   01
TWO     K   02

        # INIT
