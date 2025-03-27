# Projeto de Controle de LED com Botão e FreeRTOS no Raspberry Pi Pico

Este projeto utiliza o FreeRTOS para controlar um LED com base no estado de um botão conectado ao Raspberry Pi Pico. O código está organizado em três tarefas:

1. **Tarefa de Leitura do Botão**: Lê o estado do botão periodicamente a cada 100ms e envia esse estado para uma fila.
2. **Tarefa de Processamento do Botão**: Recebe o estado do botão da fila e processa o controle do LED.
3. **Tarefa de Controle do LED**: Controla o LED com base no estado do botão (aceso quando pressionado e apagado quando solto).

## Componentes Necessários

- **Raspberry Pi Pico**
- **Botão** (conectado ao pino GPIO 5)
- **LED** (conectado ao pino GPIO 12)
- **Resistor de Pull-Up** para o botão (para garantir um valor lógico alto quando o botão não for pressionado)

## Funcionalidade

- Quando o botão é pressionado, o LED acende.
- Quando o botão é solto, o LED apaga.

## Estrutura do Código

### `setup()`

Configura os pinos do botão e do LED:

- **Botão**: Pino GPIO 5 configurado como entrada com resistor de pull-up ativado.
- **LED**: Pino GPIO 12 configurado como saída.

### `vButtonTask()`

- Lê periodicamente o estado do botão (a cada 100ms).
- Envia o estado do botão para uma fila (`dataQueue`).

### `controlLedtask()`

- Recebe o estado do botão (0 ou 1) e controla o LED:
  - Se o botão for pressionado (estado 0), o LED é aceso.
  - Se o botão for solto (estado 1), o LED é apagado.

### `vProcessTask()`

- Recebe o estado do botão da fila.
- Chama a função `controlLedtask()` para controlar o LED de acordo com o estado do botão.

### `main()`

- Inicializa o botão e o LED.
- Cria as tarefas do FreeRTOS.
- Inicia o agendador do FreeRTOS.

## Compilação e Execução

1. Certifique-se de ter o **SDK do Raspberry Pi Pico** configurado corretamente.
2. Compile o código utilizando o sistema de build do Raspberry Pi Pico (CMake).
3. Carregue o código no Raspberry Pi Pico utilizando a ferramenta de gravação de firmware.

## Dependências

- **FreeRTOS**: Utilizado para gerenciamento das tarefas.
- **Pico SDK**: Necessário para compilar e rodar o código no Raspberry Pi Pico.

## Licença

Este projeto está licenciado sob a [MIT License](LICENSE).

