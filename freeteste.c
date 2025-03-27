#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>

/** Tarefa
 * 1- Tarefa 1 (Leitura do Botão): Essa tarefa será responsável por simular a leitura de um botão. 
 * Ela será executada periodicamente, a cada 100ms, e enviará o estado do botão para a próxima tarefa.
 * 
   2- Tarefa 2 (Processamento do Botão): Receberá o estado do botão da Tarefa 1. 
   Caso o botão seja pressionado (simulado com a variável), ela acionará a próxima tarefa 
   (a de controlar o LED). Caso contrário, apenas aguardará o próximo ciclo de leitura.

   3- Tarefa 3 (Controle do LED): Controlará um LED (simulado como uma variável ou saída digital). 
   Se o botão for pressionado, o LED será aceso, caso contrário, será apagado. 
   A tarefa será executada sempre que for acionada pela Tarefa 2.
*/

#define BTN_A 5  // Define o pino do botão (GPIO 5)

const uint led_pin_red = 12;  // Define o pino do LED (GPIO 12)
QueueHandle_t dataQueue;  // Declara um identificador para a fila

/// @brief Faz a inicialização do botão e do LED
/// Essa função configura os pinos de entrada para o botão e de saída para o LED.
/// Além disso, ativa o resistor de pull-up para o botão (para garantir que ele tenha um valor lógico quando não pressionado).
void setup(){
    gpio_init(BTN_A);  // Inicializa o pino do botão
    gpio_set_dir(BTN_A, GPIO_IN);  // Define o pino como entrada
    gpio_pull_up(BTN_A);  // Ativa o resistor de pull-up no botão

    gpio_init(led_pin_red);  // Inicializa o pino do LED
    gpio_set_dir(led_pin_red, GPIO_OUT);  // Define o pino como saída
}

/// @brief Task responsável por acionar o botão
/// Esta tarefa é responsável por ler o estado do botão e enviar esse valor para a fila.
/// Ela é executada periodicamente a cada 100ms, lendo o valor do botão e enviando para a fila.
void vButtonTask(void *pvParameter){
    int button_state = 0;  // Variável para armazenar o estado do botão
    for(;;){  // Loop infinito para execução contínua da tarefa
        button_state = gpio_get(BTN_A);  // Lê o estado do botão (0 se pressionado, 1 se solto)
        xQueueSend(dataQueue, &button_state, portMAX_DELAY);  // Envia o estado do botão para a fila
        vTaskDelay(100 / portTICK_PERIOD_MS);  // Atraso de 100ms antes da próxima leitura
   }
}

/// @brief Função que controla o LED com base no estado do botão
/// @param state O estado do botão (0 se pressionado, 1 se solto)
/// Se o estado do botão for 0 (pressionado), o LED é aceso.
/// Se o estado do botão for 1 (solto), o LED é apagado.
void controlLedtask(int state){
    if(state == 0){  // Se o botão estiver pressionado
        gpio_put(led_pin_red, 1);  // Liga o LED
    }
    else{  // Se o botão não estiver pressionado
        gpio_put(led_pin_red, 0);  // Desliga o LED
    }
}

/// @brief Task responsável pelo processamento do estado do botão
/// Esta tarefa recebe o estado do botão da fila e aciona a função para controlar o LED.
/// O LED será controlado de acordo com o estado do botão (pressionado ou não).
void vProcessTask(void *pvParameter){
    int data;  // Variável para armazenar o dado recebido da fila
    for(;;){  // Loop infinito para execução contínua da tarefa
        if(xQueueReceive(dataQueue, &data, portMAX_DELAY) == pdTRUE){  // Espera até que um dado seja recebido da fila
            controlLedtask(data);  // Controla o LED de acordo com o estado do botão
        }
    }
}

int main(){
    setup();  // Inicializa o botão e o LED

    dataQueue = xQueueCreate(5, sizeof(int));  // Cria uma fila para armazenar o estado do botão (tamanho de 5 elementos)

    if(dataQueue != NULL){  // Verifica se a fila foi criada com sucesso
        // Cria as tarefas do FreeRTOS
        xTaskCreate(vButtonTask, "Task botão", 128, NULL, 1, NULL);  // Tarefa para ler o botão
        xTaskCreate(vProcessTask, "Task verificação", 128, NULL, 1, NULL);  // Tarefa para processar o estado do botão
        vTaskStartScheduler();  // Inicia o agendador do FreeRTOS
    }

    while(1);  // Caso o agendador não seja iniciado corretamente, o programa fica em um loop infinito
    return 0;  // Não será alcançado, pois o FreeRTOS toma controle da execução
}
