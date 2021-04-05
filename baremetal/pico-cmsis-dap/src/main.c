#include <stdio.h>
#include "pico/stdlib.h"
extern char unique_id[17];

void blink(int ms)
{
    if (0 == gpio_get_dir(PICO_DEFAULT_LED_PIN))
    {
        gpio_init(PICO_DEFAULT_LED_PIN);
        gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
        printf("[APP] Led init\n");
    }
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    sleep_ms(ms);
    gpio_put(PICO_DEFAULT_LED_PIN, 0);
    sleep_ms(ms);
}

int a = 1, b = 2, c = 3, begin = 0;

int main(void)
{
    while (0 == begin) // SET THIS VAR FROM DEBUG ( GLOABAL VAR )
        ;

    stdio_init_all();
    printf("\n\n[APP] Raspberry Pi Pico RP2040 CMSIS-DAP 2021 Georgi Angelov\n");
    printf("[APP] PICO ID: %s\n", unique_id);
    while (true)
    {
        blink(a);
        a += 10;
        printf("A = %d\n", a);
        blink(b);
        b += 20;
        printf("B = %d\n", b);
        blink(c);
        c += 30;
        printf("C = %d\n", c);
    }
}
