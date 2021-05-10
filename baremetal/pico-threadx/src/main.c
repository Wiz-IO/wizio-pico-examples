// https://github.com/azure-rtos/threadx
// http://www.ece.ualberta.ca/~cmpe490/documents/ghs/405/threadxug_g40c.pdf

#include <stdio.h>
#include "pico/stdlib.h"
#include "tx_api.h"

#define DEMO_STACK_SIZE 1024
#define DEMO_BYTE_POOL_SIZE 12000
UCHAR memory_area[DEMO_BYTE_POOL_SIZE];
TX_BYTE_POOL byte_pool_0;

void blink(void)
{
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    sleep_ms(100);
    gpio_put(PICO_DEFAULT_LED_PIN, 0);
    sleep_ms(1000);
}

void toggle(void)
{
    static int s = 0;
    gpio_put(PICO_DEFAULT_LED_PIN, s);
    s ^= 2;
}

TX_THREAD thread_1;
void thread_1_entry(ULONG thread_input)
{
    printf("[TH1] Begin\n");
    while (true)
    {
        printf("[TH1] +++++\n");
        toggle();
        tx_thread_sleep(500);
    }
}

TX_THREAD thread_2;
void thread_2_entry(ULONG thread_input)
{
    printf("[TH2] Begin\n");
    while (true)
    {
        printf("[TH2] -----\n");
        tx_thread_sleep(1000);
    }
}

void tx_application_define(void *first_unused_memory)
{
    printf("[THREADX] Init-Begin\n");
    CHAR *pointer = TX_NULL;

    /* Create a byte memory pool from which to allocate the thread stacks.  */
    tx_byte_pool_create(&byte_pool_0, "byte pool 0", &memory_area[0], DEMO_BYTE_POOL_SIZE);

    /* Allocate the stack for thread 1.  */
    tx_byte_allocate(&byte_pool_0, (VOID **)&pointer, DEMO_STACK_SIZE, TX_NO_WAIT);
    tx_thread_create(&thread_1, "thread 1", thread_1_entry, 1, pointer, DEMO_STACK_SIZE, 16, 16, 4, TX_AUTO_START);

    /* Allocate the stack for thread 2.  */
    tx_byte_allocate(&byte_pool_0, (VOID **)&pointer, DEMO_STACK_SIZE, TX_NO_WAIT);
    tx_thread_create(&thread_2, "thread 2", thread_2_entry, 2, pointer, DEMO_STACK_SIZE, 16, 16, 4, TX_AUTO_START);
    printf("[THREADX] Init-End\n");
}

int main(void)
{
    stdio_init_all();
    printf("\n\nThreadX Raspberrypi Pi Pico 2021 Georgi Angelov\n");
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    tx_kernel_enter();
    while (true)
    {
        blink();
    }
}
