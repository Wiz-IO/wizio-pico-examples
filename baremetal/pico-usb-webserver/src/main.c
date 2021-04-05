#include <stdio.h>
#include "pico/stdlib.h"

extern int rndis_init(void);
extern void rndis_task(void);

void APP(void *arg)
{
    printf("[APP] BEGIN\n");
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    while (true)
    {
        printf("[APP]\n");
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        vTaskDelay(100);
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        vTaskDelay(1000);
    }
}

void NDIS(void *arg)
{
    printf("[NET] BEGIN\n");
    rndis_init();
    while (1)
    {
        rndis_task();
        vTaskDelay(1);
    }
}

int main(void)
{
    stdio_init_all();
    printf("\n\nHello World\n");
    xTaskCreate(NDIS, "NDIS", 1024, NULL, 2, NULL);
    xTaskCreate(APP, "APP", 1024, NULL, 2, NULL);
    vTaskStartScheduler();
}
