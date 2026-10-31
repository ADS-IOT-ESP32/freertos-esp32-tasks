#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_err.h"

#define LED_PIN GPIO_NUM_4
#define QUANTIDADE_PISCADAS 5

static TaskHandle_t encerramento_handle = NULL;

static void tarefa_led(void *parametros)
{
    for (int i = 1; i <= QUANTIDADE_PISCADAS; i++) {
        printf("[LED] Piscada %d de %d: aceso\n",
               i, QUANTIDADE_PISCADAS);

        ESP_ERROR_CHECK(gpio_set_level(LED_PIN, 1));
        vTaskDelay(pdMS_TO_TICKS(500));

        ESP_ERROR_CHECK(gpio_set_level(LED_PIN, 0));
        printf("[LED] Apagado\n");
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    printf("[LED] Sequencia concluida. Encerrando tarefa LED.\n");

    // Avisa a outra tarefa que as cinco piscadas terminaram.
    xTaskNotifyGive(encerramento_handle);

    vTaskDelete(NULL);
}

static void tarefa_encerramento(void *parametros)
{
    printf("[CONTROLE] Aguardando a conclusao das piscadas...\n");

    // Fica bloqueada ate receber a notificacao da tarefa LED.
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    printf("[CONTROLE] Conclusao recebida.\n");
    printf("[CONTROLE] LED apagado. Encerrando tarefa de controle.\n");

    vTaskDelete(NULL);
}

void app_main(void)
{
    ESP_ERROR_CHECK(gpio_reset_pin(LED_PIN));
    ESP_ERROR_CHECK(gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_level(LED_PIN, 0));

    printf("\n=== Demonstracao FreeRTOS: LED ===\n");

    BaseType_t resultado;

    // Cria primeiro a tarefa que recebera a notificacao.
    resultado = xTaskCreatePinnedToCore(
        tarefa_encerramento,
        "Encerramento",
        4096,
        NULL,
        2,
        &encerramento_handle,
        0
    );
    configASSERT(resultado == pdPASS);

    resultado = xTaskCreatePinnedToCore(
        tarefa_led,
        "LED",
        4096,
        NULL,
        1,
        NULL,
        0
    );
    configASSERT(resultado == pdPASS);
}