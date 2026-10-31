# FreeRTOS na ESP32-CAM

Atividade prática de criação, execução e sincronização de tarefas com FreeRTOS, utilizando o ESP-IDF e uma placa ESP32-CAM.

## Objetivo

Controlar o LED da placa por meio de uma tarefa e notificar outra tarefa quando as cinco piscadas forem concluídas.

## Funcionamento

- **Tarefa LED:** pisca o LED branco conectado ao GPIO 4 cinco vezes, mantendo-o aceso por 500 ms e apagado por 500 ms. Depois, notifica a tarefa de encerramento e exclui a si própria.
- **Tarefa de encerramento:** aguarda a notificação, exibe as mensagens de conclusão e exclui a si própria.

Ao final, o LED permanece apagado. O FreeRTOS continua ativo, executando as tarefas internas do sistema.

As duas tarefas da aplicação estão fixadas no núcleo 0. A tarefa de encerramento tem prioridade 2, e a tarefa LED tem prioridade 1.

## Hardware e ferramentas

- ESP32-CAM com base ESP32-CAM-MB.
- Cabo USB com transferência de dados.
- Windows e Visual Studio Code.
- ESP-IDF v6.1.

O exemplo utiliza o LED de flash no GPIO 4 da ESP32-CAM modelo AI-Thinker. Para este teste, mantenha o slot microSD sem cartão.

## Estrutura principal

- `main/hello_world_main.c`: implementação das tarefas e controle do LED.
- `main/CMakeLists.txt`: registro do componente e dependência `esp_driver_gpio`.
- `CMakeLists.txt`: configuração geral do projeto.
- `sdkconfig`: configurações utilizadas.
- `.gitignore`: exclusão dos arquivos gerados pela compilação.

Os nomes `hello_world_main.c` e `hello_world.bin` foram mantidos do exemplo usado como base.

## Compilar e executar

No terminal com o ambiente ESP-IDF ativado, entre na pasta do projeto e execute:

```powershell
idf.py set-target esp32
idf.py build
idf.py -p COM5 flash monitor
```

Substitua `COM5` pela porta da sua placa. A seleção do alvo é necessária na configuração inicial do projeto.

Para sair do monitor serial, pressione **Ctrl + ]**.

Para repetir a demonstração, pressione **RST/RESET** na placa. O firmware permanece gravado e também executa novamente quando a placa é ligada.

### Alternativa para o terminal do VS Code

Na instalação utilizada neste trabalho, caso `idf.py` abra no editor em vez de executar, chame o Python do ESP-IDF explicitamente:

```powershell
& "C:\Espressif\tools\python\v6.1\venv\Scripts\python.exe" "C:\esp\v6.1\esp-idf\tools\idf.py" build
```

```powershell
& "C:\Espressif\tools\python\v6.1\venv\Scripts\python.exe" "C:\esp\v6.1\esp-idf\tools\idf.py" -p COM5 flash monitor
```

Esses comandos pressupõem que o ambiente ESP-IDF esteja ativado. Ajuste os caminhos conforme sua instalação.

## Resultado

O teste na placa física confirmou cinco piscadas do LED, seguidas das mensagens de conclusão no monitor serial e do encerramento das duas tarefas da aplicação.