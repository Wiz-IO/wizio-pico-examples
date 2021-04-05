
void test_file(const char *filename);
void test_file_system(void);

#ifdef ARDUINO
#include <Arduino.h>
#include <VFS.h>

void led(int _delay)
{
  static int led = 0;
  digitalWrite(LED, led);
  led ^= 1;
  delay(_delay);
}

void setup()
{
  Serial.begin(115200, true);
  printf("\n\nArdiuno Raspberrypi PI Pico 2021 Georgi Angelov\n");
  pinMode(LED, OUTPUT);
  test_file_system();
}

void loop()
{
  led(500);
  Serial.print("loop ");
}

#else // PICO-SDK

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include <VFS.h>

#ifndef USE_FREERTOS

int main(void)
{
  stdio_init_all();
  printf("\n\nPICO-SDK Raspberrypi PI Pico 2021 Georgi Angelov\n");
  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
  test_file_system();
  while (true)
  {
    printf("LOOP \n");
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    sleep_ms(500);
    gpio_put(PICO_DEFAULT_LED_PIN, 0);
    sleep_ms(500);
  }
}

#else // RTOS

#include "FreeRTOS.h"
#include "task.h"

void vTask2(void *pvParameters)
{
  printf("[TASK 2] BEGIN\n");
  for (;;)
  {
    vTaskDelay(1000);
    printf("[TASK 2] %lu\n", xTaskGetTickCount());
  }
}

void vTask1(void *pvParameters)
{
  printf("[TASK 1] BEGIN\n");
  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
  for (;;)
  {
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    vTaskDelay(1000);
    gpio_put(PICO_DEFAULT_LED_PIN, 0);
    vTaskDelay(1000);
    printf("[TASK 1] %lu\n", to_ms_since_boot(get_absolute_time()));
  }
}

int main(void)
{
  stdio_init_all();
  printf("\n\nFreeRTOS Raspberrypi PI Pico 2021 Georgi Angelov\n");
  test_file_system();

  xTaskCreate(
      vTask1,               /* Function that implements the task. */
      "vTask1",             /* Text name for the task. */
      512,                  /* Stack size in words, not bytes. */
      (void *)1,            /* Parameter passed into the task. */
      tskIDLE_PRIORITY + 2, /* Priority at which the task is created. */
      NULL);

  xTaskCreate(
      vTask2,               /* Function that implements the task. */
      "vTask2",             /* Text name for the task. */
      512,                  /* Stack size in words, not bytes. */
      (void *)2,            /* Parameter passed into the task. */
      tskIDLE_PRIORITY + 2, /* Priority at which the task is created. */
      NULL);

  vTaskStartScheduler();
}

#endif

#endif

void test_file_system(void)
{
  if (0 == vfs_init())
  {
    test_file("R:/file_1.txt");
    test_file("R:/file_1.txt");
  }
}

void test_file(const char *filename)
{
  printf("\n----- TEST FileName: '%s' -----\n", filename);
  char text[64];
  char buff[64];
  int fd;
  sprintf(text, "Hello World ( millis = %d ms )\n", millis());
  printf("File Data: %s\n", text);
  fd = open(filename, O_WRONLY | O_CREAT, 0);
  if (fd > -1)
  {
    printf("[open] fd = %d\n", fd);
    printf(">>> %s", text);
    printf("[write] %d bytes\n", write(fd, text, strlen(text)));
    close(fd);
  }
  perror("[write status]");
  fd = open(filename, O_RDONLY, 0);
  if (fd > -1)
  {
    printf("\n[open] fd = %d\n", fd);
    memset(buff, 0, sizeof(buff));
    printf("[read] %d bytes\n<<< %s", read(fd, buff, sizeof(buff)), buff);
    close(fd);
  }
  perror("[read status]");
  FILE *pFile;
  pFile = fopen(filename, "r");
  if (pFile)
  {
    printf("\n[fopen] file = %p\n", pFile);
    memset(buff, 0, sizeof(buff));
    if (fgets(buff, sizeof(buff), pFile) != NULL)
      printf("<<< %s", buff);
    else
      perror("[fgets]");
    fclose(pFile);
  }
  perror("[fread status]");
  puts("-------------- END TEST ---------------");
}
